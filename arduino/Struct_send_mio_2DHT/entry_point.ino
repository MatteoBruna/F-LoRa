/*
#include <Adafruit_Sensor.h>
#include <DHT.h> //basato su adafruit unified sensor....works but it's huuuuge!
#include <DHT_U.h>
#include <RFM69.h>
#include <SPI.h>
#include <SPIFlash.h>
*/
#include "includes/serial.h"
#include "includes/constants.h"
#include "includes/message.h"
/*

int TRANSMITPERIOD = 2500; //transmit a packet to gateway so often (in ms)
byte sendSize=0;
boolean requestACK = false;

RFM69 radio;
DHT_Unified dht(DHTPIN, DHTTYPE);
Message theData;
long lastPeriod = -1;

*/
void setup() {
  serial_init(SERIAL_BAUD);
  Serial.println("hello");
//  dht_init();
//  radio_init();
//  char buff[50];
//  sprintf(buff, "\nTransmitting at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
//  Serial.println(buff);

}

void loop() {
/*

  //check for any received packets
  if (radio.receiveDone())
  {
    Serial.print('[');
    Serial.print(radio.SENDERID, DEC);
    Serial.print("] ");

    for (byte i = 0; i < radio.DATALEN; i++) {
      Serial.print((char)radio.DATA[i]);
    }

    Serial.print("   [RX_RSSI:");
    Serial.print(radio.readRSSI());
    Serial.print("]");

    if (radio.ACKRequested())
    {
      radio.sendACK();
      Serial.print(" - ACK sent");
      delay(10);
    }
    led_blink(LED,5);
    Serial.println();
  }

  int currPeriod = millis()/TRANSMITPERIOD;   //meglio di "delay()"
  if (currPeriod != lastPeriod)
  {
    float temp, relhum;
    dhtstuff2(&temp, &relhum);

    //fill in the struct with new values
    theData.node_id = NODEID;
    theData.uptime = millis();
    theData.temperature = temp; //sensor data
    theData.relative_humidity = relhum;

    Serial.print("Sending struct (");
    Serial.print(sizeof(theData));
    Serial.print(" bytes) ... ");
    if (radio.sendWithRetry(GATEWAYID, (const void*)(&theData), sizeof(theData)))
      Serial.print(" ok!");
    else Serial.print(" nothing...");
    Serial.println();
    led_blink(LED,3);
    lastPeriod=currPeriod;
  }
*/
}
