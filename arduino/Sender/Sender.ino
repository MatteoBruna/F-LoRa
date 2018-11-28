#include <Adafruit_Sensor.h>
#include <DHT.h> //basato su adafruit unified sensor....works but it's huuuuge!
#include <DHT_U.h>
#include <RFM69.h>
#include <SPI.h>
#include <SPIFlash.h>
#include <LowPower.h>

#include "includes/config.h"

DHT_Unified dht(DHTPIN, DHTTYPE);

int TRANSMITPERIOD = 5000; //transmit a packet to gateway so often (in ms)
byte sendSize=0;
boolean requestACK = false;

#ifdef ENABLE_ATC
  RFM69_ATC radio;
#else
  RFM69 radio;
#endif

typedef struct {
  //int           nodeId; //store this nodeId ...it's actually useless because RFM69 class already gets the node's ID
  bool          motion; //flag if data was sent due to motion or as regular communication
  int         temp;   //sensor readings. I declare them int --> they will be sent as integers using the *10/10 trick
  int         relhum;
  //int       moist; //soil moisture sensor
  int         battlev; //measure battery level
} Payload;
Payload theData;
long lastPeriod = -1;

void setup() {
  	// Initialize DHT device.
    serial_init();
    led_init();
    dht_init();
	  radio_init();
 // if (flash.initialize()) flash.sleep(); //if Moteino has FLASH-MEM, make sure it sleeps
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

    char buff[128];
    snprintf(buff, 128, "Transmitting at %d Mhz...", (FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915));
    Serial.println(buff);

//setup interrupt pin for motion sensor to wake up microcontroller
  pinMode(MOTION_PIN, INPUT);
  attachInterrupt(MOTION_IRQ, motionIRQ, RISING);
}

void dht_init() {
  	dht.begin();
}

void serial_init() {
  	Serial.begin(SERIAL_BAUD);
}

void loop() {
    //check for any received packets
    if (radio.receiveDone()) {
        Serial.print('[');
        Serial.print(radio.SENDERID, DEC);
        Serial.print("] ");
        for (byte i = 0; i < radio.DATALEN; i++) {
            Serial.print((char)radio.DATA[i]);
        }
        Serial.print("   [RX_RSSI:");
        Serial.print(radio.readRSSI());
        Serial.print("]");

        if (radio.ACKRequested()) {
          radio.sendACK();
          Serial.print(" - ACK sent");
          delay(10);
        }
        led_blink(5);
        Serial.println();
    }

    int currPeriod = millis()/TRANSMITPERIOD;   //meglio di "delay()"
    if (currPeriod != lastPeriod) {
        int temp, relhum;
        dhtstuff(&temp, &relhum);

        //fill in the struct with new values
        //theData.nodeId = NODEID;
        //theData.uptime = millis();
        theData.temp = temp; //sensor data
        theData.relhum = relhum;

        Serial.print("Sending struct (");
        Serial.print(sizeof(theData));
        Serial.print(" bytes) ... ");
        if (radio.sendWithRetry(GATEWAYID, (const void*)(&theData), sizeof(theData))) {
            Serial.print(" ok!");
        }
        else {
            Serial.print(" nothing...");
        }
        Serial.println();
        led_blink(3);
        lastPeriod=currPeriod;
    }
}

void led_init() {
    pinMode(LED, OUTPUT);//once deployed it's not useful...comment out
}

void led_blink(int DELAY_MS) {
    digitalWrite(LED, HIGH);
    delay(DELAY_MS);
    digitalWrite(LED, LOW);
}


void dhtstuff(int* temp, int* relhum)
{ 

sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    *temp = 999;
  }
  else {
    *temp = (int) (event.temperature * 10);
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    *relhum = 999;
  }
  else {
    *relhum = (int) (event.relative_humidity * 10);
  }
  
  return;
  }

//  void readBattery(int* batlev)
//{
//  unsigned int readings=0;
//
//  for (byte i=0; i<5; i++) //take several samples, and average
//    readings+=analogRead(BATT_MONITOR);
 
//  batteryVolts = BATT_FORMULA(readings / 5.0);
//  dtostrf(batteryVolts,3,2, BATstr); //update the BATStr which gets sent every BATT_CYCLES or along with the MOTION message
//  if (batteryVolts <= BATT_LOW) BATstr = "LOW";
//}
