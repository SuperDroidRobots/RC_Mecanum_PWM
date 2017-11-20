#include "MecanumPWM.h"

//-- public methods --//

//<<constructor>>
//pass in the values for the PWM and direction pins to the pwm motor controllers
MecanumPWM::MecanumPWM(int _pwmFR, int _pwmFL, int _pwmRR, int _pwmRL, int _dirFR, int _dirFL,
	int _dirRR, int _dirRL, float _maxSpd)
{
	pwmFR = _pwmFR; pwmFL = _pwmFL; pwmRR = _pwmRR; pwmRL = _pwmRL;
	dirFR = _dirFR; dirFL = _dirFL; dirRR = _dirRR; dirRL = _dirRL;

	maxSpd = _maxSpd;

	pinMode(dirFR, OUTPUT); pinMode(dirFL, OUTPUT);
	pinMode(dirRR, OUTPUT); pinMode(dirRL, OUTPUT);
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
       // Serial.print(dirFR); Serial.print(",");
        //Serial.print(dirFL); Serial.print(",");
        //Serial.print(dirRR); Serial.print(",");
        //Serial.println(dirRL); 
        
        Serial.print("\n");
}

//-- private methods --//
void MecanumPWM::setDirection() {
	bool dir = LOW;
	dir = motorFR.pulse > 0 ? HIGH : LOW; digitalWrite(dirFR, dir);
        Serial.print(dir); Serial.print(",");
	dir = motorFL.pulse > 0 ? LOW : HIGH; digitalWrite(dirFL, dir);
        Serial.print(dir); Serial.print(",");
	dir = motorRR.pulse > 0 ? HIGH : LOW; digitalWrite(dirRR, dir);
        Serial.print(dir); Serial.print(",");
	dir = motorRL.pulse > 0 ? LOW : HIGH; digitalWrite(dirRL, dir);
        Serial.print(dir); Serial.print("\t"); Serial.print("\t");

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
