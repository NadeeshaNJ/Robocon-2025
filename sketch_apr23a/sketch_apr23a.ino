#include <BluetoothSerial.h>
#include <PS4Controller.h>

// Motor control pins
#define ENABLE_PIN 27     // EN - Motor enable
#define DIRECTION_PIN 26  // DIR - Motor direction
#define PULL_PIN 33       // PULL - Motor step pulse
#define PULL_N_PIN 25     // PULL_ - Complementary step pulse (if needed)

// Motor control parameters
const int STEP_DELAY_US = 700; // Microseconds between steps
const int DEFAULT_STEPS = 25;   // Default number of steps per command

// Bluetooth variables
#define BT_DISCOVER_TIME 10000
esp_spp_sec_t sec_mask = ESP_SPP_SEC_NONE;
esp_spp_role_t role = ESP_SPP_ROLE_SLAVE;

BluetoothSerial SerialBT;
String sendData = "Hi from esp32!\n";

// PS4 controller variables
unsigned long lastTimeStamp = 0;
int left = 0;
int right = 0;

void moveMotor(bool direction, int steps = DEFAULT_STEPS) {
  digitalWrite(DIRECTION_PIN, direction ? HIGH : LOW);
  digitalWrite(ENABLE_PIN, LOW); // Enable motor

  for(int i = 0; i < steps; i++) {
    digitalWrite(PULL_PIN, HIGH);
    digitalWrite(PULL_N_PIN, LOW);
    delayMicroseconds(STEP_DELAY_US);
    
    digitalWrite(PULL_PIN, LOW);
    digitalWrite(PULL_N_PIN, HIGH);
    delayMicroseconds(STEP_DELAY_US);
  }
  
  digitalWrite(ENABLE_PIN, HIGH); // Disable motor
}

// PS4 Controller callbacks
void notify() {
  left = PS4.Left();
  right = PS4.Right();

  if (millis() - lastTimeStamp > 1) {
    if(left == 1) {
      moveMotor(true);
      SerialBT.println("Moving motor forward");
    } else if(right == 1) {
      moveMotor(false);
      SerialBT.println("Moving motor backward");
    }
    lastTimeStamp = millis();
  }
}

void onConnect() {
  Serial.println("PS4 Controller Connected!");
  SerialBT.println("PS4 Controller Connected!");
}

void onDisConnect() {
  Serial.println("PS4 Controller Disconnected!");
  SerialBT.println("PS4 Controller Disconnected!");
}

void setupBluetooth() {
  if (!SerialBT.begin("ESP32_PS4_Motor", true)) {
    Serial.println("Bluetooth initialization failed!");
    abort();
  }

  Serial.println("Starting Bluetooth discoverAsync...");
  BTScanResults *btDeviceList = SerialBT.getScanResults();
  
  if (SerialBT.discoverAsync([](BTAdvertisedDevice *pDevice) {
        Serial.printf("Found device: %s\n", pDevice->toString().c_str());
      })) {
    delay(BT_DISCOVER_TIME);
    Serial.print("Stopping discoverAsync... ");
    SerialBT.discoverAsyncStop();
    Serial.println("discoverAsync stopped");
    delay(5000);
    
    if (btDeviceList->getCount() > 0) {
      Serial.println("Found devices:");
      for (int i = 0; i < btDeviceList->getCount(); i++) {
        BTAdvertisedDevice *device = btDeviceList->getDevice(i);
        Serial.printf("Device %d: %s  %s %d\n", i, 
                      device->getAddress().toString().c_str(), 
                      device->getName().c_str(), 
                      device->getRSSI());
      }
    } else {
      Serial.println("No Bluetooth devices found");
    }
  } else {
    Serial.println("Error on Bluetooth discoverAsync");
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize motor control pins
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(DIRECTION_PIN, OUTPUT);
  pinMode(PULL_PIN, OUTPUT);
  pinMode(PULL_N_PIN, OUTPUT);

  // Initialize motor in stopped state
  digitalWrite(ENABLE_PIN, HIGH);
  digitalWrite(DIRECTION_PIN, LOW);

  // Setup Bluetooth
  setupBluetooth();

  // Setup PS4 controller
  PS4.attach(notify);
  PS4.attachOnConnect(onConnect);
  PS4.attachOnDisconnect(onDisConnect);
  PS4.begin();
  Serial.println("Ready for PS4 controller connection");
  SerialBT.println("Ready for PS4 controller connection");
}

void loop() {
  // Handle Bluetooth serial communication
  if (!SerialBT.isClosed() && SerialBT.connected()) {
    if (SerialBT.write((const uint8_t *)sendData.c_str(), sendData.length()) != sendData.length()) {
      Serial.println("Bluetooth tx error");
    }
    
    if (SerialBT.available()) {
      Serial.print("Bluetooth rx: ");
      while (SerialBT.available()) {
        int c = SerialBT.read();
        if (c >= 0) {
          Serial.print((char)c);
        }
      }
      Serial.println();
    }
  } else {
    Serial.println("Bluetooth not connected");
  }

  delay(1000);
}