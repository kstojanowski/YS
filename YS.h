
#ifndef __YS_h
#define __YS_h

#include "mbed.h"
#include "mbed_trace.h"



class YS{
    
private:



    Thread sendMeasuresThread;

    char sendMeasuresBuffer[1024];
    uint8_t State;

    

    YS();

public:

    char reciveBuffer[32];

    static YS & getSingleton()
    {
        static YS singleton;
        return singleton;
    }
    void SendMeasures();
    bool ReceiveControlValue();
    static void SendMeasuresThreadStart(void const *p);
    void SendMeasuresLoop();
    


};



#endif