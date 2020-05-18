#include "YS.h"
#include "mbedtls/debug.h"
#include "Sensors.h"



YS::YS(){
    State=0;
};


void YS::SendMeasures()
{
    SensorBase::GetJSONForMeasure(sendMeasuresBuffer, 1024);
    printf("\n%s\n",sendMeasuresBuffer);
    
}
void SendMeasuresLoop(){
    ThisThread::sleep_for(1000);
    //YS::getSingleton().SendMeasures();
}

void YS::SendMeasuresThreadStart(void const *p){
    YS *instance = (YS*)p;
    //instance->SendMeasuresLoop();
};

bool YS::ReceiveControlValue()
{
    while(true){
        //gets(reciveBuffer,22);
        if(reciveBuffer[0]=='#' && reciveBuffer[1]=='Y' && reciveBuffer[2]=='S')
        {
            
        }
    }
    

}





