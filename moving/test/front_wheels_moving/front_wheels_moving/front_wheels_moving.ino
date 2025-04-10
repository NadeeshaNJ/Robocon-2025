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

  Serial.begin(9600);
}

void loop() {
  if(Serial.available()){
    // Read the incoming string until newline
    String input = Serial.readStringUntil('\n');
    input.trim();  // Remove any whitespace
    
    // Find the space separating the two values
    int spaceIndex = input.indexOf(' ');
    
    if(spaceIndex > 0) {
      // Extract x and y values
      int x = input.substring(0, spaceIndex).toInt();
      int y = input.substring(spaceIndex + 1).toInt();
      
      // Debug output
      Serial.print("X: ");
      Serial.print(x);
      Serial.print(" Y: ");
      Serial.println(y);
      
      // Calculate motor speeds based on x and y
      // For a differential drive system:
      // Left motor speed = y + x
      // Right motor speed = y - x
      int leftSpeed = y + x;
      int rightSpeed = y - x;
      
      // Constrain speeds to -255 to 255 range
      leftSpeed = constrain(leftSpeed, -255, 255);
      rightSpeed = constrain(rightSpeed, -255, 255);
      
      // Set left motor direction and speed
      if(leftSpeed >= 0) {
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
      } else {
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        leftSpeed = -leftSpeed;  // Make speed positive
      }
      analogWrite(ENB, leftSpeed);
      
      // Set right motor direction and speed
      if(rightSpeed >= 0) {
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
      } else {
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        rightSpeed = -rightSpeed;  // Make speed positive
      }
      analogWrite(ENA, rightSpeed);
    }
  }
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