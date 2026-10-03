const byte LED_COUNT = 18;
const byte LED_PINS[LED_COUNT] = {
  2,3,4,5,6,7,8,9,10,11,12,13,
  A0,A1,A2,A3,A4,A5
};

void setup() {
  for (byte i=0; i<LED_COUNT; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }
}

void loop() {
  for (byte i=0; i<LED_COUNT; i++) {
    digitalWrite(LED_PINS[i], HIGH);
    delay(250);
    digitalWrite(LED_PINS[i], LOW);
    delay(80);
  }
}
