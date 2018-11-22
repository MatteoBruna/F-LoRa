typedef struct {
  int           node_id; // store this node_id
  unsigned long uptime; // uptime in ms
  float         temperature;        // temperature in celsius
  float         relative_humidity;  // humidity in percent
} Message;
