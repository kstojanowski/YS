#include "StepperMotors.h"



StepperMotors::StepperMotors(PinName _StepPin, PinName _DirPin, int _speed, int _positionMin , int _positionMax ):
                        stepPin(_StepPin),
                        dirPin(_DirPin),
                        speed(_speed),
                        positionMin(_positionMin),
                        positionMax(_positionMax)

{
    currentPosition=0;
    if(speed==0) speed = 1;
    if(speed<0){
        speed= speed*(-1);
        inverted = true;   
    }
    isActive=false;
    tickLoop.attach(this,&StepperMotors::loop,(1/(speed*2)));   
};


void StepperMotors::loop()
{
    if(isActive){
        stepPin.write(0);
        isActive=false;
    }
    else 
    {
        if(setPosition!=currentPosition){
            if(setPosition>currentPosition)
            {
                inverted ? dirPin.write(1) : dirPin.write(0);
                ++currentPosition;
            }
            else
            {
                inverted ? dirPin.write(0) : dirPin.write(1);
                --currentPosition;
            }  
            isActive=true;
            stepPin.write(1);

        }
    }
};

void StepperMotors::SetPosition(int newPos)
{
    if(newPos<positionMin){ 
        setPosition=positionMin+1;
    }
    else if (newPos>positionMax) {
        setPosition=positionMax-1;
    }
    else{
        setPosition = newPos;
    }
}



