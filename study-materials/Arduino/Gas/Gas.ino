int LED = D2; // Warning output
int Gas_analog = A0; // Analog signal
int Gas_digital = D4; // Digital signal

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(Gas_analog, INPUT);
  pinMode(Gas_digital, INPUT);
}

void loop() {
  int gassensorAnalog = analogRead(Gas_analog);
  int gassensorDigital = digitalRead(Gas_digital);
  
  Serial.print("Gas Sensor: ");
  Serial.print(gassensorAnalog);
  Serial.print("\t");
  Serial.print("Gas Class: ");
  Serial.print(gassensorDigital);
  Serial.print("\t");
  
  if (gassensorAnalog > 500) {
    Serial.println("Gas");
    digitalWrite(LED, HIGH); // blinking
    delay(1000);
    digitalWrite(LED, LOW);  // turn off
  }
  else {
    Serial.println("No Gas");
  }
  delay(100);
}
