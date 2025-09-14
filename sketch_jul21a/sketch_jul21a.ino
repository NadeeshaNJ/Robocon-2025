#include <PS4Controller.h>
#include <Servo.h>

// Dead zone for joystick to prevent drift
#define DEAD_ZONE 0
// Maximum PWM value
#define MAX_PWM 255

static const int servoPin = 12;

Servo servo1;


unsigned long lastTimeStamp = 0;
bool on=false;

void notify() {
  if (millis() - lastTimeStamp > 10) { // Update every 10ms for smoother control
    int r = PS4.Circle();
    int s = PS4.Square();

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
  Serial.println("Disconnected!");
}

void setup() {
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