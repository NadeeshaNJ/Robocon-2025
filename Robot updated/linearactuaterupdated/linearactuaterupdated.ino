#include <PS4Controller.h>

// L298N motor driver pins
#define IN1_PIN 26  // Input 1 for motor direction
#define IN2_PIN 27  // Input 2 for motor direction
#define ENA_PIN 33  // Enable pin for motor speed (PWM)

// Motor control parameters
const int MOTOR_SPEED = 200;  // PWM value (0-255) for motor speed
const int DEBOUNCE_MS = 50;   // Debounce interval in milliseconds

unsigned long lastTimeStamp = 0;

void controlMotor(bool counterclockwise) {
  if (counterclockwise) {
    // Counterclockwise: IN1 = HIGH, IN2 = LOW
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, LOW);
    analogWrite(ENA_PIN, MOTOR_SPEED);
    Serial.println("Motor rotating counterclockwise (Up pressed)");
  } else {
    // Clockwise: IN1 = LOW, IN2 = HIGH
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, HIGH);
    analogWrite(ENA_PIN, MOTOR_SPEED);
    Serial.println("Motor rotating clockwise (Down pressed)");
  }
}

void stopMotor() {
  // Stop motor: IN1 = LOW, IN2 = LOW, ENA = 0
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  analogWrite(ENA_PIN, 0);
  Serial.println("Motor stopped");
}

void notify() {
  int up = PS4.Up();
  int down = PS4.Down();

  if (millis() - lastTimeStamp > DEBOUNCE_MS) {
    // Prevent simultaneous up and down commands
    if (up == 1 && down == 1) {
      Serial.println("Error: Both Up and Down pressed, stopping motor");
      stopMotor();
    } else if (up == 1) {
      controlMotor(true);  // Counterclockwise
    } else if (down == 1) {
      controlMotor(false); // Clockwise
    } else {
      stopMotor(); // Stop motor when no buttons are pressed
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
  // Initialize L298N control pins
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(ENA_PIN, OUTPUT);

  // Initialize motor in stopped state
  stopMotor();

  Serial.begin(115200);
  PS4.attach(notify);
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);
  PS4.begin();
  Serial.println("Ready.");
}

void loop() {
}