#define EN_PIN 2
#define PULL_PIN 6
#define DIR_PIN 5
#define PULL_PIN2 8
#define DIR_PIN2 7

// Default settings
int speed = 200;       // Default speed in steps per second
int stepsToMove = 0;   // Number of steps to move
bool isMoving = false; // Flag to track if motor is moving
unsigned long lastStepTime = 0;
unsigned long stepInterval = 1000000 / speed; // Microseconds between steps

// String to hold incoming serial data
String inputString = "";
bool stringComplete = false;

//Pairing
// void setup() {
//   pinMode(9, OUTPUT); // Set KEY/EN pin as output
//   digitalWrite(9, HIGH); // Set KEY/EN pin HIGH to enter AT mode
//   Serial.begin(38400); // HC-05 default baud rate for AT commands
//   Serial.println("Entering AT command mode...");
// }

// void loop() {
//   // Nothing to do here
// }

//After pairing

#include <SoftwareSerial.h>

// Define SoftwareSerial pins
SoftwareSerial BTSerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600); // Serial Monitor baud rate
  BTSerial.begin(9600); // Bluetooth module baud rate (default for HC-05/HC-06)
  Serial.println("Bluetooth communication started...");
// Set pin modes
  pinMode(EN_PIN, OUTPUT);
  pinMode(PULL_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(PULL_PIN2, OUTPUT);
  pinMode(DIR_PIN2, OUTPUT);
}
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

void loop() {
  // Check if data is available from the Bluetooth module
  if (BTSerial.available()) {
    char data = BTSerial.read(); // Read the incoming data
    Serial.print("Received: ");
    Serial.println(data); // Print the received data to the Serial Monitor

    if(data=='X'){
      digitalWrite(PULL_PIN2, LOW);
      for(int j=0;j<50;j++){
        forward();
      }      
    }else if(data=='T'){
      digitalWrite(PULL_PIN2, HIGH);
      for(int j=0;j<50;j++){
        backward();
      }  
    }

    // // Echo the received data back to the Bluetooth device
    // BTSerial.print("Echo: ");
    // BTSerial.println(data);
  }

  // // Check if data is available from the Serial Monitor
  // if (Serial.available()) {
  //   char data = Serial.read(); // Read the incoming data from the Serial Monitor
  //   BTSerial.print("Sent: ");
  //   BTSerial.println(data); // Send the data to the Bluetooth device
  // }
}

