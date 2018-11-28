#define NODEID      2
#define NETWORKID   100
#define GATEWAYID   1
#define FREQUENCY   RF69_433MHZ //Match this with the radio! (others: RF69_433MHZ, RF69_868MHZ)
//#define KEY         "sampleEncryptKey" //has to be same 16 characters/bytes on all nodes, not more not less! NOT IN USE NOW FOR SIMPLICITY
#define LED         9
#define SERIAL_BAUD 9600
#define ACK_TIME    100  // # of ms to wait for an ack 30 is standard but it's too short and with senwithretry I sometimes would get two massages across...anche cosi', non e' questo...mi sa che il sendwithretry devo farlo manuale
#define IS_RFM69HW_HCW  true//uncomment only for RFM69HW/HCW! Leave out if you have RFM69W/CW!

#define DHTPIN            4         // Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT22     // DHT 22 (AM2302) or DHT11 or DHT21

//ATC = Auto Transmission Control - dials down transmit power to save battery (-100 is the noise floor, -90 is still pretty good)
//For indoor nodes that are pretty static and at pretty stable temperatures (like a MotionMote) -90dBm is quite safe
//For more variable nodes that can expect to move or experience larger temp drifts a lower margin like -70 to -80 would probably be better
//Always test your ATC mote in the edge cases in your own environment to ensure ATC will perform as you expect
#define ENABLE_ATC    //comment out this line to disable AUTO TRANSMISSION CONTROL
#define ATC_RSSI      -75

#define MOTION_PIN     3  // D3
#define MOTION_IRQ     1  // hardware interrupt 1 (D3) - where motion sensors OUTput is connected, this will generate an interrupt every time there is MOTION
#define BATT_MONITOR  A0  // Sense VBAT_COND signal 
#define BATT_FORMULA(reading) reading * 0.00322 * 1.49 // >>> fine tune this parameter to match your voltage when fully charged

#define DUPLICATE_INTERVAL 20000 //avoid duplicates in 30second intervals (ie mailman sometimes spends 30+ seconds at mailbox)
#define SEND_INTERVAL  300000  // read and send all data every this ms (approx)
#define SERIAL_EN             //comment this out when deploying to an installed Mote to save a few KB of sketch size
#define SERIAL_BAUD    115200
#ifdef SERIAL_EN
#define DEBUG(input)   {Serial.print(input); delay(1);}
#define DEBUGln(input) {Serial.println(input); delay(1);}
#define DEBUGFlush() { Serial.flush(); }
#else
#define DEBUG(input);
#define DEBUGln(input);
#define DEBUGFlush();
#endif
