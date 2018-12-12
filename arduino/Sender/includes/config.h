//////////////////////////////
// **RADIO Settings **

#define NODEID      2
#define NETWORKID   100
#define GATEWAYID   1
// Match this with the radio! (others: RF69_433MHZ, RF69_868MHZ)
#define FREQUENCY   RF69_433MHZ
//has to be same 16 characters/bytes on all nodes, not more not less! NOT IN USE NOW FOR SIMPLICITY
//#define KEY         "sampleEncryptKey"
// # of ms to wait for an ack 30 is standard but it's too short and with senwithretry I sometimes would get two massages across...anche cosi', non e' questo...mi sa che il sendwithretry devo farlo manuale
#define ACK_TIME    100
//uncomment only for RFM69HW/HCW! Leave out if you have RFM69W/CW!
#define IS_RFM69HW_HCW  true
// each loop sleeps 8 seconds, so send status message every this many sleep cycles (default "4" = 32 seconds)
#define SENDEVERYXLOOPS   4
//#define BLINK_EN         //uncomment to make LED flash when messages are sent, leave out if you want low power

//ATC = Auto Transmission Control - dials down transmit power to save battery (-100 is the noise floor, -90 is still pretty good)
//For indoor nodes that are pretty static and at pretty stable temperatures (like a MotionMote) -90dBm is quite safe
//For more variable nodes that can expect to move or experience larger temp drifts a lower margin like -70 to -80 would probably be better
//Always test your ATC mote in the edge cases in your own environment to ensure ATC will perform as you expect
//comment out this line to disable AUTO TRANSMISSION CONTROL
#define ENABLE_ATC
#define ATC_RSSI      -75

//////////////////////////////
// ** Moisture sensor settings **

// pin to feed DHT sensor
#define FEEDDHT        5
// Pin which is connected to the DHT sensor.
#define DHTPIN         4
// DHT 22 (AM2302) or DHT11 or DHT21
#define DHTTYPE        DHT22
// pin to feed soil moisture sensor
#define FEEDMOIST      6
// analog pin to measure soil moisture
#define MOISTPIN       A1

//////////////////////////////
// ** Motion sensor settings **
#define MOTION_PIN     3
// hardware interrupt 1 (D3) - where motion sensors OUTput is connected, this will generate an interrupt every time there is MOTION
#define MOTION_IRQ     1
// avoid duplicates in 30second intervals (ie mailman sometimes spends 30+ seconds at mailbox)
#define DUPLICATE_INTERVAL 20000

//////////////////////////////
// ** Battery monitor settings **

// Sense VBAT_COND signal
#define BATT_MONITOR  A0

// ** OnBoard LED settings **
#define LED         9

//////////////////////////////
// ** Serial Settings **
// comment this out when deploying to an installed Node to save a few KB of sketch size
#define SERIAL_EN
#define SERIAL_BAUD    115200

