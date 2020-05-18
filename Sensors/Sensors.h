#ifndef __sensor_h
#define __sensor_h

#include "mbed.h"


#define START_THEREAD_SIGNAL 1
#define MUTEX_MAX_TIMEOUT_MS 2000

class SensorBase{
    
public:
struct MeasureType
    {
        char measureName[32];
        float measureValue;
    };


static SensorBase *SensorList[3];
    
static uint16_t sensorCounter;

static void SensorsInitAll(){
    for(int i=0;i<sensorCounter;++i)
    {
        SensorList[i]->Init();
    }
}

static void SensorsThreadsStartAll(){
    for(int i=0;i<sensorCounter;++i)
    {
        SensorList[i]->StartMeasuring();
    }
}

static uint16_t GetJSONForMeasure (char* jsonOut, uint16_t bufferLength);

SensorBase();

virtual void Init(float loopPeriodTime = 3000);

virtual uint8_t GetSensorMeasureTypesCounter();
virtual uint8_t GetMeasureName(char * buffer, uint8_t bufferSize, uint8_t index = 0);
virtual int GetMeasureValue(uint8_t index = 0);

virtual void SendMeasures();


void SetIdleTime(float RepeatMeasureAfter);
void StartMeasuring();
bool IsUpdated();




protected:

bool initialized;
Thread sensorThread;
float periodTime;
uint8_t measureTypeCount;
MeasureType * measure; 
bool updated;

virtual void takeMeasurement();
static void threadStarter(void const *p);
void sensorLoop();
float roundTo2DecimalPlaces(float value);
int addJSONWithMeasures(char * buffer, uint16_t bufferSize);


};



#endif