#include "BluetoothSerial.h"

BluetoothSerial SerialBT;  // Declare the Bluetooth object

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT_Device"); // Start Bluetooth with a device name
  Serial.println("Bluetooth Started! Pair with 'ESP32_BT_Device'.");
}

void loop() {
  // Forward data from Serial Monitor to Bluetooth
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  // Forward data from Bluetooth to Serial Monitor
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
}