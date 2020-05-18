#ifndef MBED_AM2301_H
#define MBED_AM2301_H

#include "mbed.h"

#define ONE_BYTE_MASK         0xFF
#define N_BITS             8
#define N_BYTES            5
#define WITHOUT_SIGN 0x7F
#define CHECK_SIGN 0x80
#define TIMEOUT        300


enum exitCodes {
    AM2301_SUCCESS = 0,
    AM2301_RESPONSE_TIMEOUT = -1,
    AM2301_RESPONSE_ERROR = -2,
    AM2301_READ_BIT_TIMEOUT = -3,
    AM2301_READ_BIT_ERROR = -4,
    AM2301_CHECKSUM_ERROR = -5,
    AM2301_POLLING_INTERVAL_ERROR = -6
} ;

class AM2301 {

    

public:

    AM2301(PinName pin);


    ~AM2301();

    int calculate(void);

    int get_temperature(void);

    int get_humidity(void);

private:

    int _temperature;

    int _humidity;


    time_t _last_poll;

    DigitalInOut *_data;

    Timer timer;

    int buf[5];
    void init();

    int read();
    void close();
};

#endif