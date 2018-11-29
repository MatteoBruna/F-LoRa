#include "includes/blink.h"


void blink(int delay_ms)
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  delay(delay_ms);
  digitalWrite(LED_PIN, LOW);
}

