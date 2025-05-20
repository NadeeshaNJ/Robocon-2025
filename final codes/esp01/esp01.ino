
#include <WiFi.h>
#include <WebServer.h>

// Motor control pins
#define R_PWM_SUB 12  // Right motor PWM (front)
#define L_PWM_SUB 14  // Left motor PWM (front)
#define R_PWM_MAIN 27 // Right motor PWM (back)
#define L_PWM_MAIN 26 // Left motor PWM (back)
#define VCC 13        // Additional pin for 5V output

const char *ssid = "Galaxy M01d7c3";
const char *password = "HackMeIfYouCan";

int baseSpeed = 255;
WebServer server(80);

void forward()
{
  analogWrite(R_PWM_SUB, baseSpeed);
  analogWrite(L_PWM_SUB, 0);
  analogWrite(R_PWM_MAIN, baseSpeed);
  analogWrite(L_PWM_MAIN, 0);
}

void backward()
{
  analogWrite(R_PWM_SUB, 0);
  analogWrite(L_PWM_SUB, baseSpeed);
  analogWrite(R_PWM_MAIN, 0);
  analogWrite(L_PWM_MAIN, baseSpeed);
}

void left()
{
  analogWrite(R_PWM_SUB, 0);
  analogWrite(L_PWM_SUB, baseSpeed);
  analogWrite(R_PWM_MAIN, baseSpeed);
  analogWrite(L_PWM_MAIN, 0);
}

void right()
{
  analogWrite(R_PWM_SUB, baseSpeed);
  analogWrite(L_PWM_SUB, 0);
  analogWrite(R_PWM_MAIN, 0);
  analogWrite(L_PWM_MAIN, baseSpeed);
}

void stop()
{
  analogWrite(R_PWM_SUB, 0);
  analogWrite(L_PWM_SUB, 0);
  analogWrite(R_PWM_MAIN, 0);
  analogWrite(L_PWM_MAIN, 0);
}

void setup()
{
  Serial.begin(115200);

  // Set motor control pins as outputs
  pinMode(R_PWM_SUB, OUTPUT);
  pinMode(L_PWM_SUB, OUTPUT);
  pinMode(R_PWM_MAIN, OUTPUT);
  pinMode(L_PWM_MAIN, OUTPUT);
  pinMode(VCC, OUTPUT);
  digitalWrite(VCC, HIGH);

  // Initialize motors in stopped state
  stop();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println("IP Address: " + WiFi.localIP().toString());

  // Define server routes
  server.on("/state", HTTP_POST, []()
            {
        if (server.args() > 0) {
            if (server.hasArg("Forward") && server.arg("Forward") == "ON") {
                forward();
                Serial.println("Forward");
            } 
            else if (server.hasArg("Backward") && server.arg("Backward") == "ON") {
                backward();
                Serial.println("Backward");
            }
            else if (server.hasArg("Left") && server.arg("Left") == "ON") {
                left();
                Serial.println("Left");
            }
            else if (server.hasArg("Right") && server.arg("Right") == "ON") {
                right();
                Serial.println("Right");
            }
            else {
                stop();
                Serial.println("Stop");
            }
            
            server.send(200, "text/plain", "Movement states received");
        } else {
            Serial.println("No states received in POST request");
            server.send(400, "text/plain", "No states provided");
        } });

  // GET endpoint for connection check
  server.on("/state", HTTP_GET, []()
            { server.send(200, "text/plain", "Movement ESP32 is online"); });

  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
}
