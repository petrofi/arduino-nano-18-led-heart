const byte BUTTON_PIN = A6;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int value = analogRead(BUTTON_PIN);
  Serial.print("A6 = ");
  Serial.print(value);
  Serial.print("  Durum = ");
  Serial.println(value < 300 ? "BASILI" : "SERBEST");
  delay(150);
}
