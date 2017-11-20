/*********************************************************

Quad-Wheel Mecanum Vectoring Robot with RC Control
Brent Clay
SuperDroid Robots
December 1, 2015

This code uses an Arduino Uno mounted on a Quad-Wheel Mecanum Vectoring
platform (TP-152-004, TP-095-004, or TP-252-004). The Arduino commands
two dual channel Sabertooth motor controllers to drive four independent
Mecanum wheels. Sabertooths are set to serial mode with addresses 128
and 129.

This firmware allows vectoring RC control of the robot's motion

Code is written for a Spektrum remote + receiver but could easily be reused
for other RC approaches

Platforms:
http://www.superdroidrobots.com/shop/item.aspx/programmable-mecanum-wheel-vectoring-robot-ig52-db/1788/
http://www.superdroidrobots.com/shop/item.aspx/programmable-mecanum-wheel-vectoring-robot-ig32-sb/1713/
http://www.superdroidrobots.com/shop/item.aspx/programmable-mecanum-wheel-vectoring-robot-ig32-dm/1487/

Motor Controller:
http://www.superdroidrobots.com/shop/item.aspx/sabertooth-dual-12a-motor-driver/1155/

Vectoring Robot Support Page:
http://www.superdroidrobots.com/shop/custom.aspx/vectoring-robots/44/

Spektrum DX5e:
http://www.superdroidrobots.com/shop/item.aspx/spektrum-dx5etransmitter-with-ar610-receiver/992/

***********************************************************/

// ****************************************************
// Libraries
// ****************************************************
#include "hardware.h"
#include "MecanumPWM.h"

// *********************
// RC Vars
// *********************
unsigned long DRIVE_PULSE_WIDTH;
unsigned long TURN_PULSE_WIDTH;
unsigned long STRAFE_PULSE_WIDTH;
float pulseLow = 1051, pulseHigh = 1890;
float mByte = 0, bByte = 0;
float mFloat = 0, bFloat = 0;

// Drive commands

float spdMax = 240;

// Initialize motor control
MecanumPWM mecPWM(pwmFRpin, pwmFLpin, pwmRRpin, pwmRLpin, dirFRpin, dirFLpin, dirRRpin, dirRLpin, spdMax);

void setup() {

  Serial.begin(9600); 

// Command all motors to stop
   mecPWM.allStop();
// slope/intercept for converting RC signal to range [-1,1]
  mFloat = (float)2 / (pulseHigh - pulseLow);
  bFloat = -1*pulseLow*mFloat;
}

void loop() {
  float driveVal;
  float turnVal;
  float strafeVal;
  // Read in the RC pulses
  DRIVE_PULSE_WIDTH = pulseIn(drivePinRC, HIGH);//, PULSEIN_TIMEOUT);
  TURN_PULSE_WIDTH  = pulseIn(turnPinRC, HIGH);//, PULSEIN_TIMEOUT);
  STRAFE_PULSE_WIDTH  = pulseIn(strafePinRC, HIGH);//, PULSEIN_TIMEOUT);
 
  if (DRIVE_PULSE_WIDTH <900 || DRIVE_PULSE_WIDTH > 2000 ) driveVal = 0;
  else (driveVal = convertRCtoFloat(DRIVE_PULSE_WIDTH));
  
  if (TURN_PULSE_WIDTH <900 || TURN_PULSE_WIDTH > 2000) turnVal  = 0;
  else turnVal  = convertRCtoFloat(TURN_PULSE_WIDTH);
  
  if (STRAFE_PULSE_WIDTH <900 || STRAFE_PULSE_WIDTH > 2000) strafeVal = 0;
  else strafeVal = convertRCtoFloat(STRAFE_PULSE_WIDTH);

 // Serial.println("drive pulse");
 // Serial.println(driveVal);
  //Serial.println("turn pulse");  
  //Serial.println(turnVal);
 // Serial.println("strafe pulse");
  //Serial.println(strafeVal);
// calculate and apply pulse width to PWM outputs to motor controllers
  mecPWM.commandMotors(driveVal, turnVal, strafeVal);
  mecPWM.debugMotorPrint();
}

float convertRCtoFloat(unsigned long pulseWidth)
{
  // deadband
  if(pulseWidth > 1450 && pulseWidth < 1550) { pulseWidth = (float)(pulseHigh + pulseLow) / 2; }
  
  float checkVal = mFloat*pulseWidth + bFloat - 1;
  checkVal = checkVal < -1 ? -1 : checkVal;
  checkVal = checkVal >  1 ?  1 : checkVal;
  if (pulseWidth = 0) checkVal = 0;
  return checkVal;
}




