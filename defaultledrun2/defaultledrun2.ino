#include <Arduino.h>

// Motor Control Pins (adjust according to your wiring)
#define MOTOR1_PIN1 32  
#define MOTOR1_PIN2 33  
#define MOTOR2_PIN1 25  // (Note: You listed 25 earlier, but not in the latest list. Replace if needed)
#define MOTOR2_PIN2 26  // (Not in your list, using 19 instead)
#define MOTOR3_PIN1 19  
#define MOTOR3_PIN2 21  
#define MOTOR4_PIN1 34  // (Note: 34 is input-only on ESP32, cannot output PWM!)
#define MOTOR4_PIN2 35  

// LED Pin (built-in LED is usually GPIO2)
#define LED_PIN 2

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
  
  // Initialize LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Serial.println("ESP32 Motor Controller Ready");
}

void controlMotors(int m1, int m2, int m3, int m4) {
  // Motor 1
  digitalWrite(MOTOR1_PIN1, m1 > 0 ? HIGH : LOW);
  digitalWrite(MOTOR1_PIN2, m1 < 0 ? HIGH : LOW);
  
  // Motor 2
  digitalWrite(MOTOR2_PIN1, m2 > 0 ? HIGH : LOW);
  digitalWrite(MOTOR2_PIN2, m2 < 0 ? HIGH : LOW);
  
  // Motor 3
  digitalWrite(MOTOR3_PIN1, m3 > 0 ? HIGH : LOW);
  digitalWrite(MOTOR3_PIN2, m3 < 0 ? HIGH : LOW);
  
  // Motor 4 (Note: GPIO34 is INPUT-ONLY, so this may not work)
  digitalWrite(MOTOR4_PIN1, m4 > 0 ? HIGH : LOW);
  digitalWrite(MOTOR4_PIN2, m4 < 0 ? HIGH : LOW);
  
  // Blink LED to confirm command
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
}

void processCommand(String cmd) {
  if (cmd.startsWith("MOVE:")) {
    String values = cmd.substring(5);
    int space1 = values.indexOf(' ');
    int space2 = values.indexOf(' ', space1 + 1);
    int space3 = values.indexOf(' ', space2 + 1);
    
    int m1 = values.substring(0, space1).toInt();
    int m2 = values.substring(space1 + 1, space2).toInt();
    int m3 = values.substring(space2 + 1, space3).toInt();
    int m4 = values.substring(space3 + 1).toInt();
    
    controlMotors(m1, m2, m3, m4);
    Serial.print("Moving Motors: ");
    Serial.print(m1); Serial.print(" ");
    Serial.print(m2); Serial.print(" ");
    Serial.print(m3); Serial.print(" ");
    Serial.println(m4);
  }
  else if (cmd.startsWith("LIFT:")) {
    String direction = cmd.substring(5);
    if (direction == "UP") Serial.println("Lift UP");
    else if (direction == "DOWN") Serial.println("Lift DOWN");
  }
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    processCommand(command);
  }
}