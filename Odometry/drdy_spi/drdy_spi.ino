#include <Wire.h>
#include <MPU9250_asukiaaa.h>

MPU9250_asukiaaa mySensor;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginGyro();
  //mySensor.beginMag();

  Serial.println("MPU9250 Raw Data (No Calibration)");
}

void loop() {
  mySensor.accelUpdate();
  mySensor.gyroUpdate();
  mySensor.magUpdate();

  Serial.print("Accel X/Y/Z: ");
  Serial.print(mySensor.accelX()); Serial.print(", ");
  Serial.print(mySensor.accelY()); Serial.print(", ");
  Serial.println(mySensor.accelZ());

  Serial.print("Gyro X/Y/Z: ");
  Serial.print(mySensor.gyroX()); Serial.print(", ");
  Serial.print(mySensor.gyroY()); Serial.print(", ");
  Serial.println(mySensor.gyroZ());

  Serial.print("Mag X/Y/Z: ");
  Serial.print(mySensor.magX()); Serial.print(", ");
  Serial.print(mySensor.magY()); Serial.print(", ");
  Serial.println(mySensor.magZ());

  Serial.println("------");
  delay(500);  // Slow down the loop
}
