#include <WiFi.h>
#include <ArduinoOTA.h>

const char* ssid = "Galaxy M01d7c3";
const char* password = "HackMeIfYouCan";

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Add WiFi status monitoring
  unsigned long wifiStartTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - wifiStartTime > 20000) {
      Serial.println("Failed to connect to WiFi! Rebooting...");
      delay(1000);
      ESP.restart();
    }
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // OTA Setup with increased timeout
  ArduinoOTA.setPassword("ben10");
  ArduinoOTA.setTimeout(600000); // Increased timeout to 60 seconds
  
  ArduinoOTA
    .onStart([]() {
      String type = (ArduinoOTA.getCommand() == U_FLASH) ? "sketch" : "filesystem";
      Serial.println("Start updating " + type);
      // Add any pre-update cleanup here if needed
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
      Serial.println("Restarting in 5 seconds...");
      delay(5000);
      ESP.restart();
    });

  ArduinoOTA.begin();
  Serial.println("OTA Ready");
}

void loop() {
  ArduinoOTA.handle();
}