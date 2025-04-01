// #include "BluetoothSerial.h"

// // #define VCC 11
// // #define GND 10
// // #define TXD 9
// // #define RXD 8
// #define EN 2
// #define DIR 3
// #define PULL_ 4
// #define PULL 5

// BluetoothSerial SerialBT;

// int temp = 0;
// int threshold = 50;

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(9600);
//   Serial.println("Enter AT Commands:");
//   SerialBT.begin("ESP32");
//   // pinMode(VCC,OUTPUT);
//   // pinMode(12,OUTPUT);
//   // pinMode(GND,OUTPUT);
//   // digitalWrite(12, HIGH);
//   // digitalWrite(VCC, HIGH);
//   // digitalWrite(GND, LOW);
//   pinMode(EN, OUTPUT);
//   pinMode(DIR, OUTPUT);
//   pinMode(PULL_, OUTPUT);
//   pinMode(PULL, OUTPUT);

//   // digitalWrite(EN, HIGH);
//   digitalWrite(DIR, HIGH);
// }

// void forward(){
//   digitalWrite(DIR, HIGH);
//   for(int i=0;i<threshold;i++){
//     digitalWrite(EN, LOW);
//     digitalWrite(PULL, LOW);
//     digitalWrite(PULL_, HIGH);
//     delayMicroseconds(700); 
//     digitalWrite(EN, HIGH);
//     digitalWrite(PULL, HIGH);
//     digitalWrite(PULL_, LOW);
//     delayMicroseconds(700); 
//   }
// }

// void backward(){
//   digitalWrite(DIR, LOW);
//   for(int i=0;i<threshold;i++){
//     digitalWrite(EN, LOW);
//     digitalWrite(PULL, LOW);
//     digitalWrite(PULL_, HIGH);
//     delayMicroseconds(700); 
//     digitalWrite(EN, HIGH);
//     digitalWrite(PULL, HIGH);
//     digitalWrite(PULL_, LOW);
//     delayMicroseconds(700); 
//   }
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   if(SerialBT.available()){
//     // Serial.write(h06.read());
//     temp = SerialBT.read();
//     Serial.println(temp);
//     if(temp=='1'){
//       forward();
//     }else if(temp=='2'){
//       backward();
//     }
//   }
//   // if(Serial.available()){
//   //   h06.write(Serial.read());
//   // }

// }

#include "BluetoothSerial.h"

// Motor control pins
#define ENABLE_PIN 27     // EN - Motor enable
#define DIRECTION_PIN 26  // DIR - Motor direction
#define PULL_PIN 33       // PULL - Motor step pulse
#define PULL_N_PIN 25     // PULL_ - Complementary step pulse (if needed)

// Motor control parameters
const int STEP_DELAY_US = 700; // Microseconds between steps
const int DEFAULT_STEPS = 50;   // Default number of steps per command

BluetoothSerial SerialBT;

void setup() {
  // Initialize serial for debugging
  Serial.begin(115200);
  
  // Initialize Bluetooth
  if(!SerialBT.begin("ESP32_Motor_Controller")) {
    Serial.println("Bluetooth initialization failed!");
    while(1); // Halt if Bluetooth fails
  }
  Serial.println("Bluetooth device ready, waiting for connections...");

  // Initialize motor control pins
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(DIRECTION_PIN, OUTPUT);
  pinMode(PULL_PIN, OUTPUT);
  pinMode(PULL_N_PIN, OUTPUT);

  // Initialize motor in stopped state
  digitalWrite(ENABLE_PIN, HIGH); // Disable motor (active low for many drivers)
  digitalWrite(DIRECTION_PIN, LOW); // Default direction
}

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

void loop() {
  if(SerialBT.available()) {
    char command = SerialBT.read();
    Serial.print("Received command: ");
    Serial.println(command);
    
    switch(command) {
      case '1': // Move forward
        moveMotor(true);
        break;
      case '2': // Move backward
        moveMotor(false);
        break;
      case 's': // Stop motor (emergency stop)
        digitalWrite(ENABLE_PIN, HIGH);
        break;
      default:
        Serial.print("Unknown command: ");
        Serial.println(command);
    }
  }
}