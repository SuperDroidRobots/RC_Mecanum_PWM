#ifndef MECANUMPWM_H
#define MECANUMPWM_H

#include <Arduino.h>

// class used to vector mecanum motors driven by pwm motor controllers
class MecanumPWM {
	public:
		MecanumPWM(int _pwmFR, int _pwmFL, int _pwmRR, int _pwmRL, int _dirFR, int _dirFL, int _dirRR,
			int _dirRL, float _maxSpd);
		~MecanumPWM();
		void commandMotors(float driveChar, float turnChar, float strafeChar);
		void allStop();
		void debugMotorPrint();

	private:
		// pwm and dir pins
		int pwmFR, pwmFL, pwmRR, pwmRL;
		int dirFR, dirFL, dirRR, dirRL;

		// Motor params
		float maxSpd = 200;
		float driveVal, turnVal, strafeVal;

		typedef struct {
			int pulse;
			bool direction;
			bool brake;
		}	MotorValues;

		// Defines structs for each motor
		MotorValues motorFR, motorFL, motorRR, motorRL;

		void setDirection();
		void normalizeVectors();
		void normalizePulses();
		float getAbsolute(float val);
		float convertByteToFloat(unsigned char val);
};
#endif
