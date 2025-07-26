#include <PS4Controller.h>

// Define motor control pins for 4 mecanum wheels
// Front Left (FL) motor
#define FL_FORWARD 33   // Forward pin for Front Left motor
#define FL_REVERSE 32   // Reverse pin for Front Left motor

// Front Right (FR) motor
#define FR_FORWARD 23   // Forward pin for Front Right motor
#define FR_REVERSE 22   // Reverse pin for Front Right motor

// Rear Left (RL) motor - need to define these pins (you didn't specify in your original code)
#define RL_FORWARD 25   // Example pin, adjust according to your setup
#define RL_REVERSE 26   // Example pin, adjust according to your setup

// Rear Right (RR) motor - need to define these pins (you didn't specify in your original code)
#define RR_FORWARD 19   // Example pin, adjust according to your setup
#define RR_REVERSE 21   // Example pin, adjust according to your setup

// Dead zone for joystick to prevent drift
#define DEAD_ZONE 10
// Maximum PWM value
#define MAX_PWM 255

unsigned long lastTimeStamp = 0;

void setMotorSpeed(int forwardPin, int reversePin, int speed) {
  speed = constrain(speed, -MAX_PWM, MAX_PWM);
  
  if (speed > 0) {
    analogWrite(forwardPin, speed);
    analogWrite(reversePin, 0);
  } else if (speed < 0) {
    analogWrite(forwardPin, 0);
    analogWrite(reversePin, -speed);
  } else {
    analogWrite(forwardPin, 0);
    analogWrite(reversePin, 0);
  }
}

void notify() {
  if (millis() - lastTimeStamp > 10) { // Update every 10ms for smoother control
    // Read joystick values
    int leftY = PS4.LStickY();  // Forward/Backward movement
    int leftX = PS4.LStickX();  // Strafe movement
    int rightX = PS4.RStickX(); // Rotation
    
    // Calculate mecanum wheel vectors
    // FL: forward + strafe + rotation
    int flSpeed = leftY - leftX - rightX;
    // FR: forward - strafe + rotation
    int frSpeed = leftY + leftX + rightX;
    // RL: forward + strafe - rotation
    int rlSpeed = leftY + leftX - rightX;
    // RR: forward - strafe - rotation
    int rrSpeed = leftY - leftX + rightX;

    // Apply dead zone
    if (abs(leftY) < DEAD_ZONE && abs(leftX) < DEAD_ZONE && abs(rightX) < DEAD_ZONE) {
      flSpeed = frSpeed = rlSpeed = rrSpeed = 0;
    }

    // Set motor speeds
    setMotorSpeed(FL_FORWARD, FL_REVERSE, flSpeed);
    setMotorSpeed(FR_FORWARD, FR_REVERSE, frSpeed);
    setMotorSpeed(RL_FORWARD, RL_REVERSE, rlSpeed);
    setMotorSpeed(RR_FORWARD, RR_REVERSE, rrSpeed);

    lastTimeStamp = millis();
  }
}

void onConnect() {
  Serial.println("Connected!");
}

void onDisconnect() {
  // Stop all motors when disconnected
  setMotorSpeed(FL_FORWARD, FL_REVERSE, 0);
  setMotorSpeed(FR_FORWARD, FR_REVERSE, 0);
  setMotorSpeed(RL_FORWARD, RL_REVERSE, 0);
  setMotorSpeed(RR_FORWARD, RR_REVERSE, 0);
  Serial.println("Disconnected!");
}

void setup() {
  // Set all motor control pins as outputs
  pinMode(FL_FORWARD, OUTPUT);
  pinMode(FL_REVERSE, OUTPUT);
  pinMode(FR_FORWARD, OUTPUT);
  pinMode(FR_REVERSE, OUTPUT);
  pinMode(RL_FORWARD, OUTPUT);
  pinMode(RL_REVERSE, OUTPUT);
  pinMode(RR_FORWARD, OUTPUT);
  pinMode(RR_REVERSE, OUTPUT);

  // Initialize all motors in stopped state
  digitalWrite(FL_FORWARD, LOW);
  digitalWrite(FL_REVERSE, LOW);
  digitalWrite(FR_FORWARD, LOW);
  digitalWrite(FR_REVERSE, LOW);
  digitalWrite(RL_FORWARD, LOW);
  digitalWrite(RL_REVERSE, LOW);
  digitalWrite(RR_FORWARD, LOW);
  digitalWrite(RR_REVERSE, LOW);

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