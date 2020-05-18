#include "DCMotors.h"
#include "mbed.h"


DCMotors::DCMotors(PinName _PWMPin, PinName _DirPin, bool invert, float _period)
    : PWMPin(_PWMPin), dirPin(_DirPin), inverted(invert), period(_period)
{
    PWMPin.period(period);
    PWMPin.write(0);

    inverted ? dirPin.write(1) : dirPin.write(0);

}

void DCMotors::SetValue(int16_t newValue){

    
    
    if(newValue<0){
        inverted ? dirPin.write(0) : dirPin.write(1);
        setDuty = (newValue<=INT16_MIN ? INT16_MIN+1 : newValue)*(-1);

    }
    else{
        inverted ? dirPin.write(1) : dirPin.write(0);
        setDuty = newValue>=INT16_MAX ? INT16_MAX-1 : newValue;
    }
    
    PWMDuty = float(setDuty)/float(INT16_MAX);

    PWMPin.write(PWMDuty);


}