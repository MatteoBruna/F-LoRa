#ifndef RADIO_H
#define RADIO_H
#include <Arduino.h>
#include <RFM69.h>

typedef struct {
  int           node_id; //store this nodeId
  unsigned long uptime; //uptime in ms
  float         temp;   //temperature maybe?
} Message;


void radio_init(RFM69 &rad, int frequency, int node_id, int network_id);
void radio_receive(RFM69 &radio);
void radio_send(RFM69 &radio, int dest_node_id, Message &message);

#endif
