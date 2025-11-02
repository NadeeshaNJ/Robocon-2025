#ifndef RECEIVER_H
#define RECEIVER_H

#include "sbus.h"
#include <Arduino.h>

bfs::SbusRx sbus_rx(&Serial2, S_BUS_IN, -1, true);
bfs::SbusData data;

void setupReciever()
{
    /* Start SBUS communication */
    sbus_rx.Begin();
    Serial.println("SBUS Receiver Started!");
}



#endif