
#include <WiFi.h>
#include <WebServer.h>

// Replace with your mobile hotspot credentials
const char* ssid = "Galaxy M01d7c3";     
const char* password = "HackMeIfYouCan"; 

WebServer server(80);

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Define server route
  server.on("/Forward", HTTP_GET, []() {
    Serial.println("Forward");
    server.send(200, "text/plain", "Done");
    // if (server.hasArg("state")) {
    //   String state = server.arg("state");
    //   Serial.println("Button State: " + state);
    //   server.send(200, "text/plain", "State received");
    // } else {
    //   server.send(400, "text/plain", "No state provided");
    // }
  });
  server.on("/Backward", HTTP_GET, []() {
    Serial.println("Backward");
    server.send(200, "text/plain", "Done");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
