/*
 * This example shows how to use WebSerial variant to send logging data to the browser.
 *
 * Before using this example, make sure to look at the WebSerial example before and its description.\
 *
 * You might want to control these flags to control the async library performance:
 *  -D CONFIG_ASYNC_TCP_QUEUE_SIZE=64
 *  -D CONFIG_ASYNC_TCP_RUNNING_CORE=1
 *  -D WS_MAX_QUEUED_MESSAGES=128
 */
#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#elif defined(ESP32)
#include <AsyncTCP.h>
#include <WiFi.h>
#endif
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <WString.h>
#include <MycilaWebSerial.h>

AsyncWebServer server(80);
WebSerial webSerial;

static uint32_t last = millis();
static uint32_t count = 0;

const char* ssid = "Dialog NNJ";
const char* password = "Mixtures";

void setup() {
  Serial.begin(115200);

  // Connect to WiFi in Station Mode
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("")

  Serial.println("\nConnected! IP address: ");
  Serial.println(WiFi.localIP());

  webSerial.onMessage([](const std::string& msg) { Serial.println(msg.c_str()); });
  webSerial.begin(&server);
  webSerial.setBuffer(100);

  server.onNotFound([](AsyncWebServerRequest* request) { request->redirect("/webserial"); });
  server.begin();
////////////////////////////////////
 

}

void loop() {
  if (millis() - last > 1000) {
    count++;

    webSerial.print(F("IP address: "));
    webSerial.println(WiFi.softAPIP());
    webSerial.printf("Uptime: %lums\n", millis());
    webSerial.printf("Free heap: %" PRIu32 "\n", ESP.getFreeHeap());

    last = millis();
  }
}
