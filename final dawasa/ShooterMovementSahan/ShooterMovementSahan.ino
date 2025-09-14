#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

const char* ssid = "ShooterMovementSahan";
const char* password = "password1234";
WebServer server(80);

// Motor pins - EXACTLY as you specified
#define FL_FORWARD 33   // Front Left forward
#define FL_REVERSE 32   // Front Left reverse 
#define FR_FORWARD 23   // Front Right forward
#define FR_REVERSE 22   // Front Right reverse
#define RL_FORWARD 25   // Rear Left forward
#define RL_REVERSE 26   // Rear Left reverse        
#define RR_FORWARD 19   // Rear Right forward
#define RR_REVERSE 21   // Rear Right reverse

unsigned long lastCommandTime = 0;  // Track last command timestamp
const unsigned long commandTimeout = 100;  // 0.1 second timeout

void setup() {
  Serial.begin(115200);
  
  // Initialize ALL motor pins as outputs
  pinMode(FL_FORWARD, OUTPUT);
  pinMode(FL_REVERSE, OUTPUT);
  pinMode(FR_FORWARD, OUTPUT);
  pinMode(FR_REVERSE, OUTPUT);
  pinMode(RL_FORWARD, OUTPUT);
  pinMode(RL_REVERSE, OUTPUT);
  pinMode(RR_FORWARD, OUTPUT);
  pinMode(RR_REVERSE, OUTPUT);
  
  // Ensure all motors are stopped initially
  digitalWrite(FL_FORWARD, LOW);
  digitalWrite(FL_REVERSE, LOW);
  digitalWrite(FR_FORWARD, LOW);
  digitalWrite(FR_REVERSE, LOW);
  digitalWrite(RL_FORWARD, LOW);
  digitalWrite(RL_REVERSE, LOW);
  digitalWrite(RR_FORWARD, LOW);
  digitalWrite(RR_REVERSE, LOW);

  WiFi.disconnect(true); // Disconnects and clears saved WiFi credentials

  // Start WiFi as Access Point on channel 1
  WiFi.softAP(ssid, password, 5);  // 1 = WiFi channel

  // Set maximum Wi-Fi TX power
  bool success = WiFi.setTxPower(WIFI_POWER_19_5dBm);
  if (success) {
    Serial.println("WiFi TX power set to maximum (19.5dBm).");
  } else {
    Serial.println("Failed to set WiFi TX power.");
  }

  Serial.println("Access Point started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());


  // Setup endpoints
  server.on("/", HTTP_GET, []() {
    Serial.println("GET /");
    server.send(200, "text/plain", "4-Wheel Motor Controller Ready");
  });
  
  server.on("/movement", HTTP_POST, handleMovement);
  
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("Available endpoints:");
  Serial.println("GET /");
  Serial.println("POST /movement");

  lastCommandTime = millis();  // Initialize the timer
}

void loop() {
  server.handleClient();

    // Check if it's been too long since the last movement command
  if (millis() - lastCommandTime > commandTimeout) {
    stopAllMotors();
  }
}
void handleMovement() {
  Serial.println("\n=== New Movement Command ===");

  if (server.method() != HTTP_POST) {
    Serial.println("Error: Only POST allowed");
    server.send(405, "text/plain", "Method Not Allowed");
    return;
  }

  String body = server.arg("plain");
  Serial.printf("Raw body: %s\n", body.c_str());

  DynamicJsonDocument doc(256);
  DeserializationError error = deserializeJson(doc, body);

  if (error) {
    Serial.printf("JSON error: %s\n", error.c_str());
    server.send(400, "text/plain", "Bad Request: Invalid JSON");
    return;
  }

  int forward = doc["forward"] | 0;
  int backward = doc["backward"] | 0;
  int left = doc["left"] | 0;
  int right = doc["right"] | 0;

  Serial.printf("Command values - F:%d B:%d L:%d R:%d\n", forward, backward, left, right);

  int flSpeed = forward - backward - left + right;
  int frSpeed = forward - backward + left - right;
  int rlSpeed = forward - backward + left + right;
  int rrSpeed = forward - backward - left - right;

  flSpeed = constrain(flSpeed, -255, 255);
  frSpeed = constrain(frSpeed, -255, 255);
  rlSpeed = constrain(rlSpeed, -255, 255);
  rrSpeed = constrain(rrSpeed, -255, 255);

  Serial.printf("Motor speeds - FL:%d FR:%d RL:%d RR:%d\n", flSpeed, frSpeed, rlSpeed, rrSpeed);

  setMotorSpeed(FL_FORWARD, FL_REVERSE, flSpeed);
  setMotorSpeed(FR_FORWARD, FR_REVERSE, frSpeed);
  setMotorSpeed(RL_FORWARD, RL_REVERSE, rlSpeed);
  setMotorSpeed(RR_FORWARD, RR_REVERSE, rrSpeed);

  lastCommandTime = millis();  // ✅ Update the last command time

  server.send(200, "application/json", "{\"status\":\"ok\"}");
  Serial.println("=== Command Processed ===");
}

// Add this function to stop all motors
void stopAllMotors() {
  setMotorSpeed(FL_FORWARD, FL_REVERSE, 0);
  setMotorSpeed(FR_FORWARD, FR_REVERSE, 0);
  setMotorSpeed(RL_FORWARD, RL_REVERSE, 0);
  setMotorSpeed(RR_FORWARD, RR_REVERSE, 0);
}


void setMotorSpeed(int fwdPin, int revPin, int speed) {
  speed = constrain(speed, -255, 255);
  
  if (speed > 0) {
    analogWrite(fwdPin, speed);
    analogWrite(revPin, 0);
    Serial.printf("Pin %d: %d, Pin %d: 0\n", fwdPin, speed, revPin);
  } else if (speed < 0) {
    analogWrite(fwdPin, 0);
    analogWrite(revPin, -speed);
    Serial.printf("Pin %d: 0, Pin %d: %d\n", fwdPin, revPin, -speed);
  } else {
    analogWrite(fwdPin, 0);
    analogWrite(revPin, 0);
    Serial.printf("Pin %d: 0, Pin %d: 0 (STOP)\n", fwdPin, revPin);
  }
}

