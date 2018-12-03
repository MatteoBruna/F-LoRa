#include "includes/radio.h"
#include "includes/blink.h"


void radio_init(RFM69 &radio, int frequency, int node_id, int network_id) {
  radio.initialize(frequency, node_id, network_id);
#ifdef IS_RFM69HW_HCW
  radio.setHighPower(); //must include this only for RFM69HW/HCW!
#endif
//  rad.encrypt(KEY);
}


void radio_receive(RFM69 &radio) {
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
        blink(100);
        Serial.println();
    }
}

void radio_send(RFM69 &radio, int dest_node_id, Message &message) {
    Serial.print("Sending struct (");
    Serial.print(sizeof(message));
    Serial.print(" bytes) ... ");
    if (radio.sendWithRetry(dest_node_id, (const void*)(&message), sizeof(message))) {
        Serial.print(" ok!");
    } else {
        Serial.print(" nothing...");
    }
    Serial.println();
}

