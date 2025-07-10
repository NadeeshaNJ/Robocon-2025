#include <PS4Controller.h>

// Differential drive motor control pins (Right side motors)
#define IN1_R 14    // Front right motor direction pin 1
#define IN2_R 12    // Front right motor direction pin 2
#define ENA_R 32    // Front right motor PWM speed control pin

// Platform motor control pins
#define PLATFORM_ENABLE_PIN 27     // EN - Platform motor enable
#define PLATFORM_DIRECTION_PIN 26  // DIR - Platform motor direction
#define PLATFORM_PULL_PIN 33       // PULL - Platform motor step pulse
#define PLATFORM_PULL_N_PIN 25     // PULL_ - Platform complementary step pulse

// Motor control parameters
#define DEAD_ZONE 10               // Dead zone for joystick to prevent drift
#define MAX_PWM 255                // Maximum PWM value
const int PLATFORM_STEP_DELAY_US = 700; // Microseconds between steps for platform
const int DEFAULT_STEPS = 25;          // Default number of steps per command

unsigned long lastTimeStamp = 0;

// Move motor function for platform
void moveMotor(bool direction, int steps, int enablePin, int directionPin, int pullPin, int pullNPin, int stepDelayUs) {
  digitalWrite(directionPin, direction ? HIGH : LOW);
  digitalWrite(enablePin, LOW); // Enable motor

  for (int i = 0; i < steps; i++) {
    digitalWrite(pullPin, HIGH);
    digitalWrite(pullNPin, LOW);
    delayMicroseconds(stepDelayUs);
    
    digitalWrite(pullPin, LOW);
    digitalWrite(pullNPin, HIGH);
    delayMicroseconds(stepDelayUs);
  }
  
  digitalWrite(enablePin, HIGH); // Disable motor
}

void notify() {
  if (millis() - lastTimeStamp > 10) { // Update every 10ms for smoother control
    // Differential drive control (Joystick, replacing actuator control)
    int x = PS4.LStickX();
    int y = PS4.LStickY();

    // Apply dead zone
    if (abs(x) < DEAD_ZONE) x = 0;
    if (abs(y) < DEAD_ZONE) y = 0;

    // Calculate base speed (forward/backward) and turn (left/right)
    int baseSpeed = -y; // Invert y for intuitive control (up = forward)
    int turn = x;

    // Calculate right motor speed for differential drive
    int rightSpeed = baseSpeed - turn;

    // Constrain speeds to -255 to 255
    rightSpeed = constrain(rightSpeed, -MAX_PWM, MAX_PWM);

    // Control right motors
    if (rightSpeed >= 0) {
      digitalWrite(IN1_R, HIGH);
      digitalWrite(IN2_R, LOW);
    } else {
      digitalWrite(IN1_R, LOW);
      digitalWrite(IN2_R, HIGH);
      rightSpeed = -rightSpeed; // Make speed positive for PWM
    }
    analogWrite(ENA_R, rightSpeed);

    // Platform control (D-pad Left/Right)
    int platformLeft = PS4.Left();
    int platformRight = PS4.Right();

    if (platformLeft == 1) {
      moveMotor(true, DEFAULT_STEPS, PLATFORM_ENABLE_PIN, PLATFORM_DIRECTION_PIN, 
                PLATFORM_PULL_PIN, PLATFORM_PULL_N_PIN, PLATFORM_STEP_DELAY_US);
    } else if (platformRight == 1) {
      moveMotor(false, DEFAULT_STEPS, PLATFORM_ENABLE_PIN, PLATFORM_DIRECTION_PIN, 
                PLATFORM_PULL_PIN, PLATFORM_PULL_N_PIN, PLATFORM_STEP_DELAY_US);
    }

    lastTimeStamp = millis();
  }
}

void onConnect() {
  Serial.println("Connected!");
}

void onDisconnect() {
  // Stop differential drive motors
  digitalWrite(IN1_R, LOW);
  digitalWrite(IN2_R, LOW);
  analogWrite(ENA_R, 0);
  
  // Stop platform motor
  digitalWrite(PLATFORM_ENABLE_PIN, HIGH); // Disable platform motor
  
  Serial.println("Disconnected!");
}

void setup() {
  // Initialize differential drive motor control pins
  pinMode(IN1_R, OUTPUT);
  pinMode(IN2_R, OUTPUT);
  pinMode(ENA_R, OUTPUT);

  // Initialize platform motor control pins
  pinMode(PLATFORM_ENABLE_PIN, OUTPUT);
  pinMode(PLATFORM_DIRECTION_PIN, OUTPUT);
  pinMode(PLATFORM_PULL_PIN, OUTPUT);
  pinMode(PLATFORM_PULL_N_PIN, OUTPUT);

  // Initialize all motors in stopped state
  digitalWrite(IN1_R, LOW);
  digitalWrite(IN2_R, LOW);
  analogWrite(ENA_R, 0);
  
  digitalWrite(PLATFORM_ENABLE_PIN, HIGH); // Disable platform motor
  digitalWrite(PLATFORM_DIRECTION_PIN, LOW); // Default direction

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