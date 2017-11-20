#include <Arduino.h>

// Define LED pins //
#define L1 44			// Output Status LEDs
#define L2 42
#define L3 40
#define L4 38

// PWM Motor Controller Pins //
#define pwmFRpin 5
#define dirFRpin 4
//#define brkA 1
#define pwmFLpin 3
#define dirFLpin 2
//#define brkB 8
#define pwmRRpin 6
#define dirRRpin 7
//#define brkC 11
#define pwmRLpin 9
#define dirRLpin 8

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
