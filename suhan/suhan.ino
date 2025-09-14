#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

const char* ssid = "Galaxy M01d7c3";
const char* password = "HackMeIfYouCan";
const char* websocket_server = "192.168.255.98";
const int websocket_port = 8765;

WebSocketsClient webSocket;
unsigned long lastPingTime = 0;
const unsigned long pingInterval = 10000; // Send ping every 10 seconds
bool isConnected = false;
unsigned long lastWifiCheckTime = 0;
const unsigned long wifiCheckInterval = 5000; // Check WiFi every 5 seconds

void reconnectWebSocket() {
  Serial.println("🔄 Reconnecting to WebSocket server...");
  webSocket.disconnect();
  delay(500);
  webSocket.begin(websocket_server, websocket_port, "/");
}

void reconnectWiFi() {
  Serial.println("🔄 Reconnecting to WiFi...");
  WiFi.disconnect();
  delay(1000);
  WiFi.begin(ssid, password);

  // Wait up to 20 seconds for WiFi connection
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 20000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n📶 WiFi reconnected!");
    reconnectWebSocket();
  } else {
    Serial.println("\n❌ WiFi reconnection failed!");
  }
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_CONNECTED:
      Serial.println("✅ Connected to WebSocket server");
      isConnected = true;

      // Identify this client as ESP32
      webSocket.sendTXT(R"({"type": "identify", "client": "esp32"})");
      break;

    case WStype_DISCONNECTED:
      Serial.println("❌ Disconnected from WebSocket server");
      isConnected = false;
      break;

    case WStype_TEXT: {
      // Log first 100 chars only to avoid flooding serial monitor
      char shortMessage[101];
      size_t msgLen = length < 100 ? length : 100;
      memcpy(shortMessage, payload, msgLen);
      shortMessage[msgLen] = '\0';
      Serial.printf("📨 Message from server: %s%s\n",
                   shortMessage, length > 100 ? "..." : "");
      break;
    }

    case WStype_PING:
      Serial.println("📌 Received ping");
      break;

    case WStype_PONG:
      Serial.println("📌 Received pong");
      break;

    case WStype_ERROR:
      Serial.println("❌ WebSocket error");
      break;

    default:
      Serial.printf("⚠️ Unknown WebSocket event type: %d\n", type);
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("🔌 Connecting to WiFi...");

  // Set WiFi mode and disconnect from any previous connection
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Configure WiFi connection for better stability
  WiFi.begin(ssid, password);

  // Wait for connection
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n📶 WiFi connected!");
    Serial.print("📱 IP address: ");
    Serial.println(WiFi.localIP());

    // Initialize WebSocket with more options for stability
    webSocket.begin(websocket_server, websocket_port, "/");
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);

    // Enable auto-reconnect
    webSocket.enableHeartbeat(15000, 3000, 2);
  } else {
    Serial.println("\n❌ WiFi connection failed!");
  }
}

void loop() {
  // Handle WebSocket communication
  webSocket.loop();

  // Send periodic ping to keep connection alive
  unsigned long currentMillis = millis();
  if (isConnected && currentMillis - lastPingTime > pingInterval) {
    webSocket.sendTXT(R"({"type": "ping"})");
    lastPingTime = currentMillis;
    Serial.println("📤 Sent ping to server");
  }

  // Check WiFi connection periodically
  if (currentMillis - lastWifiCheckTime > wifiCheckInterval) {
    lastWifiCheckTime = currentMillis;
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("❌ WiFi disconnected!");
      reconnectWiFi();
    }
  }
}