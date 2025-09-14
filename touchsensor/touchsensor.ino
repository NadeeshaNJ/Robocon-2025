void setup() {
  pinMode(2, INPUT);    // Set pin 2 as input for touch sensor signal
  pinMode(13, OUTPUT);  // Set pin 13 as output for LED
}

void loop() {
  digitalWrite(13, digitalRead(2));  // LED state mirrors sensor state
}