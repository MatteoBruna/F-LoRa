#include <RFM69.h>

#include "includes/config.h"
#include "includes/blink.h"
#include "includes/radio.h"

static RFM69 radio;

void serial_init(long baud) {
    Serial.begin(baud);
}

void setup() {
    serial_init(SERIAL_BAUD);
    radio_init(radio, FREQUENCY, NODEID, NETWORKID);

    char buff[50];
    sprintf(buff, "\nReceiving at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
    Serial.println(buff);
}


void loop() {
    radio_receive(radio);
}

