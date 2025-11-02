#ifndef PINS_H
#define PINS_H

// Receiver Input Pins
#define S_BUS_IN 16

// BTS7960 Motor Driver Pins (Wheels) (Right)
#define RRPWM 25
#define RLPWM 26
#define REN 33

// BTS7960 Motor Driver Pins (Wheels) (Left)
#define LRPWM 18
#define LLPWM 19
#define LEN 21

// Weapon Motor Pins
#define WLPWM 12
#define WRPWM 14
#define WEN 27

#define NOISE_THRESHOLD 15    // Ignore small values < 15 in PWM

#define THROTTLE_CHANNEL 1
#define STEERING_CHANNEL 0
#define WEAPON_CHANNEL 2
#define KILL_SWITCH_CHANNEL 5 // Channel 6 (zero-based index)
#define WEAPON_STATE_CHANNEL 6 // Channel 7
#define STEERING_INVERT_CHANNEL 7 // Channel 8

#define TIMEOUT_MS 500        // Stop bot if no SBUS data received in 500ms

#endif