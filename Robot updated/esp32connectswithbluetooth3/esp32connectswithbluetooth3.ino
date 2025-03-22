#define EN_PIN 5
#define PULL_PIN 21
#define DIR_PIN 18
#define PULL_PIN2 19
#define DIR_PIN2 5
// Default settings
int speed = 200;       // Default speed in steps per second
int stepsToMove = 0;   // Number of steps to move
bool isMoving = false; // Flag to track if motor is moving
unsigned long lastStepTime = 0;
unsigned long stepInterval = 1000000 / speed; // Microseconds between steps

// String to hold incoming serial data
String inputString = "";
bool stringComplete = false;

void setup() {
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
  // for(int i=0;i<10;i++){
  //   if(i%2==0){
  //     digitalWrite(PULL_PIN2, LOW);
  //     for(int j=0;j<1000;j++){
  //       forward();
  //     }
  //   }else{
  //     digitalWrite(PULL_PIN, LOW);
  //     for(int j=0;j<1000;j++){
  //       backward();
  //     }
  //   }
  // }
  forward();
}

