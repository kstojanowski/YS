#ifndef __SR04_h
#define __SR04_h
#include "mbed.h"
#include "Sensors.h"
#include "hcsr04.h"


class SR04 : public SensorBase
{
    public:
    SR04(PinName TriggerPin, PinName EchoPin): sens(TriggerPin,EchoPin)
    {
        measureTypeCount=1;
        //printf("\nconstructor done\n");
    };

    void Init(float loopPeriodTime = 500)
    {
        //printf("\n sunlight sensor init\n");
        measure = new MeasureType[1];
        strncpy(measure[0].measureName,"Distance",32);
        measure[0].measureValue=-256.0;
        if(loopPeriodTime>0){
            periodTime = loopPeriodTime;
        }

        
            sensorThread.start(callback(&SensorBase::threadStarter,this));
            initialized=true;
      
    };

    ~SR04()
    {
        delete[] measure;
    }



    void takeMeasurement()
    {
        if(initialized)
        {
                sens.start();
                ThisThread::sleep_for(500);
                measure[0].measureValue = roundTo2DecimalPlaces(sens.get_dist_cm());
                //printf("\n%s: %f\n",measure[0].measureName,measure[0].measureValue);
        }
    };


    private:
    HCSR04 sens;

    

};

#endif