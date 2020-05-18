#ifndef __DC_motors_h
#define __DC_motors_h

#include "mbed.h"


class DCMotors
{
public:

DCMotors(PinName _PWMPin, PinName _DirPin, bool invert = false, float _period = 0.02);

void SetValue(int16_t newValue);


private:
PwmOut PWMPin;
DigitalOut dirPin;

int16_t setDuty;

float period;

float PWMDuty;
bool inverted;

};
#endif