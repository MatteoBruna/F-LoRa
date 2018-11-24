#include <Adafruit_Sensor.h>
#include <DHT.h> //basato su adafruit unified sensor....works but it's huuuuge!
#include <DHT_U.h>
#include <RFM69.h>
#include <SPI.h>
#include <SPIFlash.h>

#include "includes/config.h"

DHT_Unified dht(DHTPIN, DHTTYPE);

int TRANSMITPERIOD = 2500; //transmit a packet to gateway so often (in ms)
byte sendSize=0;
boolean requestACK = false;

RFM69 radio;

typedef struct {
  int           nodeId; //store this nodeId
  unsigned long uptime; //uptime in ms
  float         temp;   //sensor readings
  float         relhum;
} Payload;
Payload theData;
long lastPeriod = -1;

void setup() {
  	// Initialize DHT device.
    serial_init();
    led_init();
    dht_init();
	radio_init();
}

void radio_init() {
    radio.initialize(FREQUENCY,NODEID,NETWORKID);
#ifdef IS_RFM69HW_HCW
    radio.setHighPower(); //must include this only for RFM69HW/HCW!
#endif
    radio.encrypt(KEY);

    char buff[128];
    snprintf(buff, 128, "Transmitting at %d Mhz...", (FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915));
    Serial.println(buff);
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
        float temp, relhum;
        dhtstuff2(&temp, &relhum);

        //fill in the struct with new values
        theData.nodeId = NODEID;
        theData.uptime = millis();
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
    pinMode(LED, OUTPUT);
}

void led_blink(int DELAY_MS) {
    digitalWrite(LED, HIGH);
    delay(DELAY_MS);
    digitalWrite(LED, LOW);
}

void dhtstuff() {
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
        Serial.println("Error reading temperature!");
    } else {
        Serial.print("Temperature: ");
        Serial.print(event.temperature);
        Serial.println(" *C");
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
        Serial.println("Error reading humidity!");
    } else {
        Serial.print("Humidity: ");
        Serial.print(event.relative_humidity);
        Serial.println("%");
    }
}

void dhtstuff2(float* temp, float* relhum) {
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
        *temp = 999;
    } else {
        *temp = event.temperature;
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
        *relhum = 999;
    } else {
        *relhum = event.relative_humidity;
    }
}
