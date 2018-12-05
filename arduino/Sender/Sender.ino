//based on mailbox notifier

#include <Adafruit_Sensor.h>
#include <DHT.h> //basato su adafruit unified sensor....works but it's huuuuge!
#include <DHT_U.h>
#include <RFM69.h>
#include <RFM69_ATC.h>
#include <SPI.h>
#include <SPIFlash.h>
#include <LowPower.h>

#include "includes/config.h"

DHT_Unified dht(DHTPIN, DHTTYPE);  //create dht object

#ifdef ENABLE_ATC    //create radio object
  RFM69_ATC radio;
#else
  RFM69 radio;
#endif

volatile boolean motionDetected=false;
char sendBuf[61];
byte sendLen;
byte sendLoops=0;
boolean requestACK = false;

unsigned long MLO=0; //MotionLastObserved (ago, in ms)
unsigned long now = 0, time=0, lastSend = 0;
unsigned int batterylevel = 0;
unsigned int temp, relhum; //global variables for DHT22 measured values
unsigned int soilmoisture; 

void setup() {
  	// Initialize devices
    serial_init();
    led_init();
    dht_init();
	  radio_init();
 // if (flash.initialize()) flash.sleep(); //if Moteino has FLASH-MEM, make sure it sleeps

  radio.sleep();
  
  pinMode(BATT_MONITOR, INPUT);
  pinMode(FEEDDHT, OUTPUT);
  pinMode(FEEDMOIST, OUTPUT);
  //setup interrupt pin for motion sensor to wake up microcontroller
  pinMode(MOTION_PIN, INPUT);
  //attachInterrupt(MOTION_IRQ, motionIRQ, RISING);

}

void radio_init() {
    radio.initialize(FREQUENCY,NODEID,NETWORKID);
#ifdef IS_RFM69HW_HCW
    radio.setHighPower(); //must include this only for RFM69HW/HCW!
#endif
    //radio.encrypt(KEY); encryption not used now for simplicity

#ifdef ENABLE_ATC
  radio.enableAutoPower(ATC_RSSI);
  DEBUGln("RFM69_ATC Enabled (Auto Transmission Control)\n");
#endif
}

void dht_init() {
  	dht.begin();
}

void serial_init() {
  #ifdef SERIAL_EN
    Serial.begin(SERIAL_BAUD);
  #endif  
}

void motionIRQ()
{
  motionDetected=true;
  DEBUGln("IRQ");
}

void loop() {  //voglio che mandi il read dei sensori una volta ogni tot e il movimento ogni volta che lo registra. Quando registra il movimento non deve leggere i sensori, cosi' risparmio energia
  now = millis();
  readBattery();

  if (motionDetected && (time-MLO > DUPLICATE_INTERVAL))
  {
    DEBUG("MOTION");
    MLO = time; //save timestamp of event
    sprintf(sendBuf, "MOTION,%d", batterylevel);
    sendLen = strlen(sendBuf); DEBUGln(sendBuf);
    if (radio.sendWithRetry(GATEWAYID, sendBuf, sendLen))
    {
     DEBUGln("..OK");
     #ifdef BLINK_EN
       Blink(LED,3);
     #endif
    }
    else DEBUGln("..NOK");
    radio.sleep();
  }
  else sendLoops++;

  //send sensor readings every SENDEVERYXLOOPS
  if (sendLoops>=SENDEVERYXLOOPS)
  {
    dhtmeas(); //read the sensors
    soilmoist();
    
    sendLoops=0;
    char periodO='X';
    unsigned long lastOpened = (time - MLO) / 1000; //get seconds
    
    if (lastOpened <= 59) periodO = 's'; //1-59 seconds
    else if (lastOpened <= 3599) { periodO = 'm'; lastOpened/=60; } //1-59 minutes
    else if (lastOpened <= 259199) { periodO = 'h'; lastOpened/=3600; } // 1-71 hours
    else if (lastOpened >= 259200) { periodO = 'd'; lastOpened/=86400; } // >=3 days
   
    DEBUG("lastOpened: ");
    DEBUGln(lastOpened);
    DEBUG("periodO: ");
    DEBUGln(periodO);
    DEBUG("batterylevel: ");
    DEBUGln(batterylevel);
    DEBUG("temp: ");
    DEBUGln(temp);
    DEBUG("relhum: ");
    DEBUGln(relhum);
    DEBUG("soilmoisture: ");
    DEBUGln(soilmoisture);
            
    sprintf(sendBuf, "%ld,%c,%d,%d,%d,%d", lastOpened, periodO, batterylevel, temp, relhum, soilmoisture); //es: 56,m,48,15,69,96
    sendLen = strlen(sendBuf);
    radio.send(GATEWAYID, sendBuf, sendLen);
    radio.sleep();
    DEBUG(sendBuf); DEBUG(" ("); DEBUG(sendLen); DEBUGln(")"); 
    lastSend = time;
    #ifdef BLINK_EN
      Blink(3);
    #endif
  }
  
  motionDetected=false; //do NOT move this after the SLEEP line below or motion will never be detected
  time = time + 8000 + millis()-now + 480; //correct millis() resonator drift, may need to be tweaked to be accurate
  //LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  delay(5000);
  DEBUGln("WAKEUP");
}

void led_init() {
    pinMode(LED, OUTPUT);//once deployed it's not useful...comment out
}

void Blink(int DELAY_MS) {
    digitalWrite(LED, HIGH);
    delay(DELAY_MS);
    digitalWrite(LED, LOW);
}


void dhtmeas()
{ 
unsigned int n = 0; //variable for repeating the measurements if necessary
digitalWrite(FEEDDHT, HIGH); //feed the sensor from OUTPUT pin
delay(1500); //the sensor needs time to warm up and give reliable measurements
sensors_event_t event;

  // Get temperature event and print its value.
  for (byte i=0; i<3; i++){
    dht.temperature().getEvent(&event);
    if (!isnan(event.temperature)) {
      temp = (int) (event.temperature * 10);
      break;
      }
     else {
       temp = 999;
      }
    }
  
  // Get humidity event and print its value.
  for (byte i=0; i<3; i++){
    dht.humidity().getEvent(&event);
    if (!isnan(event.relative_humidity)) {
      relhum = (int) (event.relative_humidity * 10);
      break;
      }
     else {
       relhum = 999;
      }
    }

  digitalWrite(FEEDDHT, LOW); //switch off sensor
  }

void readBattery()
{
  unsigned int readings=0;
  for (byte i=0; i<5; i++) //take several samples, and average
    readings+=analogRead(BATT_MONITOR);
    batterylevel = (int) ((readings / 5.0)*16.4)-906;
}

void soilmoist()
{
  digitalWrite(FEEDMOIST, HIGH); //feed the sensor
  unsigned int readings=0;
  for (byte i=0; i<5; i++) //take several samples, and average
    readings+=analogRead(MOISTPIN);
    soilmoisture = readings / 5;
  digitalWrite(FEEDMOIST, LOW); //feed the sensor
}
