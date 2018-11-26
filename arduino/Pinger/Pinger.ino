#include <Adafruit_Sensor.h>
#include <RFM69.h>
#include <SPI.h>
#include <SPIFlash.h>

#include "includes/config.h"

int TRANSMITPERIOD = 2500; //transmit a packet to gateway so often (in ms)
byte sendSize=0;
boolean requestACK = false;

RFM69 radio;

typedef struct {
  int           node_id; 
} Message;
Message message;

long lastPeriod = -1;

void setup() {
    serial_init();
    led_init();
	radio_init();
}

void radio_init() {
    radio.initialize(FREQUENCY, NODEID, NETWORKID);
}

void serial_init() {
  	Serial.begin(SERIAL_BAUD);
}

void loop() {
    int currPeriod = millis()/TRANSMITPERIOD;   //meglio di "delay()"
    if (currPeriod != lastPeriod) {
        //fill in the struct with new values
        message.node_id = NODEID;

        Serial.print("Sending struct (");
        Serial.print(sizeof(message));
        Serial.print(" bytes) ... ");
        if (radio.sendWithRetry(GATEWAYID, (const void*)(&message), sizeof(message))) {
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

