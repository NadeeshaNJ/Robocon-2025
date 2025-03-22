
#include <PS4Controller.h>

#define EN_PIN 5
#define PULL_PIN 21
#define DIR_PIN 18
#define PULL_PIN2 19
#define DIR_PIN2 5
unsigned long lastTimeStamp = 0;

// Default settings
int speed = 200;       // Default speed in steps per second
int stepsToMove = 0;   // Number of steps to move
bool isMoving = false; // Flag to track if motor is moving
unsigned long lastStepTime = 0;
unsigned long stepInterval = 1000000 / speed; // Microseconds between steps

// String to hold incoming serial data
String inputString = "";
bool stringComplete = false;
void forward(){
  digitalWrite(PULL_PIN, HIGH);
  digitalWrite(DIR_PIN, HIGH);
  digitalWrite(EN_PIN, HIGH);
  delayMicroseconds(100000);
  digitalWrite(EN_PIN, LOW);
  digitalWrite(PULL_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);
  delayMicroseconds(1000);
}
void backward(){
  digitalWrite(PULL_PIN2, HIGH);
  digitalWrite(DIR_PIN, HIGH);
  digitalWrite(EN_PIN, HIGH);
  delayMicroseconds(100000);
  digitalWrite(EN_PIN, LOW);
  digitalWrite(PULL_PIN2, LOW);
  digitalWrite(DIR_PIN, LOW);
  delayMicroseconds(1000);
}


void notify()
{
  // char messageString[200];
  // sprintf(messageString, "%4d,%4d,%4d,%4d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d",
  // PS4.LStickX(),
  // PS4.LStickY(),
  // PS4.RStickX(),
  // PS4.RStickY(),
  // PS4.Left(),
  // PS4.Down(),
  // PS4.Right(),
  // PS4.Up(),
  // PS4.Square(),
  // PS4.Cross(),
  // PS4.Circle(),
  // PS4.Triangle(),
  // PS4.L1(),
  // PS4.R1(),
  // PS4.L2(),
  // PS4.R2(),  
  // PS4.Share(),
  // PS4.Options(),
  // PS4.PSButton(),
  // PS4.Touchpad(),
  // PS4.Charging(),
  // PS4.Audio(),
  // PS4.Mic(),
  // PS4.Battery());

  //Only needed to print the message properly on serial monitor. Else we dont need it.
  if (millis() - lastTimeStamp > 1)
  {
    // Serial.println(PS4.Cross());
    if(PS4.Left()==1){
      digitalWrite(PULL_PIN2, LOW);
      for(int i=0;i<20;i++){
        forward();
      }
      
    }
    if(PS4.Right()==1){
      digitalWrite(PULL_PIN2, HIGH);
      for(int i=0;i<20;i++){
        backward();
      }
      
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
  // Set pin modes
  pinMode(EN_PIN, OUTPUT);
  pinMode(PULL_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(PULL_PIN2, OUTPUT);
  pinMode(DIR_PIN2, OUTPUT);

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
