// Motor control pins
#define IN1_R 25    // Front right motor direction pin 1
#define IN2_R 33    // Front right motor direction pin 2
#define ENA_R 32    // Front right motor PWM speed control pin
#define IN3_R 12    // Rear right motor direction pin 1
#define IN4_R 13    // Rear right motor direction pin 2
#define ENB_R 14    // Rear right motor PWM speed control pin

#define IN1_L 26    // Front left motor direction pin 1
#define IN2_L 27    // Front left motor direction pin 2
#define ENA_L 15    // Front left motor PWM speed control pin
#define IN3_L 16    // Rear left motor direction pin 1
#define IN4_L 17    // Rear left motor direction pin 2
#define ENB_L 18    // Rear left motor PWM speed control pin

#define EN 35

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
  pinMode(EN, OUTPUT);

  digitalWrite(EN, HIGH);

  // Initialize all motors in stopped state
  stopAllMotors();

  Serial.begin(115200);
  Serial.println("Ready for serial input. Send 'X Y' values (e.g., '100 -50')");
}

void loop() {
  if(Serial.available()){
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    int spaceIndex = input.indexOf(' ');
    
    if(spaceIndex > 0) {
      int x = input.substring(0, spaceIndex).toInt();
      int y = input.substring(spaceIndex + 1).toInt();
      
      Serial.print("X: ");
      Serial.print(x);
      Serial.print(" Y: ");
      Serial.println(y);
      
      // Calculate motor speeds for differential drive
      // Left motors (front and rear same speed)
      int leftSpeed = y + x;
      // Right motors (front and rear same speed)
      int rightSpeed = y - x;
      
      // Constrain speeds
      leftSpeed = constrain(leftSpeed, -255, 255);
      rightSpeed = constrain(rightSpeed, -255, 255);
      
      // Control left motors (front and rear)
      if(leftSpeed >= 0) {
        digitalWrite(IN1_L, HIGH);
        digitalWrite(IN2_L, LOW);
        digitalWrite(IN3_L, HIGH);
        digitalWrite(IN4_L, LOW);
      } else {
        digitalWrite(IN1_L, LOW);
        digitalWrite(IN2_L, HIGH);
        digitalWrite(IN3_L, LOW);
        digitalWrite(IN4_L, HIGH);
        leftSpeed = -leftSpeed;
      }
      analogWrite(ENA_L, leftSpeed);
      analogWrite(ENB_L, leftSpeed);
      
      // Control right motors (front and rear)
      if(rightSpeed >= 0) {
        digitalWrite(IN1_R, HIGH);
        digitalWrite(IN2_R, LOW);
        digitalWrite(IN3_R, HIGH);
        digitalWrite(IN4_R, LOW);
      } else {
        digitalWrite(IN1_R, LOW);
        digitalWrite(IN2_R, HIGH);
        digitalWrite(IN3_R, LOW);
        digitalWrite(IN4_R, HIGH);
        rightSpeed = -rightSpeed;
      }
      analogWrite(ENA_R, rightSpeed);
      analogWrite(ENB_R, rightSpeed);
    }
  }
}

void stopAllMotors() {
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
}