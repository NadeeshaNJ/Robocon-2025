
#define EN 2
#define DIR 3
#define PULL_ 4
#define PULL 5

void setup() {
  // put your setup code here, to run once:
  pinMode(EN, OUTPUT);
  pinMode(DIR, OUTPUT);
  pinMode(PULL_, OUTPUT);
  pinMode(PULL, OUTPUT);

  // digitalWrite(EN, HIGH);
  digitalWrite(DIR, HIGH);
  // digitalWrite(PULL_, LOW);
  // digitalWrite(PULL, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(DIR, HIGH);
  for(int i=0;i<400;i++){
    digitalWrite(EN, LOW);
    digitalWrite(PULL, LOW);
    digitalWrite(PULL_, HIGH);
    delayMicroseconds(700); 
    digitalWrite(EN, HIGH);
    digitalWrite(PULL, HIGH);
    digitalWrite(PULL_, LOW);
    delayMicroseconds(700); 
  }
  digitalWrite(DIR, LOW);
  for(int i=0;i<400;i++){
    digitalWrite(EN, LOW);
    digitalWrite(PULL, LOW);
    digitalWrite(PULL_, HIGH);
    delayMicroseconds(700); 
    digitalWrite(EN, HIGH);
    digitalWrite(PULL, HIGH);
    digitalWrite(PULL_, LOW);
    delayMicroseconds(700); 
  }
  
}
