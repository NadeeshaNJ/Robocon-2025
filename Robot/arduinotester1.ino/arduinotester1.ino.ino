#define EN_PIN 2
#define PULL_PIN 6
#define DIR_PIN 5
#define PULL_PIN2 8
#define DIR_PIN2 7

void setup() {
  // Set pin modes
  pinMode(EN_PIN, OUTPUT);
  pinMode(PULL_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(PULL_PIN2, OUTPUT);
  pinMode(DIR_PIN2, OUTPUT);

  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("Arduino ready. Waiting for commands...");
}

void forward() {
  digitalWrite(PULL_PIN, HIGH);
  digitalWrite(DIR_PIN, HIGH);
  digitalWrite(EN_PIN, HIGH);
  delayMicroseconds(100000);
  digitalWrite(EN_PIN, LOW);
  digitalWrite(PULL_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);
  delayMicroseconds(1000);
}

void backward() {
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
  if (Serial.available() > 0) {
    char command = Serial.read();

    if (command == 'R') {
      // Rotate clockwise
      Serial.println("Received command: Rotate clockwise (R)");
      for (int i = 0; i < 100; i++) {
        forward();
      }
    } else if (command == 'L') {
      // Rotate anti-clockwise
      Serial.println("Received command: Rotate anti-clockwise (L)");
      for (int i = 0; i < 100; i++) {
        backward();
      }
    } else {
      Serial.println("Received unknown command");
    }
  }
}