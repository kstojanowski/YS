#include "DHT.h"

AM2301::AM2301(PinName pin) {
    _data = new DigitalInOut(pin);
    _last_poll = 0;
}

AM2301::~AM2301() {
}


void AM2301::init(void)
{
    _data->output();
    _data->write(1);
}

int AM2301::read() {
    int n;

    _data->output();
    _data->write(0);
    wait_ms(1);


    timer.reset();
    _data->write(1);
    timer.start();
    _data->input();


    do {
        n = timer.read_us();
        if (n > TIMEOUT) {
            timer.stop();
            return AM2301_RESPONSE_TIMEOUT;
        }
    } while (_data->read() == 1);

     timer.reset();


    if ((n < 10) || (n > 200)) {
        timer.stop();
        return AM2301_RESPONSE_ERROR;
    }


    do {
        n = timer.read_us();
        if (n > TIMEOUT) {
            timer.stop();
            return AM2301_RESPONSE_TIMEOUT;
        }
    } while (_data->read() == 0);


    timer.reset();
    if ((n < 70) || (n > 110)) {
        timer.stop();
        return AM2301_RESPONSE_ERROR;
    }


    do {
        n = timer.read_us();
        if (n > TIMEOUT) {
            timer.stop();
            return AM2301_RESPONSE_TIMEOUT;
        }
    } while (_data->read() == 1);

    timer.reset();
    if ((n < 70) || (n > 100)) {
        timer.stop();
        return AM2301_RESPONSE_ERROR;
    }


    memset(buf, 0, sizeof(buf));

    for (int i = 0; i < N_BYTES; i++) {


        for (int b = 0; b < N_BITS; b++) {

            do {
                n = timer.read_us();
                if (n > TIMEOUT) {
                    timer.stop();
                    return AM2301_READ_BIT_TIMEOUT;
                }
            } while (_data->read() == 0);
            timer.reset();


            if ((n < 40) || (n > 70)) {
                return AM2301_READ_BIT_ERROR;
            }


            do {
                n = timer.read_us();
                if (n > TIMEOUT) {
                    timer.stop();
                    return AM2301_READ_BIT_TIMEOUT;
                }
            } while (_data->read() == 1);
            timer.reset();

            if ((n > 15) && (n < 35)) {
                buf[i] <<= 1;
            } else if ((n > 65) && (n < 80)) {
                buf[i] = ((buf[i] << 1) | 1);
            } else
                return AM2301_READ_BIT_ERROR;
        }
    }

    
    timer.stop();


    _data->output();
    _data->write(1);


    int checksum = (buf[0] + buf[1] + buf[2] + buf[3]);



    if ((checksum & ONE_BYTE_MASK) == buf[4]) {
        return AM2301_SUCCESS;
    } else {
        return AM2301_CHECKSUM_ERROR;
    }
}

void AM2301::close(void)
{
    timer.stop();
    _data->output();
    _data->write(1);
}

int AM2301::calculate(void)
{


    if ((_last_poll != 0)) {
        if ((time(NULL) - _last_poll < 2)) {
            return AM2301_POLLING_INTERVAL_ERROR;
        }
    }

    
    this->init();


    int ret = this->read();


    this->close();


    _last_poll = time(NULL);


    if (ret == AM2301_SUCCESS) {


        _humidity = static_cast<int16_t>((buf[0] << N_BITS)  + buf[1]);

       
        _temperature = static_cast<int16_t>(((buf[2] & WITHOUT_SIGN) << N_BITS)  + buf[3]);


        if ((buf[0] & CHECK_SIGN) == CHECK_SIGN) {
            _temperature *= -1;
        }
    }


    return ret;
}


int AM2301::get_temperature() {
    return _temperature;
}


int AM2301::get_humidity() {
    return _humidity;
}