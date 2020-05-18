#ifndef __DHT21_h
#define __DHT21_h
#include "mbed.h"
#include "Sensors.h"
#include "DHTSensor/DHT.h"

class DHT21 : public SensorBase
{
    public:
    DHT21(PinName _signalPin): sens(_signalPin)
    {
        measureTypeCount=2;
    };

    void Init(float loopPeriodTime = 3000)
    {
        measure = new MeasureType[2];
        strncpy(measure[0].measureName,"Temperature",32);
        strncpy(measure[1].measureName,"Humidity",32);
        measure[0].measureValue=-256.0;
        measure[1].measureValue=-256.0;
        if(loopPeriodTime>0){
            periodTime = loopPeriodTime;
        }

        //sensorThread.start(this, &Sensors::threadStarter);
        sensorThread.start(callback(&SensorBase::threadStarter,this));
        initialized = true;

    };

    ~DHT21()
    {
        delete[] measure;
    }



    void takeMeasurement()
    {
        int ret = sens.calculate();
        if(ret!=0){
            //printf("\ndht read error %d\n", ret);
            return;
        }
        measure[0].measureValue = roundTo2DecimalPlaces(sens.get_temperature());
        //printf("\ndht temp: %f\n",measure[0].measureValue);
        measure[1].measureValue = roundTo2DecimalPlaces(sens.get_humidity());
        //printf("\ndht humi: %f\n",measure[1].measureValue);
    };
    static void test(){
        printf("ok");
    }

    private:
    AM2301 sens;

    

};

#endif