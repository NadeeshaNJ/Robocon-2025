
#include <PS4Controller.h>

// Motor control pins
#define ENABLE_PIN 27     // EN - Motor enable
#define DIRECTION_PIN 26  // DIR - Motor direction
#define PULL_PIN 33       // PULL - Motor step pulse
#define PULL_N_PIN 25     // PULL_ - Complementary step pulse (if needed)

// Motor control parameters
const int STEP_DELAY_US = 700; // Microseconds between steps
const int DEFAULT_STEPS = 25;   // Default number of steps per command

unsigned long lastTimeStamp = 0;
int left = 0;
int right = 0;

void moveMotor(bool direction, int steps = DEFAULT_STEPS) {
  digitalWrite(DIRECTION_PIN, direction ? HIGH : LOW);
  digitalWrite(ENABLE_PIN, LOW); // Enable motor

  for(int i = 0; i < steps; i++) {
    digitalWrite(PULL_PIN, HIGH);
    digitalWrite(PULL_N_PIN, LOW);
    delayMicroseconds(STEP_DELAY_US);
    
    digitalWrite(PULL_PIN, LOW);
    digitalWrite(PULL_N_PIN, HIGH);
    delayMicroseconds(STEP_DELAY_US);
  }
  
  digitalWrite(ENABLE_PIN, HIGH); // Disable motor
}

void notify()
{
  left = PS4.Left();
  right = PS4.Right();

  if (millis() - lastTimeStamp > 1)
  {
    if(left == 1) {
      moveMotor(true);
      // Serial.println("Left");
    }else if(right==1){
      moveMotor(false);
      // Serial.println("Right");
    }else{

    }
    lastTimeStamp = millis();
  }
}

void onConnect()
{
  Serial.println("Connected!.");
}

void onDisConnect()
{
  Serial.println("Disconnected!.");    
}

void setup() 
{

  // Initialize motor control pins
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(DIRECTION_PIN, OUTPUT);
  pinMode(PULL_PIN, OUTPUT);
  pinMode(PULL_N_PIN, OUTPUT);

  // Initialize motor in stopped state
  digitalWrite(ENABLE_PIN, HIGH); // Disable motor (active low for many drivers)
  digitalWrite(DIRECTION_PIN, LOW); // Default direction

  Serial.begin(115200);
  PS4.attach(notify);
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);
  PS4.begin();
  Serial.println("Ready.");
}

void loop()
{

}