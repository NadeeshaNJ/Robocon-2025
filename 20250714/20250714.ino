#include <PS4Controller.h>

// Define motor control pins
// Right side motors (front and rear)
#define IN1_R 33   // Front right motor direction pin 1
#define IN2_R 32    // Front right motor direction pin 2
#define ENA_R 34    // Front right motor PWM speed control pin
#define IN3_R 12    // Rear right motor direction pin 1
#define IN4_R 13    // Rear right motor direction pin 2
#define ENB_R 14    // Rear right motor PWM speed control pin

// Left side motors (front and rear)
#define IN1_L 22    // Front left motor direction pin 1
#define IN2_L 23    // Front left motor direction pin 2
#define ENA_L 15    // Front left motor PWM speed control pin
#define IN3_L 16    // Rear left motor direction pin 1
#define IN4_L 17    // Rear left motor direction pin 2
#define ENB_L 18    // Rear left motor PWM speed control pin

#define IN1_LIFT 19    // Front left motor direction pin 1
#define IN2_LIFT 21    // Front left motor direction pin 2

#define EN 35

// Dead zone for joystick to prevent drift
#define DEAD_ZONE 0
// Maximum PWM value
#define MAX_PWM 255

unsigned long lastTimeStamp = 0;

void notify() {
  if (millis() - lastTimeStamp > 10) { // Update every 10ms for smoother control
    // Read button states
    bool x = PS4.Up();
    bool y = PS4.Down();
    bool a = PS4.Left();
    bool b = PS4.Right();
    bool p = PS4.Triangle();
    bool q = PS4.Cross();
    bool s = PS4.Square();

    int leftSpeed = 0;
    int rightSpeed = 0;
    bool liftUp = false;
    bool liftDown = false;

    // Movement controls
    if(x){
      leftSpeed = 255;
      rightSpeed = 255;
      Serial.println("F!");
    } else if(y){
      leftSpeed = -255;
      rightSpeed = -255;
      Serial.println("B!");
    } else if(a){
      leftSpeed = -255;
      rightSpeed = 255;
      Serial.println("L!");
    } else if(b){
      leftSpeed = 255;
      rightSpeed = -255;
      Serial.println("R!");
    }else{
      leftSpeed = 0;
      rightSpeed = 0;
    }
    
    // Lift controls
    if(p){
      liftUp = true;
      liftDown = false;
      Serial.println("UP!");
    } else if(q){
      liftUp = false;
      liftDown = true;
      Serial.println("DOWN!");
    } else {
      liftUp = false;
      liftDown = false;
    }

    if(s){
      liftUp = false;
      liftDown = false;
      leftSpeed = 0;
      rightSpeed = 0;
    }

    // Constrain speeds to -255 to 255
    leftSpeed = constrain(leftSpeed, -MAX_PWM, MAX_PWM);
    rightSpeed = constrain(rightSpeed, -MAX_PWM, MAX_PWM);

    // Control left motors (front and rear, same direction)
    if (leftSpeed > 0) {
      digitalWrite(IN1_L, HIGH);
      digitalWrite(IN2_L, LOW);
      digitalWrite(IN3_L, HIGH);
      digitalWrite(IN4_L, LOW);
      analogWrite(ENA_L, leftSpeed);
      analogWrite(ENB_L, leftSpeed);
    } else if (leftSpeed < 0) {
      digitalWrite(IN1_L, LOW);
      digitalWrite(IN2_L, HIGH);
      digitalWrite(IN3_L, LOW);
      digitalWrite(IN4_L, HIGH);
      analogWrite(ENA_L, -leftSpeed);
      analogWrite(ENB_L, -leftSpeed);
    } else {
      digitalWrite(IN1_L, LOW);
      digitalWrite(IN2_L, LOW);
      digitalWrite(IN3_L, LOW);
      digitalWrite(IN4_L, LOW);
      analogWrite(ENA_L, 0);
      analogWrite(ENB_L, 0);
    }

    // Control right motors (front and rear, same direction)
    if (rightSpeed > 0) {
      digitalWrite(IN1_R, HIGH);
      digitalWrite(IN2_R, LOW);
      digitalWrite(IN3_R, HIGH);
      digitalWrite(IN4_R, LOW);
      analogWrite(ENA_R, rightSpeed);
      analogWrite(ENB_R, rightSpeed);
    } else if (rightSpeed < 0) {
      digitalWrite(IN1_R, LOW);
      digitalWrite(IN2_R, HIGH);
      digitalWrite(IN3_R, LOW);
      digitalWrite(IN4_R, HIGH);
      analogWrite(ENA_R, -rightSpeed);
      analogWrite(ENB_R, -rightSpeed);
    } else {
      digitalWrite(IN1_R, LOW);
      digitalWrite(IN2_R, LOW);
      digitalWrite(IN3_R, LOW);
      digitalWrite(IN4_R, LOW);
      analogWrite(ENA_R, 0);
      analogWrite(ENB_R, 0);
    }

    // Control lift motor
    if (liftUp) {
      digitalWrite(IN1_LIFT, HIGH);
      digitalWrite(IN2_LIFT, LOW);
    } else if (liftDown) {
      digitalWrite(IN1_LIFT, LOW);
      digitalWrite(IN2_LIFT, HIGH);
    } else {
      digitalWrite(IN1_LIFT, LOW);
      digitalWrite(IN2_LIFT, LOW);
    }

    lastTimeStamp = millis();
  }
}

void onConnect() {
  Serial.println("Connected!");
}

void onDisconnect() {
  // Stop all motors when disconnected
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
  digitalWrite(IN1_LIFT, LOW);
  digitalWrite(IN2_LIFT, LOW);
  Serial.println("Disconnected!");
}

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
  pinMode(IN1_LIFT, OUTPUT);
  pinMode(IN2_LIFT, OUTPUT);
  pinMode(EN, OUTPUT);

  // Initialize all motors in stopped state
  digitalWrite(IN1_R, LOW);
  digitalWrite(IN2_R, LOW);
  digitalWrite(IN3_R, LOW);
  digitalWrite(IN4_R, LOW);
  digitalWrite(IN1_L, LOW);
  digitalWrite(IN2_L, LOW);
  digitalWrite(IN3_L, LOW);
  digitalWrite(IN4_L, LOW);
  digitalWrite(EN, HIGH);
  analogWrite(ENA_R, 0);
  analogWrite(ENB_R, 0);
  analogWrite(ENA_L, 0);
  analogWrite(ENB_L, 0);

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