/*
   Board pin | NodeMCU GPIO |  Arduino IDE
      A-           1             5 or D1
      A+           3             0 or D3
      B-           2             4 or D2
      B+           4             2 or D4
*/

const int pwmMotorA = D1;
// const int pwmMotorB = D2;
const int dirMotorA = D3;
// const int dirMotorB = D4;

int motorSpeed = 0;

void setup() {
  Serial.begin(115200);
  Serial.println();

  pinMode(pwmMotorA , OUTPUT);
  // pinMode(pwmMotorB, OUTPUT);
  pinMode(dirMotorA, OUTPUT);
  // pinMode(dirMotorB, OUTPUT);

  Serial.println("Motor SHield 12E Initialized");
  delay(5000);
}

void loop() {
  // Kiểm tra xem có dữ liệu từ Serial không
  if (Serial.available() > 0) {
    char command = Serial.read(); // Đọc lệnh từ Serial

    if (command == 'u') { // Nếu phím 'u' được nhấn
      if (motorSpeed < 255) { // Giới hạn tốc độ tối đa
        motorSpeed += 50; // Tăng tốc độ
        Serial.print("Increasing speed: ");
        Serial.println(motorSpeed);
      } else {
        motorSpeed = 255;
      }
    } else if (command == 'd') { // Nếu phím 'd' được nhấn
      if (motorSpeed > 0) { // Giới hạn tốc độ tối thiểu
        motorSpeed -= 50; // Giảm tốc độ
        Serial.print("Decreasing speed: ");
        Serial.println(motorSpeed);
      } else {
        motorSpeed = 0;
      }
    }
  }

  // Điều khiển động cơ
  analogWrite(pwmMotorA, motorSpeed);
  digitalWrite(dirMotorA, LOW); // Đổi thành HIGH đảo chiều động cơ
  delay(100); // Điều chỉnh tốc độ cập nhật

  //
  // Serial.println("Activate A");
  // digitalWrite(pwmMotorA, motorSpeed);
  // digitalWrite(dirMotorA, LOW);
  // delay(1500);

  // Serial.println("Reverse A");
  // digitalWrite(dirMotorA, HIGH);
  // delay(1500);

  // Serial.println("Stop A");
  // digitalWrite(pwmMotorA, 0);
  // digitalWrite(dirMotorA, LOW);
  // delay(3000);


}