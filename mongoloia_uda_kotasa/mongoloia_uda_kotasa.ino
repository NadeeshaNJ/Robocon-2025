#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Servo.h>

const char* ssid = "Sahan";
const char* password = "HackMeIfYouCan";
WebServer server(80);

// Pin Definitions
#define IN1_LIFT 16    // Lift motor direction pin 1
#define IN2_LIFT 17    // Lift motor direction pin 2
static const int servoPin = 12;  // Feeder servo pin
static const int servoPin2 = 13;

// Control variables
Servo feederServo;
Servo rotateServo;
bool feederOn = false;
unsigned long lastTimeStamp = 0;
int counter = 0;
int degree = 0;

void handleShooterFeederData() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
    return;
  }

  String body = server.arg("plain");
  DynamicJsonDocument doc(256);
  DeserializationError error = deserializeJson(doc, body);

  if (error) {
    server.send(400, "text/plain", "Bad Request");
    return;
  }

  // Process shooter commands
  if (doc.containsKey("triangle") && doc["triangle"] == 1) {
    digitalWrite(IN1_LIFT, HIGH);
    digitalWrite(IN2_LIFT, LOW);
    Serial.println("Lift UP!");
  } 
  else if (doc.containsKey("cross") && doc["cross"] == 1) {
    digitalWrite(IN1_LIFT, LOW);
    digitalWrite(IN2_LIFT, HIGH);
    Serial.println("Lift DOWN!");
  }
  else {
    digitalWrite(IN1_LIFT, LOW);
    digitalWrite(IN2_LIFT, LOW);
  }

  // Process feeder commands
  if (doc.containsKey("circle") && doc["circle"] == 1 && feederOn) {
    for(int posDegrees = 100; posDegrees <= 240; posDegrees++) {
      feederServo.write(posDegrees);
      Serial.println(posDegrees);
      delay(20);
    }
    feederOn = !feederOn;
  }
  if (doc.containsKey("square") && doc["square"] == 1 && !feederOn) {
    for(int posDegrees = 240; posDegrees >= 100; posDegrees--) {
      feederServo.write(posDegrees);
      Serial.println(posDegrees);
      delay(20);
    }
    feederOn = !feederOn;
  }
  // Process feeder commands
  if (doc.containsKey("rotate_left") && doc["rotate_left"] == 1) {
    counter+=2;
    // if(counter%5){
    //   degree++;
    //   rotateServo.write(degree);
    //   Serial.println(degree);
    //   delay(20);
    // }
    rotateServo.write(counter);
    Serial.println(counter);
    delay(20);
    
    
  }
  if (doc.containsKey("rotate_right") && doc["rotate_right"] == 1) {
    counter-=2;
    // if(counter%10){
    //   degree--;
    //   rotateServo.write(degree);
    //   Serial.println(degree);
    //   delay(20);
    // }
    rotateServo.write(counter);
    Serial.println(counter);
    delay(20);
  }

  server.send(200, "application/json", "{\"status\":\"ok\"}");
}

void setup() {
  // Initialize motor control pins
  pinMode(IN1_LIFT, OUTPUT);
  pinMode(IN2_LIFT, OUTPUT);
  digitalWrite(IN1_LIFT, LOW);
  digitalWrite(IN2_LIFT, LOW);

  // Initialize servo
  feederServo.attach(servoPin);
  feederServo.write(135); // Initial position
  delay(2000);

  rotateServo.attach(servoPin2);
  rotateServo.write(0); // Initial position
  delay(2000);

  // Start serial communication
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);

  // Set maximum Wi-Fi TX power
  bool success = WiFi.setTxPower(WIFI_POWER_19_5dBm);
  if (success) {
    Serial.println("WiFi TX power set to maximum (19.5dBm).");
  } else {
    Serial.println("Failed to set WiFi TX power.");
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Setup server routes
  server.on("/shooter_feeder", HTTP_POST, handleShooterFeederData);
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/plain", "Combined Shooter/Feeder Controller Ready");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
