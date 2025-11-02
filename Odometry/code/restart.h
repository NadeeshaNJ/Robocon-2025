#ifndef RESTART_H
#define RESTART_H

#include "receiver.h"
#include "pins.h"
#include <Arduino.h>

#define TOGGLE_COUNT 3   // Number of toggles required
#define TIMEOUT_MS 2000  // Time window in milliseconds

void restart()
{
    int toggleCount = 0;
    unsigned long startTime = millis();
    bool lastKillState = false; // Store last state of kill switch

    Serial.println("🔄 Restart mode activated. Toggle kill switch to restart.");

    while (true)  // Stay here until restart
    {
        if (sbus_rx.Read()) // Check if SBUS data is available
        {
            data = sbus_rx.data();
            bool killSwitch = (data.ch[KILL_SWITCH_CHANNEL] > 1000); // CH6: Kill switch

            if (killSwitch != lastKillState) // Detect a toggle
            {
                toggleCount++;
                lastKillState = killSwitch;
                Serial.print("Kill switch toggled: ");
                Serial.println(toggleCount);
            }

            // Restart the system if toggle count met within time window
            if (toggleCount >= TOGGLE_COUNT && millis() - startTime <= TIMEOUT_MS)
            {
                Serial.println("🔄 Restarting system...");
                delay(100);  // Small delay before reset
                ESP.restart();  // Safe reset for ESP32
            }
        }

        // If time expired, reset counter but continue listening
        if (millis() - startTime > TIMEOUT_MS)
        {
            toggleCount = 0; // Reset toggle count
            startTime = millis(); // Reset timer
            Serial.println("⏳ Time expired. Restart attempt reset. Keep toggling...");
        }

        delay(1); // Prevents watchdog timeout
    }
}

#endif // RESTART_H
