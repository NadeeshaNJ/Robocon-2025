#include <PS4Controller.h>

// Define motor control pins
#define IN1_R 25
#define IN2_R 33
#define ENA_R 32
#define IN3_R 12
#define IN4_R 13
#define ENB_R 14

#define IN1_L 26
#define IN2_L 27
#define ENA_L 15
#define IN3_L 16
#define IN4_L 17
#define ENB_L 18

unsigned long lastTimeStamp = 0;

void setMotor(int in1, int in2, int pwmPin, int speed) {
  if (speed >= 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    speed = -speed;
  }
  analogWrite(pwmPin, constrain(speed, 0, 255));
}

void notify() {
  if (millis() - lastTimeStamp > 1) {
    int x = PS4.LStickX();   // Strafe
    int y = PS4.LStickY();   // Forward/Backward
    int r = PS4.RStickX();   // Rotation

    // Mix for X-Drive
    int frontLeft  = y + x + r;
    int frontRight = y - x - r;
    int rearLeft   = y - x + r;
    int rearRight  = y + x - r;

    // Constrain values
    frontLeft  = constrain(frontLeft, -255, 255);
    frontRight = constrain(frontRight, -255, 255);
    rearLeft   = constrain(rearLeft, -255, 255);
    rearRight  = constrain(rearRight, -255, 255);

    // Apply motor values
    setMotor(IN1_L, IN2_L, ENA_L, frontLeft);    // Front Left
    setMotor(IN3_L, IN4_L, ENB_L, rearLeft);     // Rear Left
    setMotor(IN1_R, IN2_R, ENA_R, frontRight);   // Front Right
    setMotor(IN3_R, IN4_R, ENB_R, rearRight);    // Rear Right

    lastTimeStamp = millis();
  }
}

void onConnect() {
  Serial.println("Connected!");
}

void onDisConnect() {
  // Stop all motors
  digitalWrite(IN1_R, LOW); digitalWrite(IN2_R, LOW);
  digitalWrite(IN3_R, LOW); digitalWrite(IN4_R, LOW);
  digitalWrite(IN1_L, LOW); digitalWrite(IN2_L, LOW);
  digitalWrite(IN3_L, LOW); digitalWrite(IN4_L, LOW);
  analogWrite(ENA_R, 0); analogWrite(ENB_R, 0);
  analogWrite(ENA_L, 0); analogWrite(ENB_L, 0);
  Serial.println("Disconnected!");
}

void setup() {
  pinMode(IN1_R, OUTPUT); pinMode(IN2_R, OUTPUT); pinMode(ENA_R, OUTPUT);
  pinMode(IN3_R, OUTPUT); pinMode(IN4_R, OUTPUT); pinMode(ENB_R, OUTPUT);
  pinMode(IN1_L, OUTPUT); pinMode(IN2_L, OUTPUT); pinMode(ENA_L, OUTPUT);
  pinMode(IN3_L, OUTPUT); pinMode(IN4_L, OUTPUT); pinMode(ENB_L, OUTPUT);
  Serial.begin(115200);
  
  PS4.attach(notify);
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);

  if (!PS4.begin()) {
    Serial.println("Failed to initialize Bluetooth!");
    while (1);
  }

  Serial.println("Bluetooth initialized. Waiting for PS4 controller...");
}

void loop() {
  // Everything is interrupt-based, nothing to do here.
}
