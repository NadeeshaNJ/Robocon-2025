#include <PS4Controller.h>

// Define motor control pins
// Right side motors (front and rear)
#define LEFT_F 33   // Front right motor direction pin 1
#define LEFT_R 32    // Front right motor direction pin 2#define ENA_R 34    // Front right motor PWM speed control pin

// Left side motors (front and rear)
#define RIGHT_F 23    // Front left motor direction pin 1
#define RIGHT_R 22    // Front left motor direction pin 2

#define IN1_LIFT 19    // Front left motor direction pin 1
#define IN2_LIFT 21    // Front left motor direction pin 2

// Dead zone for joystick to prevent drift
#define DEAD_ZONE 0
// Maximum PWM value
#define MAX_PWM 255

unsigned long lastTimeStamp = 0;

void notify() {
  if (millis() - lastTimeStamp > 10) { // Update every 10ms for smoother control
    // Read joystick values (-128 to 127)
    int x = PS4.Up();
    int y = PS4.Down();
    int a = PS4.Left();
    int b = PS4.Right();

    int p = PS4.Triangle();
    int q = PS4.Cross();

    int leftSpeed = 0;
    int rightSpeed = 0;

    if (x>0){
      leftSpeed = 255;
      rightSpeed = 255;
      Serial.println("F!");
    }else if (y>0){
      leftSpeed = -255;
      rightSpeed = -255;
      Serial.println("B!");
    }else if (a>0){
      leftSpeed = -255;
      rightSpeed = 255;
      Serial.println("L!");
    }else if(b>0){
      leftSpeed = 255;
      rightSpeed = -255;
      Serial.println("R!");
    }
    else{
      leftSpeed = 0;
      rightSpeed = 0;
    }

    if(p>0){
      digitalWrite(IN1_LIFT, HIGH);
      digitalWrite(IN2_LIFT, LOW);
      Serial.println("UP!");
    }else if(q>0){
      digitalWrite(IN1_LIFT, LOW);
      digitalWrite(IN2_LIFT, HIGH);
      Serial.println("DOWN!");
    } else{
      digitalWrite(IN1_LIFT, LOW);
      digitalWrite(IN2_LIFT, LOW);
    }

    // Constrain speeds to -255 to 255
    leftSpeed = constrain(leftSpeed, -MAX_PWM, MAX_PWM);
    rightSpeed = constrain(rightSpeed, -MAX_PWM, MAX_PWM);

    if(leftSpeed>0){
      analogWrite(LEFT_F,leftSpeed);
      analogWrite(LEFT_R,0);
    }else if(leftSpeed<0){
      analogWrite(LEFT_F,0);
      analogWrite(LEFT_R,-leftSpeed);
    }else{
      analogWrite(LEFT_F,0);
      analogWrite(LEFT_R,0);
    }

    if(rightSpeed>0){
      analogWrite(RIGHT_F,rightSpeed);
      analogWrite(RIGHT_R,0);
    }else if(rightSpeed<0){
      analogWrite(RIGHT_F,0);
      analogWrite(RIGHT_R,-rightSpeed);
    }else{
      analogWrite(RIGHT_F,0);
      analogWrite(RIGHT_R,0);
    }
    

    lastTimeStamp = millis();
  }
}

void onConnect() {
  Serial.println("Connected!");
}

void onDisconnect() {
  // Stop all motors when disconnected
  digitalWrite(LEFT_F, LOW);
  digitalWrite(LEFT_R, LOW);
  digitalWrite(RIGHT_F, LOW);
  digitalWrite(RIGHT_R, LOW);
  digitalWrite(IN1_LIFT, LOW);
  digitalWrite(IN2_LIFT, LOW);
  Serial.println("Disconnected!");
}

void setup() {
  // Set all motor control pins as outputs
  pinMode(LEFT_F, OUTPUT);
  pinMode(LEFT_R, OUTPUT);
  pinMode(RIGHT_F, OUTPUT);
  pinMode(RIGHT_R, OUTPUT);
  pinMode(IN1_LIFT, OUTPUT);
  pinMode(IN2_LIFT, OUTPUT);

  // Initialize all motors in stopped state
  digitalWrite(LEFT_F, LOW);
  digitalWrite(LEFT_R, LOW);
  digitalWrite(RIGHT_F, LOW);
  digitalWrite(RIGHT_R, LOW);
  digitalWrite(IN1_LIFT, LOW);
  digitalWrite(IN2_LIFT, LOW);
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