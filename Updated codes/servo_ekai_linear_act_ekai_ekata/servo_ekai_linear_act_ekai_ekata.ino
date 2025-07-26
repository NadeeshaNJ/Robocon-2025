#include <PS4Controller.h>
#include <Servo.h>

// Dead zone for joystick to prevent drift
#define DEAD_ZONE 0
// Maximum PWM value
#define MAX_PWM 255
#define IN1_LIFT 16    // Front left motor direction pin 1
#define IN2_LIFT 17    // Front left motor direction pin 2
static const int servoPin = 12;

Servo servo1;


unsigned long lastTimeStamp = 0;
bool on=false;

void notify() {
  if (millis() - lastTimeStamp > 10) { // Update every 10ms for smoother control
    int r = PS4.Circle();
    int s = PS4.Square();
    int p = PS4.Triangle();
    int q = PS4.Cross();

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

    if(r>0 && on){
      for(int posDegrees = 135; posDegrees <= 180; posDegrees++) {
          servo1.write(posDegrees);
          Serial.println(posDegrees);
          delay(20);
        }
        on = !on;
    }
    if(s>0 && !on){
      for(int posDegrees = 180; posDegrees >= 135; posDegrees--) {
          servo1.write(posDegrees);
          Serial.println(posDegrees);
          delay(20);
        }
      on = !on;
    }

    lastTimeStamp = millis();
  }
}

void onConnect() {
  Serial.println("Connected!");
}

void onDisconnect() {
  // Stop all motors when disconnected
  digitalWrite(IN1_LIFT, LOW);
  digitalWrite(IN2_LIFT, LOW);
  Serial.println("Disconnected!");
}

void setup() {
  pinMode(IN1_LIFT, OUTPUT);
  pinMode(IN2_LIFT, OUTPUT);

  digitalWrite(IN1_LIFT, LOW);
  digitalWrite(IN2_LIFT, LOW);
  Serial.begin(115200);
  servo1.attach(servoPin);
  servo1.write(135);
  delay(2000);
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