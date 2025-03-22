
#include <PS4Controller.h>

void setup() {
  Serial.begin(115200);
  PS4.begin();
  Serial.println("Scanning for PS4 controller...");
}

void loop() {
  if (PS4.isConnected()) {
    Serial.printf("PS4 controller connected with MAC address: %s\n", PS4.getAddress().toString().c_str());
    while (1); // Stop after finding the MAC address
  }
}
