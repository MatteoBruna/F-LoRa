#if 0
#define PIR_OUT (3) /* PIR OUT pin*/

volatile bool active;

void setup(void)
{
  active = false;
  Serial.begin(9600);
  pinMode(PIR_OUT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIR_OUT), callback, CHANGE);
}

void loop() {
    Serial.print(active);
    delay(1000.0);
}

void callback() {
    noInterrupts();
    active = !active;
    interrupts();
} 
#endif

const byte ledPin = LED_BUILTIN;
const byte interruptPin = 3;
volatile byte state = LOW;

void blink() {
  state = !state;
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

void loop() {
  digitalWrite(ledPin, state);
}


