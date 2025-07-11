// Motor control pins (IN1, IN2 only)
const int motorFL[] = {21, 19};  // Front Left
const int motorFR[] = {33, 32};  // Front Right
const int motorRL[] = {23, 22};  // Rear Left 
const int motorRR[] = {26, 25};  // Rear Right

// Built-in LED pin
const int builtinLed = 2;

void setup() {
  Serial.begin(115200);
  
  // Initialize motor pins
  pinMode(motorFL[0], OUTPUT); pinMode(motorFL[1], OUTPUT);
  pinMode(motorFR[0], OUTPUT); pinMode(motorFR[1], OUTPUT);
  pinMode(motorRL[0], OUTPUT); pinMode(motorRL[1], OUTPUT);
  pinMode(motorRR[0], OUTPUT); pinMode(motorRR[1], OUTPUT);
  
  // Initialize LED
  pinMode(builtinLed, OUTPUT);
  digitalWrite(builtinLed, LOW);
  
  // LED test sequence
  for(int i=0; i<3; i++) {
    digitalWrite(builtinLed, HIGH);
    delay(200);
    digitalWrite(builtinLed, LOW);
    delay(200);
  }
  
  Serial.println("System Ready - Send MOVE commands");
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    Serial.print("Received: "); Serial.println(command);  // Debug input
    
    if (command.startsWith("MOVE:")) {
      processMovement(command.substring(5));
    } else {
      Serial.println("Invalid command");
      digitalWrite(builtinLed, HIGH);  // Error indication
      delay(100);
      digitalWrite(builtinLed, LOW);
    }
  }
}

void processMovement(String data) {
  // Debug raw data
  Serial.print("Raw data: "); Serial.println(data);
  digitalWrite(builtinLed, HIGH);  // Show processing
  
  int values[4] = {0, 0, 0, 0};  // Initialize with zeros
  int index = 0;
  
  // Find first space
  int spacePos = data.indexOf(' ');
  
  while (spacePos != -1 && index < 3) {
    values[index++] = data.substring(0, spacePos).toInt();
    data = data.substring(spacePos + 1);
    spacePos = data.indexOf(' ');
  }
  
  // Get last value
  if (index < 4) {
    values[index] = data.toInt();
  }

  // Debug parsed values
  Serial.print("Parsed: ");
  for (int i = 0; i < 4; i++) {
    Serial.print(values[i]); Serial.print(" ");
  }
  Serial.println();

  // Check if all motors are moving forward
  bool allForward = true;
  for (int i = 0; i < 4; i++) {
    if (values[i] <= 10) {
      allForward = false;
      break;
    }
  }

  // Control LED based on forward movement
  digitalWrite(builtinLed, allForward ? HIGH : LOW);
  
  // Control motors
  controlMotor(motorFL, values[0]);
  controlMotor(motorFR, values[1]);
  controlMotor(motorRL, values[2]);
  controlMotor(motorRR, values[3]);
  
  // Debug output
  Serial.print("Motors: ");
  Serial.print(values[0]); Serial.print(" ");
  Serial.print(values[1]); Serial.print(" ");
  Serial.print(values[2]); Serial.print(" ");
  Serial.print(values[3]);
  Serial.print(" | LED: "); Serial.println(allForward ? "ON" : "OFF");
  
  delay(10);  // Small delay for stability
  digitalWrite(builtinLed, LOW);  // Turn off processing indicator
}

void controlMotor(const int pins[2], int speed) {
  speed = constrain(speed, -255, 255);
  
  if (speed > 10) {  // Forward
    digitalWrite(pins[0], HIGH);
    digitalWrite(pins[1], LOW);
  } 
  else if (speed < -10) {  // Backward
    digitalWrite(pins[0], LOW);
    digitalWrite(pins[1], HIGH);
  } 
  else {  // Stop
    digitalWrite(pins[0], LOW);
    digitalWrite(pins[1], LOW);
  }
}
