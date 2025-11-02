#include "Bitcraze_PMW3901.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
//#include <cmath>

#define BNO055_SAMPLERATE_DELAY_MS (100)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29);

// Using digital pin 10 for chip select
Bitcraze_PMW3901 flow(5);

float yaw,pitch,roll;
float dx_world, dy_world;
float dx_raw,dy_raw;
float x_new, y_new;

struct SensorData {
  int16_t deltaX;
  int16_t deltaY;
  float yaw;
  float x_new;
  float y_new;
};

void printSensorData(const SensorData& data) {
  Serial.print("deltaX: ");
  Serial.print(data.deltaX);
  Serial.print(", deltaY: ");
  Serial.print(data.deltaY);
  Serial.print(", yaw: ");
  Serial.print(data.yaw);
  Serial.print(", x_new: ");
  Serial.print(data.x_new);
  Serial.print(", y_new: ");
  Serial.println(data.y_new);
}

void setup() {
  Serial.begin(115200);

  if (!flow.begin()) {
    Serial.println("Initialization of the flow sensor failed");
    while(1) { }
  }
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
   
  delay(1000);

  /* Use external crystal for better accuracy */
  bno.setExtCrystalUse(true);
}

int16_t deltaX,deltaY;

void loop() {
  // Get motion count since last call
  flow.readMotionCount(&deltaX, &deltaY);

  // Serial.print("X: ");
  // Serial.print(deltaX);
  // Serial.print(", Y: ");
  // Serial.print(deltaY);
  // Serial.print("\n");

  sensors_event_t event;
  bno.getEvent(&event);

  yaw = event.orientation.x;
  pitch = event.orientation.y;
  roll = event.orientation.z;

  // Serial.print(F("Orientation: x: "));
  // Serial.print(yaw);
  // Serial.print(F(" y: "));
  // Serial.print(pitch);
  // Serial.print(F(" z: "));
  // Serial.print(roll);
  // Serial.println(F(""));


 float yaw_rad = yaw * PI / 180.0;

  dx_world = deltaX * cos(yaw_rad) - deltaY * sin(yaw_rad);
  dy_world = deltaX * sin(yaw_rad) + deltaY * cos(yaw_rad);


  x_new = dx_world;
  y_new = dy_world;


  // Serial.print("displacement x: ");
  // Serial.print(x_new);
  // Serial.print(F(" y: "));
  // Serial.println(y_new);

SensorData data;
data.deltaX = deltaX;
data.deltaY = deltaY;
data.yaw = yaw;
data.x_new = x_new;
data.y_new = y_new;

printSensorData(data);

  delay(100);
}