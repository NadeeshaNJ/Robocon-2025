#include <PS4Controller.h>

// Define motor control pins
// Right side motors (front and rear)
#define IN1_R 25    // Front right motor direction pin 1
#define IN2_R 33    // Front right motor direction pin 2
#define ENA_R 32    // Front right motor PWM speed control pin
#define IN3_R 12    // Rear right motor direction pin 1
#define IN4_R 13    // Rear right motor direction pin 2
#define ENB_R 14    // Rear right motor PWM speed control pin

// Left side motors (front and rear)
#define IN1_L 26    // Front left motor direction pin 1
#define IN2_L 27    // Front left motor direction pin 2
#define ENA_L 15    // Front left motor PWM speed control pin
#define IN3_L 16    // Rear left motor direction pin 1
#define IN4_L 17    // Rear left motor direction pin 2
#define ENB_L 18    // Rear left motor PWM speed control pin

// Dead zone for joystick to prevent drift
#define DEAD_ZONE 10
// Maximum PWM value
#define MAX_PWM 255

unsigned long lastTimeStamp = 0;

void notify() {
  if (millis() - lastTimeStamp > 10) { // Update every 10ms for smoother control
    // Read joystick values (-128 to 127)
    int x = PS4.LStickX();
    int y = PS4.LStickY();

    // Apply dead zone
    if (abs(x) < DEAD_ZONE) x = 0;
    if (abs(y) < DEAD_ZONE) y = 0;

    // Calculate base speed (forward/backward) and turn (left/right)
    // y: Forward (positive) / Backward (negative)
    // x: Turn right (positive) / Turn left (negative)
    int baseSpeed = -y; // Invert y for intuitive control (up = forward)
    int turn = x;

    // Calculate left and right motor speeds for differential drive
    int leftSpeed = baseSpeed + turn;
    int rightSpeed = baseSpeed - turn;

    // Constrain speeds to -255 to 255
    leftSpeed = constrain(leftSpeed, -MAX_PWM, MAX_PWM);
    rightSpeed = constrain(rightSpeed, -MAX_PWM, MAX_PWM);

    // Control left motors (front and rear, same direction)
    if (leftSpeed >= 0) {
      digitalWrite(IN1_L, HIGH);
      digitalWrite(IN2_L, LOW);
      digitalWrite(IN3_L, HIGH);
      digitalWrite(IN4_L, LOW);
    } else {
      digitalWrite(IN1_L, LOW);
      digitalWrite(IN2_L, HIGH);
      digitalWrite(IN3_L, LOW);
      digitalWrite(IN4_L, HIGH);
      leftSpeed = -leftSpeed; // Make speed positive for PWM
    }
    analogWrite(ENA_L, leftSpeed);
    analogWrite(ENB_L, leftSpeed);

    // Control right motors (front and rear, same direction)
    if (rightSpeed >= 0) {
      digitalWrite(IN1_R, HIGH);
      digitalWrite(IN2_R, LOW);
      digitalWrite(IN3_R, HIGH);
      digitalWrite(IN4_R, LOW);
    } else {
      digitalWrite(IN1_R, LOW);
      digitalWrite(IN2_R, HIGH);
      digitalWrite(IN3_R, LOW);
      digitalWrite(IN4_R, HIGH);
      rightSpeed = -rightSpeed; // Make speed positive for PWM
    }
    analogWrite(ENA_R, rightSpeed);
    analogWrite(ENB_R, rightSpeed);

    lastTimeStamp = millis();
  }
}

void onConnect() {
  Serial.println("Connected!");
}

void onDisconnect() {
  // Stop all motors when disconnected
  digitalWrite(IN1_R, LOW);
  digitalWrite(IN2_R, LOW);
  digitalWrite(IN3_R, LOW);
  digitalWrite(IN4_R, LOW);
  digitalWrite(IN1_L, LOW);
  digitalWrite(IN2_L, LOW);
  digitalWrite(IN3_L, LOW);
  digitalWrite(IN4_L, LOW);
  analogWrite(ENA_R, 0);
  analogWrite(ENB_R, 0);
  analogWrite(ENA_L, 0);
  analogWrite(ENB_L, 0);
  Serial.println("Disconnected!");
}

void setup() {
  // Set all motor control pins as outputs
  pinMode(IN1_R, OUTPUT);
  pinMode(IN2_R, OUTPUT);
  pinMode(ENA_R, OUTPUT);
  pinMode(IN3_R, OUTPUT);
  pinMode(IN4_R, OUTPUT);
  pinMode(ENB_R, OUTPUT);
  pinMode(IN1_L, OUTPUT);
  pinMode(IN2_L, OUTPUT);
  pinMode(ENA_L, OUTPUT);
  pinMode(IN3_L, OUTPUT);
  pinMode(IN4_L, OUTPUT);
  pinMode(ENB_L, OUTPUT);

  // Initialize all motors in stopped state
  digitalWrite(IN1_R, LOW);
  digitalWrite(IN2_R, LOW);
  digitalWrite(IN3_R, LOW);
  digitalWrite(IN4_R, LOW);
  digitalWrite(IN1_L, LOW);
  digitalWrite(IN2_L, LOW);
  digitalWrite(IN3_L, LOW);
  digitalWrite(IN4_L, LOW);
  analogWrite(ENA_R, 0);
  analogWrite(ENB_R, 0);
  analogWrite(ENA_L, 0);
  analogWrite(ENB_L, 0);

  Serial.begin(115200);
  PS4.attach(notify);
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisconnect);

  if (!PS4.begin()) {
    Serial.println("Failed to initialize Bluetooth!");
    while (1); // Halt if initialization fails
  }

  Serial.println("Bluetooth initialized. Waiting for PS4 controller...");
}

void loop() {
  // Main loop is empty; control is handled by notifications
}