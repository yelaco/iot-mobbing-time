const int Relay = D1;

void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode(Relay, OUTPUT);
  Serial.println("Motor Shield 12E Initialized");
  delay(5000);
}

void loop() {
  Serial.println("Turn On Light!");
  digitalWrite(Relay, HIGH);
  delay(4500);
  Serial.println("Turn Off Light!");
  digitalWrite(Relay, LOW);
  delay(4500);
}
