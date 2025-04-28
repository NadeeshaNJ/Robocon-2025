#include <PS4Controller.h>

// Differential drive motor control pins (L298N Channel A)
#define IN1_R 14    // IN1 - Right motor direction pin 1
#define IN2_R 12    // IN2 - Right motor direction pin 2
#define ENA_R 32    // ENA - Right motor PWM speed control pin

// Platform motor control pins (L298N Channel B)
#define IN3_P 26    // IN3 - Platform motor direction pin 1
#define IN4_P 33    // IN4 - Platform motor direction pin 2
#define ENB_P 27    // ENB - Platform motor PWM speed control pin

// Motor control parameters
#define DEAD_ZONE 10       // Dead zone for joystick to prevent drift
#define MAX_PWM 255        // Maximum PWM value

unsigned long lastTimeStamp = 0;

void notify() {
  if (millis() - lastTimeStamp > 10) { // Update every 10ms for smoother control
    // Differential drive control (Joystick, L298N Channel A)
    int x = PS4.LStickX();
    int y = PS4.LStickY();

    // Apply dead zone
    if (abs(x) < DEAD_ZONE) x = 0;
    if (abs(y) < DEAD_ZONE) y = 0;

    // Calculate base speed (forward/backward) and turn (left/right)
    int baseSpeed = -y; // Invert y for intuitive control (up = forward)
    int turn = x;

    // Calculate right motor speed for differential drive
    int rightSpeed = baseSpeed - turn;

    // Constrain speeds to -255 to 255
    rightSpeed = constrain(rightSpeed, -MAX_PWM, MAX_PWM);

    // Control right motor (Channel A)
    if (rightSpeed >= 0) {
      digitalWrite(IN1_R, HIGH);
      digitalWrite(IN2_R, LOW);
    } else {
      digitalWrite(IN1_R, LOW);
      digitalWrite(IN2_R, HIGH);
      rightSpeed = -rightSpeed; // Make speed positive for PWM
    }
    analogWrite(ENA_R, rightSpeed);

    // Platform control (D-pad Left/Right, L298N Channel B)
    int platformSpeed = 0;
    if (PS4.Left()) {
      platformSpeed = -MAX_PWM; // Full speed backward
    } else if (PS4.Right()) {
      platformSpeed = MAX_PWM;  // Full speed forward
    }

    // Control platform gear motor (Channel B)
    if (platformSpeed >= 0) {
      digitalWrite(IN3_P, HIGH);
      digitalWrite(IN4_P, LOW);
    } else {
      digitalWrite(IN3_P, LOW);
      digitalWrite(IN4_P, HIGH);
      platformSpeed = -platformSpeed; // Make speed positive for PWM
    }
    analogWrite(ENB_P, platformSpeed);

    lastTimeStamp = millis();
  }
}

void onConnect() {
  Serial.println("Connected!");
}

void onDisconnect() {
  // Stop differential drive motor (Channel A)
  digitalWrite(IN1_R, LOW);
  digitalWrite(IN2_R, LOW);
  analogWrite(ENA_R, 0);
  
  // Stop platform motor (Channel B)
  digitalWrite(IN3_P, LOW);
  digitalWrite(IN4_P, LOW);
  analogWrite(ENB_P, 0);
  
  Serial.println("Disconnected!");
}

void setup() {
  // Initialize differential drive motor control pins (L298N Channel A)
  pinMode(IN1_R, OUTPUT);
  pinMode(IN2_R, OUTPUT);
  pinMode(ENA_R, OUTPUT);

  // Initialize platform motor control pins (L298N Channel B)
  pinMode(IN3_P, OUTPUT);
  pinMode(IN4_P, OUTPUT);
  pinMode(ENB_P, OUTPUT);

  // Initialize all motors in stopped state
  digitalWrite(IN1_R, LOW);
  digitalWrite(IN2_R, LOW);
  analogWrite(ENA_R, 0);
  
  digitalWrite(IN3_P, LOW);
  digitalWrite(IN4_P, LOW);
  analogWrite(ENB_P, 0);

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