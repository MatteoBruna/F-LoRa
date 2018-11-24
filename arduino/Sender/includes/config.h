#define NODEID      2
#define NETWORKID   100
#define GATEWAYID   1
#define FREQUENCY   RF69_433MHZ //Match this with the version of your Moteino! (others: RF69_433MHZ, RF69_868MHZ)
#define KEY         "sampleEncryptKey" //has to be same 16 characters/bytes on all nodes, not more not less!
#define LED         9
#define SERIAL_BAUD 9600
#define ACK_TIME    30  // # of ms to wait for an ack
#define IS_RFM69HW_HCW  true//uncomment only for RFM69HW/HCW! Leave out if you have RFM69W/CW!

#define DHTPIN            3         // Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT22     // DHT 22 (AM2302) oppure DHT11 o DHT21
