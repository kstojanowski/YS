#ifndef __BAR30_h
#define __BAR30_h
#include "mbed.h"
#include "Sensors.h"
#include "MS5837.h"


class BAR30 : public SensorBase
{
    public:
    BAR30(PinName sda, PinName scl): sens(sda,scl)
    {
        measureTypeCount=3;
        //printf("\nconstructor done\n");
    };

    void Init(float loopPeriodTime = 500)
    {
        //printf("\n sunlight sensor init\n");
        measure = new MeasureType[3];
        strncpy(measure[0].measureName,"Temperature_Bar30",32);
        strncpy(measure[1].measureName,"Pressure",32);
        strncpy(measure[2].measureName,"Depth",32);
        measure[0].measureValue=-256.0;
        measure[1].measureValue=-256.0;
        measure[2].measureValue=-256.0;
        if(loopPeriodTime>0){
            periodTime = loopPeriodTime;
        }

        //TODO mutex for i2c
        //if(SensorBase::Sensor_I2C_Mutex.trylock_for(MUTEX_MAX_TIMEOUT_MS))
        //{
            //sensorThread.start(this, &Sensors::threadStarter);
            sensorThread.start(callback(&SensorBase::threadStarter,this));
            initialized=true;
        //    SensorBase::Sensor_I2C_Mutex.unlock();
        //}
        //else
        //{
        //    initialized=false;
        //}
    };

    ~BAR30()
    {
        delete[] measure;
    }



    void takeMeasurement()
    {
        if(initialized)
        {
            //if(SensorBase::Sensor_I2C_Mutex.trylock_for(MUTEX_MAX_TIMEOUT_MS))
            //{
                measure[0].measureValue = roundTo2DecimalPlaces(sens.MS5837_Temperature());
                //printf("\n%s: %f\n",measure[0].measureName,measure[0].measureValue);
                measure[1].measureValue = roundTo2DecimalPlaces(sens.MS5837_Pressure());
                //printf("\n%s: %f\n",measure[1].measureName,measure[1].measureValue);
                measure[2].measureValue = roundTo2DecimalPlaces(sens.get_depth());
                //printf("\n%s: %f\n",measure[2].measureName,measure[2].measureValue);
            //    SensorBase::Sensor_I2C_Mutex.unlock();
            //}
            //else
            //{
            //    
            //}
        }
    };


    private:
    MS5837 sens;

    

};



#endif