#include <Arduino.h>

// Define LED pins //
#define L1 44			// Output Status LEDs
#define L2 42
#define L3 40
#define L4 38

// PWM Motor Controller Pins //
#define PwmFRpin 9
#define InaFRpin A3
#define InbFRpin A2

#define PwmFLpin 5
#define InaFLpin A1
#define InbFLpin A0

#define PwmRRpin 6
#define InaRRpin 8
#define InbRRpin 7

#define PwmRLpin 3
#define InaRLpin 4
#define InbRLpin 2

// Battery Monitoring Analog Input //
#define batt24VInput 1	//analog pin 1

// Output signals for TTL Relays //
#define output1 41		
#define output2 43
#define output3 45
#define output4 47

#define strafePinRC 12
#define drivePinRC 13
#define turnPinRC 11
