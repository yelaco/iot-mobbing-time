int PWMA = 5;  // Right side motor speed control (PWM)
int PWMB = 4;  // Left side motor speed control (PWM)
int DA = 0;    // Right side motor direction
int DB = 2;    // Left side motor direction

void setup() {
  Serial.begin(9600);
  Serial.println(HIGH);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(DA, OUTPUT);
  pinMode(DB, OUTPUT);

  // Initialize motors to stop
  digitalWrite(PWMA, LOW);
  digitalWrite(PWMB, LOW);
  digitalWrite(DA, LOW);
  digitalWrite(DB, LOW);

  Serial.println("Motors initialized.");
}

void loop() {
  forward();
  delay(1000); 
  stopMotors();
  delay(1000); 
  backward();
  delay(1000);
  stopMotors();
  delay(1000);
  slowForward();
  delay(1000);
  stopMotors();
  delay(1000); 
  round();
  delay(1000);  
  stopMotors();
  delay(1000);  
}

void forward() {
  Serial.println("Moving forward...");
  digitalWrite(DA, LOW); // Right motor forward
  digitalWrite(DB, LOW); // Left motor forward
  analogWrite(PWMA, 255); // Adjust the value (0-255) for speed control
  analogWrite(PWMB, 255); // Adjust the value (0-255) for speed control
}

void slowForward() {
  Serial.println("Moving slow forward...");
  digitalWrite(DA, LOW); // Right motor forward
  digitalWrite(DB, LOW); // Left motor forward
  analogWrite(PWMA, 200); // Adjust the value (0-255) for speed control
  analogWrite(PWMB, 200); // Adjust the value (0-255) for speed control
}

void stopMotors() {
  Serial.println("Stopping motors...");
  digitalWrite(DA, LOW);  // Right motor stop
  digitalWrite(DB, LOW);  // Left motor stop
  digitalWrite(PWMA, LOW); // Right motor speed off
  digitalWrite(PWMB, LOW); // Left motor speed off
}

void backward() {
  Serial.println("Moving backward...");
  digitalWrite(DA, HIGH); // Right motor backward
  digitalWrite(DB, HIGH); // Left motor backward
  analogWrite(PWMA, 255); // Adjust the value (0-255) for speed control
  analogWrite(PWMB, 255); // Adjust the value (0-255) for speed control
}

void round() {
  Serial.println("Moving backward...");
  digitalWrite(DA, HIGH); // Right motor backward
  digitalWrite(DB, LOW); // Left motor forward
  analogWrite(PWMA, 255); // Adjust the value (0-255) for speed control
  analogWrite(PWMB, 255); // Adjust the value (0-255) for speed control
}
