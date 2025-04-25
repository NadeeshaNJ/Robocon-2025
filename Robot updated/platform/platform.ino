#include <PS4Controller.h>

// Moving Platform motor control pins
#define PLATFORM_ENABLE_PIN 27     // EN - Platform motor enable
#define PLATFORM_DIRECTION_PIN 26  // DIR - Platform motor direction
#define PLATFORM_PULL_PIN 33       // PULL - Platform motor step pulse
#define PLATFORM_PULL_N_PIN 25     // PULL_ - Platform complementary step pulse

// Linear Actuator motor control pins
#define ACTUATOR_ENABLE_PIN 14     // EN - Actuator motor enable
#define ACTUATOR_DIRECTION_PIN 12  // DIR - Actuator motor direction
#define ACTUATOR_PULL_PIN 13        // PULL - Actuator motor step pulse
#define ACTUATOR_PULL_N_PIN 15     // PULL_ - Actuator complementary step pulse

// Motor control parameters
const int PLATFORM_STEP_DELAY_US = 700; // Microseconds between steps for platform
const int ACTUATOR_STEP_DELAY_US = 700; // Microseconds between steps for actuator
const int DEFAULT_STEPS = 25;          // Default number of steps per command

unsigned long lastTimeStamp = 0;

// Move motor function for both platform and actuator
void moveMotor(bool direction, int steps, int enablePin, int directionPin, int pullPin, int pullNPin, int stepDelayUs) {
  Serial.print("Setting direction for pin ");
  Serial.print(directionPin);
  Serial.print(" to ");
  Serial.println(direction ? "HIGH (Forward)" : "LOW (Reverse)");
  
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
  // Platform controls (Left/Right on D-pad)
  int platformLeft = PS4.Left();
  int platformRight = PS4.Right();
  
  // Actuator controls (Up/Down on D-pad)
  int actuatorUp = PS4.Up();
  int actuatorDown = PS4.Down();

  if (millis() - lastTimeStamp > 10) { // Increased debounce to 10ms
    // Platform movement
    if (platformLeft == 1) {
      Serial.println("Platform Left pressed");
      moveMotor(true, DEFAULT_STEPS, PLATFORM_ENABLE_PIN, PLATFORM_DIRECTION_PIN, 
                PLATFORM_PULL_PIN, PLATFORM_PULL_N_PIN, PLATFORM_STEP_DELAY_US);
    } else if (platformRight == 1) {
      Serial.println("Platform Right pressed");
      moveMotor(false, DEFAULT_STEPS, PLATFORM_ENABLE_PIN, PLATFORM_DIRECTION_PIN, 
                PLATFORM_PULL_PIN, PLATFORM_PULL_N_PIN, PLATFORM_STEP_DELAY_US);
    }

    // Actuator movement
    if (actuatorUp == 1) {
      Serial.println("Actuator Up pressed (Forward)");
      moveMotor(true, DEFAULT_STEPS, ACTUATOR_ENABLE_PIN, ACTUATOR_DIRECTION_PIN, 
                ACTUATOR_PULL_PIN, ACTUATOR_PULL_N_PIN, ACTUATOR_STEP_DELAY_US);
    } else if (actuatorDown == 1) {
      Serial.println("Actuator Down pressed (Reverse)");
      moveMotor(false, DEFAULT_STEPS, ACTUATOR_ENABLE_PIN, ACTUATOR_DIRECTION_PIN, 
                ACTUATOR_PULL_PIN, ACTUATOR_PULL_N_PIN, ACTUATOR_STEP_DELAY_US);
    }

    lastTimeStamp = millis();
  }
}

void onConnect() {
  Serial.println("Connected!");
}

void onDisConnect() {
  Serial.println("Disconnected!");    
}

void setup() {
  // Initialize platform motor control pins
  pinMode(PLATFORM_ENABLE_PIN, OUTPUT);
  pinMode(PLATFORM_DIRECTION_PIN, OUTPUT);
  pinMode(PLATFORM_PULL_PIN, OUTPUT);
  pinMode(PLATFORM_PULL_N_PIN, OUTPUT);

  // Initialize actuator motor control pins
  pinMode(ACTUATOR_ENABLE_PIN, OUTPUT);
  pinMode(ACTUATOR_DIRECTION_PIN, OUTPUT);
  pinMode(ACTUATOR_PULL_PIN, OUTPUT);
  pinMode(ACTUATOR_PULL_N_PIN, OUTPUT);

  // Initialize motors in stopped state
  digitalWrite(PLATFORM_ENABLE_PIN, HIGH); // Disable platform motor
  digitalWrite(PLATFORM_DIRECTION_PIN, LOW); // Default direction
  digitalWrite(ACTUATOR_ENABLE_PIN, HIGH); // Disable actuator motor
  digitalWrite(ACTUATOR_DIRECTION_PIN, LOW); // Default direction

  Serial.begin(115200);
  PS4.attach(notify);
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);
  PS4.begin();
  Serial.println("Ready.");
}

void loop() {
}