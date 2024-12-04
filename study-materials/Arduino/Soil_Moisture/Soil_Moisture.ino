int LED = D1; // Warning output
int analog_sig = A0; // Analog signal
int digital_sig = D2; // Digital signal

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(analog_sig, INPUT);
  pinMode(digital_sig, INPUT);
}

void loop() {
  // Read analog value of soil moisture and convert to percent
  // Read ten times and then take the average of the real value
  for (int i = 0; i < 9; i++) {
    real_value += analogRead(analog_sig);
  }

  value = real_value / 10;

  // Mapping the real analog value (getting by experiments) to percent 0-100
  int percent = map(value, 500, 1023, 0, 100);

  // By default, 100% means dry, 0% means wet. This command reverses it.
  percent = 100 - percent;

  Serial.print("Level of soil moisture (in percent): ");
  Serial.print(percent);
  Serial.print('%');
  Serial.print('\n');

  // Read digital value of soil moisture
  int moistureDigital = digitalRead(digital_sig);

  // Turn on LED if detecting moisture
  if (moistureDigital == 0) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}
