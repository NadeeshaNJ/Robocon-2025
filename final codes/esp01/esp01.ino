#include <Arduino.h>

// Define the LED pin for ESP-01 (connected to GPIO2)
#define LED_PIN 2

// Motor control pins (example - adjust to your setup)
#define MOTOR1_PIN1 12
#define MOTOR1_PIN2 14
#define MOTOR2_PIN1 27
#define MOTOR2_PIN2 26
#define MOTOR3_PIN1 25
#define MOTOR3_PIN2 33
#define MOTOR4_PIN1 32
#define MOTOR4_PIN2 35

void setup() {
  Serial.begin(115200);
  
  // Initialize motor control pins
  pinMode(MOTOR1_PIN1, OUTPUT);
  pinMode(MOTOR1_PIN2, OUTPUT);
  pinMode(MOTOR2_PIN1, OUTPUT);
  pinMode(MOTOR2_PIN2, OUTPUT);
  pinMode(MOTOR3_PIN1, OUTPUT);
  pinMode(MOTOR3_PIN2, OUTPUT);
  pinMode(MOTOR4_PIN1, OUTPUT);
  pinMode(MOTOR4_PIN2, OUTPUT);
  
  // Initialize LED pin (GPIO2 on ESP-01)
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Serial.println("ESP-01 Ready");
}

void controlMotors(int m1, int m2, int m3, int m4) {
  // Control 4 motors based on speed values (-255 to 255)
  // This is a simplified example - adjust for your motor driver
  
  // Motor 1
  digitalWrite(MOTOR1_PIN1, m1 > 0 ? HIGH : LOW);
  digitalWrite(MOTOR1_PIN2, m1 < 0 ? HIGH : LOW);
  
  // Motor 2
  digitalWrite(MOTOR2_PIN1, m2 > 0 ? HIGH : LOW);
  digitalWrite(MOTOR2_PIN2, m2 < 0 ? HIGH : LOW);
  
  // Motor 3
  digitalWrite(MOTOR3_PIN1, m3 > 0 ? HIGH : LOW);
  digitalWrite(MOTOR3_PIN2, m3 < 0 ? HIGH : LOW);
  
  // Motor 4
  digitalWrite(MOTOR4_PIN1, m4 > 0 ? HIGH : LOW);
  digitalWrite(MOTOR4_PIN2, m4 < 0 ? HIGH : LOW);
  
  // Toggle LED to show command received
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
}

void processCommand(String cmd) {
  if (cmd.startsWith("MOVE:")) {
    // Parse motor values (e.g., "MOVE:255 255 255 255")
    String values = cmd.substring(5);
    int space1 = values.indexOf(' ');
    int space2 = values.indexOf(' ', space1 + 1);
    int space3 = values.indexOf(' ', space2 + 1);
    
    int m1 = values.substring(0, space1).toInt();
    int m2 = values.substring(space1 + 1, space2).toInt();
    int m3 = values.substring(space2 + 1, space3).toInt();
    int m4 = values.substring(space3 + 1).toInt();
    
    controlMotors(m1, m2, m3, m4);
    Serial.print("Moving: ");
    Serial.println(values);
  }
  else if (cmd.startsWith("LIFT:")) {
    String direction = cmd.substring(5);
    if (direction == "UP") {
      // Implement lift up
      Serial.println("Lifting up");
    }
    else if (direction == "DOWN") {
      // Implement lift down
      Serial.println("Lifting down");
    }
  }
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    processCommand(command);
  }
}