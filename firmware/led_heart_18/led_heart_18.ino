/*
  Arduino Nano + 18 LED Kalp
  LED1..LED12 = D2..D13
  LED13..LED18 = A0..A5
  Buton = A6 (10k pull-up ile)
  A7 = randomSeed için boş
*/

const byte LED_COUNT = 18;
const byte LED_PINS[LED_COUNT] = {
  2,3,4,5,6,7,8,9,10,11,12,13,
  A0,A1,A2,A3,A4,A5
};

const byte BUTTON_PIN = A6;
const byte RANDOM_PIN = A7;
const byte MODE_COUNT = 7;

byte mode = 0;
bool lastButtonPressed = false;
unsigned long lastButtonTime = 0;
unsigned long lastEffectTime = 0;
int positionLed = 0;
int directionLed = 1;

void allOff() {
  for (byte i = 0; i < LED_COUNT; i++) digitalWrite(LED_PINS[i], LOW);
}

void showOne(byte index) {
  allOff();
  if (index < LED_COUNT) digitalWrite(LED_PINS[index], HIGH);
}

void resetEffect() {
  allOff();
  positionLed = 0;
  directionLed = 1;
  lastEffectTime = 0;
}

void checkButton() {
  int raw = analogRead(BUTTON_PIN);
  bool pressed = raw < 300;

  if (pressed && !lastButtonPressed && millis() - lastButtonTime > 180) {
    mode++;
    if (mode >= MODE_COUNT) mode = 0;
    resetEffect();
    lastButtonTime = millis();
  }

  lastButtonPressed = pressed;
}

void effectChase() {
  if (millis() - lastEffectTime < 80) return;
  lastEffectTime = millis();
  showOne(positionLed);
  positionLed = (positionLed + 1) % LED_COUNT;
}

void effectPingPong() {
  if (millis() - lastEffectTime < 70) return;
  lastEffectTime = millis();
  showOne(positionLed);
  positionLed += directionLed;

  if (positionLed >= LED_COUNT - 1) {
    positionLed = LED_COUNT - 1;
    directionLed = -1;
  } else if (positionLed <= 0) {
    positionLed = 0;
    directionLed = 1;
  }
}

void effectOppositePair() {
  if (millis() - lastEffectTime < 100) return;
  lastEffectTime = millis();

  allOff();
  digitalWrite(LED_PINS[positionLed], HIGH);
  digitalWrite(LED_PINS[(positionLed + 9) % LED_COUNT], HIGH);
  positionLed = (positionLed + 1) % LED_COUNT;
}

void effectSparkle() {
  if (millis() - lastEffectTime < 110) return;
  lastEffectTime = millis();

  allOff();
  for (byte i = 0; i < 3; i++) {
    digitalWrite(LED_PINS[random(LED_COUNT)], HIGH);
  }
}

void effectComet() {
  if (millis() - lastEffectTime < 90) return;
  lastEffectTime = millis();

  allOff();
  for (byte i = 0; i < 4; i++) {
    int index = positionLed - i;
    while (index < 0) index += LED_COUNT;
    digitalWrite(LED_PINS[index], HIGH);
  }
  positionLed = (positionLed + 1) % LED_COUNT;
}

bool showWholeHeart(unsigned long durationMs, byte expectedMode) {
  unsigned long start = millis();

  while (millis() - start < durationMs) {
    checkButton();
    if (mode != expectedMode) {
      allOff();
      return false;
    }

    // Aynı anda 3 LED: toplam akımı düşük tutar.
    for (byte group = 0; group < 6; group++) {
      allOff();
      for (byte i = group; i < LED_COUNT; i += 6) {
        digitalWrite(LED_PINS[i], HIGH);
      }
      delayMicroseconds(1200);
    }
  }

  allOff();
  return true;
}

void effectHeartbeat() {
  const byte activeMode = 6;

  if (!showWholeHeart(110, activeMode)) return;
  delay(75);
  checkButton();
  if (mode != activeMode) return;

  if (!showWholeHeart(180, activeMode)) return;
  delay(380);
}

void setup() {
  for (byte i = 0; i < LED_COUNT; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }

  randomSeed(analogRead(RANDOM_PIN));
  allOff();
}

void loop() {
  checkButton();

  switch (mode) {
    case 0: allOff(); break;
    case 1: effectChase(); break;
    case 2: effectPingPong(); break;
    case 3: effectOppositePair(); break;
    case 4: effectSparkle(); break;
    case 5: effectComet(); break;
    case 6: effectHeartbeat(); break;
  }
}
