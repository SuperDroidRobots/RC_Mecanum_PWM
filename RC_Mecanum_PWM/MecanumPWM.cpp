#include "MecanumPWM.h"

//-- public methods --//

//<<constructor>>
//pass in the values for the PWM and direction pins to the pwm motor controllers
MecanumPWM::MecanumPWM(int _pwmFR, int _pwmFL, int _pwmRR, int _pwmRL, int _inaFRpin, int _inbFRpin, int _inaFLpin, int _inbFLpin, int _inaRRpin, int _inbRRpin,
      int _inaRLpin, int _inbRLpin, float _maxSpd)
{
	pwmFR = _pwmFR; pwmFL = _pwmFL; pwmRR = _pwmRR; pwmRL = _pwmRL;
  inaFRpin = _inaFRpin; inbFRpin = _inbFRpin; inaFLpin = _inaFLpin; inbFLpin = _inbFLpin;
  inaRRpin = _inaRRpin; inbRRpin = _inbRRpin; inaRLpin = _inaRLpin; inbRLpin = _inbRLpin;
	maxSpd = _maxSpd;

	pinMode(inaFRpin, OUTPUT); pinMode(inbFRpin, OUTPUT);
  pinMode(inaFLpin, OUTPUT); pinMode(inbFLpin, OUTPUT);
  pinMode(inaRRpin, OUTPUT); pinMode(inbRRpin, OUTPUT);
  pinMode(inaRLpin, OUTPUT); pinMode(inbRLpin, OUTPUT);



}

//<<destructor>>
MecanumPWM::~MecanumPWM() { }

// Stops the Robot
void MecanumPWM::allStop() {
	commandMotors(0, 0, 0);
}

void MecanumPWM::commandMotors(float driveChar, float turnChar, float strafeChar) {

	driveVal = driveChar;
	turnVal = turnChar;
	strafeVal = strafeChar;

	//normalizeVectors();

	motorFR.pulse = (driveVal + turnVal + strafeVal)*maxSpd;
	motorRR.pulse = (driveVal + turnVal - strafeVal)*maxSpd;
	motorFL.pulse = (driveVal - turnVal - strafeVal)*maxSpd;
	motorRL.pulse = (driveVal - turnVal + strafeVal)*maxSpd;
	/*
	Serial.print(motorFR.pulse); Serial.print(",");
	Serial.print(motorFL.pulse); Serial.print(",");
	Serial.print(motorRR.pulse); Serial.print(",");
	Serial.print(motorRL.pulse); Serial.println("");
	*/
	// write high or low to direction pins
	setDirection();

	if (motorFR.pulse > maxSpd || motorFL.pulse > maxSpd ||
		motorRR.pulse > maxSpd || motorRL.pulse > maxSpd)
	{
		normalizePulses();
	}

	analogWrite(pwmFR, motorFR.pulse);
	analogWrite(pwmFL, motorFL.pulse);
	analogWrite(pwmRR, motorRR.pulse);
	analogWrite(pwmRL, motorRL.pulse);

}

void MecanumPWM::debugMotorPrint()
{
	Serial.print(driveVal); Serial.print(","); Serial.print(turnVal); Serial.print(",");
	Serial.print(strafeVal); Serial.print("\t");
	Serial.print(motorFR.pulse); Serial.print(",");
	Serial.print(motorFL.pulse); Serial.print(",");
	Serial.print(motorRR.pulse); Serial.print(",");
	Serial.print(motorRL.pulse); Serial.print("\t");
  Serial.print(motorFR.direction); Serial.print(",");
  Serial.print(motorFL.direction); Serial.print(",");
  Serial.print(motorRR.direction); Serial.print(",");
  Serial.println(motorRL.direction); 
        
        Serial.print("\n");
}

//-- private methods --//
void MecanumPWM::setDirection() {
	bool dir = LOW;
	motorFR.direction = motorFR.pulse > 0 ? HIGH : LOW; digitalWrite(inaFRpin, motorFR.direction); digitalWrite(inbFRpin, !motorFR.direction);
        //Serial.print(dir); Serial.print(",");
	motorFL.direction = motorFL.pulse > 0 ? LOW : HIGH; digitalWrite(inaFLpin, motorFL.direction); digitalWrite(inbFLpin, !motorFL.direction);
        //Serial.print(dir); Serial.print(",");
	motorRR.direction = motorRR.pulse > 0 ? HIGH : LOW; digitalWrite(inaRRpin, motorRR.direction); digitalWrite(inbRRpin, !motorRR.direction);
        //Serial.print(dir); Serial.print(",");
	motorRL.direction = motorRL.pulse > 0 ? LOW : HIGH; digitalWrite(inaRLpin, motorRL.direction); digitalWrite(inbRLpin, !motorRL.direction);
        //Serial.print(dir); Serial.print("\t"); Serial.print("\t");

	motorFR.pulse = getAbsolute(motorFR.pulse);
	motorFL.pulse = getAbsolute(motorFL.pulse);
	motorRR.pulse = getAbsolute(motorRR.pulse);
	motorRL.pulse = getAbsolute(motorRL.pulse);

	/*
	if(motorA.pulse > spdScaler*rampLimit || motorB.pulse > spdScaler*rampLimit ||
	motorC.pulse > spdScaler*rampLimit)
	{
		normalizePulses();
	}
	*/
}

// Scale pulse outputs
void MecanumPWM::normalizePulses()
{
	float a = motorFR.pulse, b = motorFL.pulse, c = motorRR.pulse, d = motorRL.pulse;

	float maxVal = max(a, b);
	maxVal = max(maxVal, c); maxVal = max(maxVal, d);
	if (maxVal < maxSpd) { return; }

	float scale = (float)maxSpd / maxVal;

	motorFR.pulse = scale * a;
	motorFL.pulse = scale * b;
	motorRR.pulse = scale * c;
	motorRL.pulse = scale * d;
}

void MecanumPWM::normalizeVectors() {
	float a = driveVal, b = turnVal, c = strafeVal;
	float len = sqrt(a*a + b*b + c*c);
	if (len < 1) { return; }

	driveVal = a / len;
	turnVal = b / len;
	strafeVal = c / len;
}

float MecanumPWM::getAbsolute(float val) {
	val = val < 0 ? -1 * val : val;
	return val;
}

float MecanumPWM::convertByteToFloat(unsigned char val)
{
	float returnVal = 0;
	if (val > 124 && val < 130) { val = 127; }

	//if(val > 127)       { returnVal = (float)(val - 127) / 127; }
	//else if(val < 127)  { returnVal = -1*(float)(127 - val) / 127; }

	returnVal = (float)(val - 127) / 127;

	returnVal = returnVal >  1 ? 1 : returnVal;
	returnVal = returnVal < -1 ? -1 : returnVal;

	return returnVal;
}
