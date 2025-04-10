// Define motor control pins
// right mortor
#define IN1 8    // Direction pin 1
#define IN2 9    // Direction pin 2
#define ENA 10    // PWM speed control pin
// left mortor
#define IN3 7   // Direction pin 1
#define IN4 6    // Direction pin 2
#define ENB 5    // PWM speed control pin

#define temp 2

void setup() {
  // Set motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(temp, OUTPUT);

  // Initialize motor in a stopped state
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(temp, HIGH);
  analogWrite(ENA, 0);  // Start with 0 speed
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);  // Start with 0 speed
}

void loop() {
  // Example: Rotate motor forward with increasing speed
  for (int speed = 0; speed <= 255; speed += 5) {
    setMotorSpeed(speed, true);  // Forward direction
    delay(100);
  }

  // Stop motor briefly
  setMotorSpeed(0, true);
  delay(1000);

  // Example: Rotate motor backward with decreasing speed
  for (int speed = 255; speed >= 0; speed -= 5) {
    setMotorSpeed(speed, false);  // Backward direction
    delay(100);
  }

  // Stop motor
  setMotorSpeed(0, false);
  delay(1000);
}

// Function to set motor speed and direction
void setMotorSpeed(int speed, bool direction) {
  // Constrain speed to 0-255 (PWM range)
  speed = constrain(speed, 0, 255);

  // Set direction
  if (direction) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN4, HIGH);
    digitalWrite(IN3, LOW);
  }

  // Apply PWM speed
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}