#ifndef __stepper_motors_h
#define __stepper_motors_h

#include "mbed.h"



#define STEP_THREAD_SIGNAL 1

class StepperMotors
{
public:

StepperMotors(PinName _StepPin, PinName _DirPin, int speed = 100, int positionMin = INT_MIN, int positionMax = INT_MAX);

void SetPosition(int newPos);


private:
DigitalOut stepPin;
DigitalOut dirPin;


int positionMin;
int positionMax;

int currentPosition;
int setPosition;
float speed;
bool inverted;
bool isActive;

LowPowerTicker tickLoop;

void loop();

};
#endif