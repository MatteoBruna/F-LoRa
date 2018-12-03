#include <RFM69.h>

#include "includes/config.h"
#include "includes/blink.h"
#include "includes/radio.h"

static RFM69 radio;
static long last_period = -1;

void serial_init(long baud) {
    Serial.begin(baud);
}

void setup() {
    serial_init(SERIAL_BAUD);
    radio_init(radio, FREQUENCY, NODEID, NETWORKID);

    char buff[50];
    sprintf(buff, "\nTransmitting at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
    Serial.println(buff);
}


void loop() {
    int transmit_period = 1000; //transmit a packet to gateway so often (in ms)
    Message message;
    message.node_id = NODEID;
    message.uptime = millis();
    message.temp = 91.23; //it's hot!
    //fill in the struct with new values

    int curr_period = millis()/transmit_period;
    if (curr_period != last_period) {
        blink(300);
        radio_send(radio, GATEWAYID, message);
        last_period=curr_period;
    }
}

