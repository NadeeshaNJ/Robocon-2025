#include <WiFi.h>
#include <WebServer.h>
#include <Servo.h>

// Motor control pins (using same IBT driver configuration as movement ESP32)
#define ROTATE_R_PWM 25  // Right rotation (clockwise)
#define ROTATE_L_PWM 33  // Left rotation (counter-clockwise)
#define LIFT_R_PWM 32    // Lift up
#define LIFT_L_PWM 35    // Lift down
#define SHOOTER_R_PWM 34 // Shooter push (original pin - kept for compatibility)
#define SHOOTER_L_PWM 4  // Shooter pull (unused in your case)
#define VCC 13           // Additional pin for 5V output
#define SERVO_PIN 26     // ESP32 pin GPIO26 connected to servo motor

const char *ssid = "Galaxy M01d7c3";
const char *password = "HackMeIfYouCan";

int baseSpeed = 255;
WebServer server(80);
bool emergencyState = false;
Servo shooterServo;    // Create servo object for shooter mechanism
int servoPosition = 0; // Current position of the servo
int position = 0;
void setup()
{
    Serial.begin(115200);

    // Set motor control pins as outputs
    pinMode(ROTATE_R_PWM, OUTPUT);
    pinMode(ROTATE_L_PWM, OUTPUT);
    pinMode(LIFT_R_PWM, OUTPUT);
    pinMode(LIFT_L_PWM, OUTPUT);
    pinMode(SHOOTER_R_PWM, OUTPUT);
    pinMode(SHOOTER_L_PWM, OUTPUT);
    pinMode(VCC, OUTPUT);
    digitalWrite(VCC, HIGH);

    // Initialize all outputs to LOW
    analogWrite(ROTATE_R_PWM, 0);
    analogWrite(ROTATE_L_PWM, 0);
    analogWrite(LIFT_R_PWM, 0);
    analogWrite(LIFT_L_PWM, 0);
    analogWrite(SHOOTER_R_PWM, 0);
    analogWrite(SHOOTER_L_PWM, 0);

    // Initialize servo
    shooterServo.attach(SERVO_PIN);
    shooterServo.write(servoPosition); // Initialize to starting position

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
            // Emergency has highest priority
            if (server.hasArg("Emergency")) {
                emergencyState = (server.arg("Emergency") == "ON");
                Serial.println("Emergency: " + String(emergencyState ? "ON" : "OFF"));
                
                if (emergencyState) {
                    // Stop all motors immediately
                    analogWrite(ROTATE_R_PWM, 0);
                    analogWrite(ROTATE_L_PWM, 0);
                    analogWrite(LIFT_R_PWM, 0);
                    analogWrite(LIFT_L_PWM, 0);
                    analogWrite(SHOOTER_R_PWM, 0);
                    analogWrite(SHOOTER_L_PWM, 0);
                    shooterServo.write(0); // Return servo to home position during emergency
                    server.send(200, "text/plain", "Emergency state received");
                    return;
                }
            }

            // Process other controls only if not in emergency
            if (!emergencyState) {
                // Rotation control
                if (server.hasArg("RotateLeft") && server.arg("RotateLeft") == "ON" && server.hasArg("RotateRight") && server.arg("RotateRight") == "OFF") {
                    analogWrite(ROTATE_L_PWM, baseSpeed);
                    analogWrite(ROTATE_R_PWM, 0); // Ensure opposite direction is off
                    Serial.println("RotateLeft");
                }else if(server.hasArg("RotateLeft") && server.arg("RotateLeft") == "OFF" && server.hasArg("RotateRight") && server.arg("RotateRight") == "ON"){
                    analogWrite(ROTATE_R_PWM, baseSpeed);
                    analogWrite(ROTATE_L_PWM, 0); // Ensure opposite direction is off
                    Serial.println("RotateRight");
                }else{
                    analogWrite(ROTATE_R_PWM, 0);
                    analogWrite(ROTATE_L_PWM, 0);
                }

                // Lift control
                if (server.hasArg("LiftUp") && server.arg("LiftUp") == "ON" && server.hasArg("LiftDown") && server.arg("LiftDown") == "OFF") {
                    analogWrite(LIFT_R_PWM, baseSpeed);
                    analogWrite(LIFT_L_PWM, 0); // Ensure opposite direction is off
                    Serial.println("LiftUp");
                }else if(server.hasArg("LiftUp") && server.arg("LiftUp") == "OFF" && server.hasArg("LiftDown") && server.arg("LiftDown") == "ON") {
                    analogWrite(LIFT_L_PWM, baseSpeed);
                    analogWrite(LIFT_R_PWM, 0); // Ensure opposite direction is off
                    Serial.println("LiftDown");
                }else{
                    analogWrite(LIFT_L_PWM, 0);
                    analogWrite(LIFT_R_PWM, 0);
                }

                // Shooter control using servo
                // if (server.hasArg("ShooterPush")) {
                //     bool state = (server.arg("ShooterPush") == "ON");
                //     if (state) {
                //         // Activate shooter servo (move to shooting position)
                //         shooterServo.write(180);
                //         servoPosition = 180;
                //         Serial.println("ShooterPush: ON - Servo at 180°");
                //     } else {
                //         // Return shooter servo to home position
                //         shooterServo.write(0);
                //         servoPosition = 0;
                //         Serial.println("ShooterPush: OFF - Servo at 0°");
                //     }
                // }
                
                // Additional servo control options
                // if (server.hasArg("ServoPosition")) {
                //     int position = server.arg("ServoPosition").toInt();
                //     // Constrain position to valid range
                //     position = constrain(position, 0, 180);
                //     shooterServo.write(position);
                //     servoPosition = position;
                //     Serial.println("Servo Position set to: " + String(position));
                // }else{
                //   shooterServo.write(0);
                // }
            }
            
            server.send(200, "text/plain", "Control states received");
        } else {
            Serial.println("No states received in POST request");
            server.send(400, "text/plain", "No states provided");
        } });

    // GET endpoint for connection check and status
    server.on("/state", HTTP_GET, []()
              {
        String status = "Lifting ESP32 is online\n";
        status += "Servo Position: " + String(servoPosition) + " degrees";
        server.send(200, "text/plain", status); });

    // Add an endpoint specifically for servo control
    server.on("/servo", HTTP_POST, []()
              {
        if (server.hasArg("position")) {
            int pos = server.arg("position").toInt();
            pos = constrain(pos, 0, 240);
            if (pos==180){
              pos=240;
            }
            if (pos!=position){
              position = pos;
              shooterServo.write(position);
              delay(1000);
            }          
            server.send(200, "text/plain", "Servo position set to " + String(position));
            Serial.println("Servo position set to " + String(position));
        } else {
            server.send(400, "text/plain", "No position provided");
        } });

    server.begin();
    Serial.println("HTTP server started");
}

void loop()
{
    server.handleClient();
}
