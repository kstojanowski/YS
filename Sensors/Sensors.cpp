#include "Sensors.h"


SensorBase::SensorBase() : initialized(false)
{
    SensorBase::sensorCounter++;
    updated=false;

    //TODO dynamic add sensor?
};
void SensorBase::Init(float loopPeriodTime)
{
    measure = new MeasureType;
    strncpy(measure[0].measureName,"status",32);
    measure[0].measureValue=0;
    if(loopPeriodTime>0){
        periodTime = loopPeriodTime;
    }
    else{
        periodTime = 10000;
    }

    sensorThread.start(callback(&SensorBase::threadStarter,this));

};

uint8_t SensorBase::GetSensorMeasureTypesCounter()
{
    return measureTypeCount;
};
uint8_t SensorBase::GetMeasureName(char * buffer, uint8_t bufferSize, uint8_t index)
{
    if(index>=measureTypeCount || measure == NULL)
    {
        return 0;
    }

    strncpy(buffer, measure[index].measureName,bufferSize);
    return strlen(measure[index].measureName);

};
int SensorBase::GetMeasureValue(uint8_t index)
{
    if(index>=measureTypeCount || measure == NULL)
    {
        return INT_MIN;
    }

    return measure[index].measureValue;
};

void SensorBase::SendMeasures()
{
    //for(int i=0;i<measureTypeCount;++i)
    //{
        char json[1024];
        int packet_len = 0;
        //GetJSONForMeasure(json,1024);
        //CommL().SendSingleMeasureToServer(json);
        //}
        
    //}

};

int SensorBase::addJSONWithMeasures(char * buffer, uint16_t bufferSize){
    char tempJSON[64];
    int ret;
    size_t jsonLen=0;
    
    if(!updated){
        printf("\nMeasures from sensor haven't updated yet\n");
        return (-1);
    }
    for(int i=0;i<measureTypeCount;++i){
        ret = snprintf(tempJSON, 64, "{\"type\":\"%s\",\"value\":%.2f}",measure[i].measureName,measure[i].measureValue);
        jsonLen+=ret;
        if(ret>63 || ret<0){
            printf("\nGetJSONWithMeasures(), tempJSON overload\n");
            return (-2);
        }
        if(ret>bufferSize){
            printf("\nGetJSONWithMeasures(), buffer overload\n");
            return(-3);
        }
        if(i>0)
        {
            strcat(buffer,",");
            jsonLen+=1;
            if(ret+jsonLen>bufferSize){
                printf("\nGetJSONWithMeasures(), buffer overload\n");
                return(-4);
            }
            strncat(buffer,tempJSON,bufferSize);
        }
        else{
            strcpy(buffer, tempJSON);
        }
        
    }
        
    updated = false;
    return jsonLen;
    
};


uint16_t SensorBase::GetJSONForMeasure (char* jsonOut, uint16_t bufferLength)
{
    char json_entry[128];
    sprintf(json_entry, "{\"name\":\"%s\",", MBED_CONF_APP_DEVICE_NAME);
    strcpy(jsonOut, json_entry);
    strcat(jsonOut, "\"sensors\":[");
    for(int i=0; i<sensorCounter;i++)
    {
        if(SensorList[i]->IsUpdated())
        {
            if(i>0){
                strcat(jsonOut,",");
            }
            SensorList[i]->addJSONWithMeasures(json_entry, 128);
            strcat(jsonOut, json_entry);
        }
    }

    sprintf(json_entry, "]}\r\n");
    strcat(jsonOut, json_entry);
    return strlen(jsonOut);
}


void SensorBase::SetIdleTime(float RepeatMeasureAfter)
{
    if(RepeatMeasureAfter >= 1)
    {
        periodTime = RepeatMeasureAfter;
    }
    
};

void SensorBase::takeMeasurement()
{
    measure[0].measureValue=-256;
    
};

void SensorBase::StartMeasuring()
{
    if(initialized){
        sensorThread.signal_set(START_THEREAD_SIGNAL);
    }
}


void SensorBase::threadStarter(void const *p)
{
    SensorBase *instance = (SensorBase*)p;
    instance->sensorLoop();
}

void SensorBase::sensorLoop()
{
    if(initialized){
        sensorThread.signal_wait(START_THEREAD_SIGNAL);
        while(periodTime>0)
        {
            ThisThread::sleep_for(periodTime);
            takeMeasurement();
            updated = true;
        
        };
    }


};

float SensorBase::roundTo2DecimalPlaces(float val){
    return roundf(val * 100) / 100;
}

bool SensorBase::IsUpdated(){
    return updated;
}

