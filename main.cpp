#include "mbed.h"
#include "YS.h"
#include "Sensors.h"
#include "DHT21.h"
#include "Bar30.h"
#include "SR04.h"
#include "StepperMotors.h"
#include "DCMotors.h"

static Serial pc(USBTX,USBRX,"YellowSubmarine",115200);

uint16_t SensorBase::sensorCounter;
const int RecStr[] = {35,89,83,35,35};
int recValue[8];

Thread recThread;



SensorBase *SensorBase::SensorList[] = {new DHT21(PB_10), new BAR30(I2C_SDA,I2C_SCL), new SR04(PB_11,PE_0) };

void device_loop();
void set_motor_value();

StepperMotors CLF(PG_3,PD_7,200),CRF(PD_6,PD_5,200),CCB(PD_4,PD_3,200);
DCMotors DCLeft(PE_9,PF_12), DCRight(PE_11,PF_13);

// main() runs in its own thread in the OS
int main()
{

    SensorBase::SensorsInitAll();

    SensorBase::SensorsThreadsStartAll();

    recThread.start(&device_loop);

    while (true) {
        
        ThisThread::sleep_for(1000);
        
        YS::getSingleton().SendMeasures();
    }


}

void device_loop()
{
    uint8_t iter;
	uint8_t crc=0;
    int rec;
    int len;
    int16_t temp;

    while(1)
    {
        iter=0;
        len=0;
        while(iter<22)
        {
            
            rec = pc.getc();
            if(iter<4){
                if(rec==RecStr[iter])
                {
                    iter==0 ? crc=rec : crc^=rec;
                    iter++;
                }
                else
                {
                    rec==RecStr[0]? iter =1 : iter = 0;
                }
            }
            else if(iter==4)
            {
                if(rec>0){
                    len=rec;
                    iter++;
                }
                else {
                    rec==RecStr[0]? iter =1 : iter = 0;
                }
            }else if(iter>4 && iter<21)
            {
                if((iter-5)%2==0)
                {
                    temp = uint16_t(rec<<8);
                    iter++;
                }
                else{
                    temp = temp|rec;
                    recValue[(iter-5)/2]= temp;
                    iter++;
                }

            }else if(iter>=21){
                iter++;
            }
        }
        
        set_motor_value();
		
    }
}


void set_motor_value(){
    DCLeft.SetValue(recValue[0]);
    DCRight.SetValue(recValue[1]);
    CLF.SetPosition(recValue[2]);
    CRF.SetPosition(recValue[3]);
    CCB.SetPosition(recValue[4]);

}