// Pin definitions for motor driver (adjust based on your motor driver and wiring)
#define MOTOR1_PWM  13  // Front-left motor PWM
#define MOTOR1_DIR1 12  // Front-left motor direction 1
#define MOTOR1_DIR2 14  // Front-left motor direction 2
#define MOTOR2_PWM  27  // Front-right motor PWM
#define MOTOR2_DIR1 26  // Front-right motor direction 1
#define MOTOR2_DIR2 25  // Front-right motor direction 2
#define MOTOR3_PWM  19  // Rear-left motor PWM
#define MOTOR3_DIR1 18  // Rear-left motor direction 1
#define MOTOR3_DIR2 5   // Rear-left motor direction 2
#define MOTOR4_PWM  23  // Rear-right motor PWM
#define MOTOR4_DIR1 22  // Rear-right motor direction 1
#define MOTOR4_DIR2 21  // Rear-right motor direction 2

// PWM channel setup
#define PWM_CHANNEL1 0
#define PWM_CHANNEL2 1
#define PWM_CHANNEL3 2
#define PWM_CHANNEL4 3
#define PWM_FREQ 5000  // PWM frequency in Hz
#define PWM_RES 8      // PWM resolution (8-bit: 0-255)

// Speed variable (0-255 for PWM)
int motorSpeed = 150;  // Default speed (adjustable)

// Setup function
void setup() {
  // Initialize Serial for communication
  Serial.begin(115200);
  Serial.println("Mecanum Robot Control: F=Forward, B=Backward, L=Left, R=Right, S=Stop");

  // Set motor pins as outputs
  pinMode(MOTOR1_DIR1, OUTPUT);
  pinMode(MOTOR1_DIR2, OUTPUT);
  pinMode(MOTOR2_DIR1, OUTPUT);
  pinMode(MOTOR2_DIR2, OUTPUT);
  pinMode(MOTOR3_DIR1, OUTPUT);
  pinMode(MOTOR3_DIR2, OUTPUT);
  pinMode(MOTOR4_DIR1, OUTPUT);
  pinMode(MOTOR4_DIR2, OUTPUT);

  // Setup PWM channels
  ledcSetup(PWM_CHANNEL1, PWM_FREQ, PWM_RES);
  ledcSetup(PWM_CHANNEL2, PWM_FREQ, PWM_RES);
  ledcSetup(PWM_CHANNEL3, PWM_FREQ, PWM_RES);
  ledcSetup(PWM_CHANNEL4, PWM_FREQ, PWM_RES);

  // Attach PWM pins to channels
  ledcAttachPin(MOTOR1_PWM, PWM_CHANNEL1);
  ledcAttachPin(MOTOR2_PWM, PWM_CHANNEL2);
  ledcAttachPin(MOTOR3_PWM, PWM_CHANNEL3);
  ledcAttachPin(MOTOR4_PWM, PWM_CHANNEL4);

  // Stop all motors initially
  stop();
}

// Function to set motor speed and direction
void setMotor(int pwmPin, int dir1Pin, int dir2Pin, int pwmChannel, int speed, bool forward) {
  digitalWrite(dir1Pin, forward ? HIGH : LOW);
  digitalWrite(dir2Pin, forward ? LOW : HIGH);
  ledcWrite(pwmChannel, speed);
}

// Move forward: All wheels rotate forward
void moveForward() {
  setMotor(MOTOR1_PWM, MOTOR1_DIR1, MOTOR1_DIR2, PWM_CHANNEL1, motorSpeed, true);  // Front-left
  setMotor(MOTOR2_PWM, MOTOR2_DIR1, MOTOR2_DIR2, PWM_CHANNEL2, motorSpeed, true);  // Front-right
  setMotor(MOTOR3_PWM, MOTOR3_DIR1, MOTOR3_DIR2, PWM_CHANNEL3, motorSpeed, true);  // Rear-left
  setMotor(MOTOR4_PWM, MOTOR4_DIR1, MOTOR4_DIR2, PWM_CHANNEL4, motorSpeed, true);  // Rear-right
}

// Move backward: All wheels rotate backward
void moveBackward() {
  setMotor(MOTOR1_PWM, MOTOR1_DIR1, MOTOR1_DIR2, PWM_CHANNEL1, motorSpeed, false);
  setMotor(MOTOR2_PWM, MOTOR2_DIR1, MOTOR2_DIR2, PWM_CHANNEL2, motorSpeed, false);
  setMotor(MOTOR3_PWM, MOTOR3_DIR1, MOTOR3_DIR2, PWM_CHANNEL3, motorSpeed, false);
  setMotor(MOTOR4_PWM, MOTOR4_DIR1, MOTOR4_DIR2, PWM_CHANNEL4, motorSpeed, false);
}

// Move left: Front-left backward, front-right forward, rear-left forward, rear-right backward
void moveLeft() {
  setMotor(MOTOR1_PWM, MOTOR1_DIR1, MOTOR1_DIR2, PWM_CHANNEL1, motorSpeed, false);
  setMotor(MOTOR2_PWM, MOTOR2_DIR1, MOTOR2_DIR2, PWM_CHANNEL2, motorSpeed, true);
  setMotor(MOTOR3_PWM, MOTOR3_DIR1, MOTOR3_DIR2, PWM_CHANNEL3, motorSpeed, true);
  setMotor(MOTOR4_PWM, MOTOR4_DIR1, MOTOR4_DIR2, PWM_CHANNEL4, motorSpeed, false);
}

// Move right: Front-left forward, front-right backward, rear-left backward, rear-right forward
void moveRight() {
  setMotor(MOTOR1_PWM, MOTOR1_DIR1, MOTOR1_DIR2, PWM_CHANNEL1, motorSpeed, true);
  setMotor(MOTOR2_PWM, MOTOR2_DIR1, MOTOR2_DIR2, PWM_CHANNEL2, motorSpeed, false);
  setMotor(MOTOR3_PWM, MOTOR3_DIR1, MOTOR3_DIR2, PWM_CHANNEL3, motorSpeed, false);
  setMotor(MOTOR4_PWM, MOTOR4_DIR1, MOTOR4_DIR2, PWM_CHANNEL4, motorSpeed, true);
}

// Stop all motors
void stop() {
  ledcWrite(PWM_CHANNEL1, 0);
  ledcWrite(PWM_CHANNEL2, 0);
  ledcWrite(PWM_CHANNEL3, 0);
  ledcWrite(PWM_CHANNEL4, 0);
  digitalWrite(MOTOR1_DIR1, LOW);
  digitalWrite(MOTOR1_DIR2, LOW);
  digitalWrite(MOTOR2_DIR1, LOW);
  digitalWrite(MOTOR2_DIR2, LOW);
  digitalWrite(MOTOR3_DIR1, LOW);
  digitalWrite(MOTOR3_DIR2, LOW);
  digitalWrite(MOTOR4_DIR1, LOW);
  digitalWrite(MOTOR4_DIR2, LOW);
}

// Main loop to read Serial commands
void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    switch (command) {
      case 'F':
      case 'f':
        moveForward();
        Serial.println("Moving Forward");
        break;
      case 'B':
      case 'b':
        moveBackward();
        Serial.println("Moving Backward");
        break;
      case 'L':
      case 'l':
        moveLeft();
        Serial.println("Moving Left");
        break;
      case 'R':
      case 'r':
        moveRight();
        Serial.println("Moving Right");
        break;
      case 'S':
      case 's':
        stop();
        Serial.println("Stopped");
        break;
      default:
        Serial.println("Invalid command. Use F, B, L, R, S");
        break;
    }
  }
}