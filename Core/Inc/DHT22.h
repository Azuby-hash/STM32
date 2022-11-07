#ifndef __DHT22_H
#define __DHT22_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stdint.h"

uint16_t getTemp();
uint16_t getHumidity();

class DHTHandler {
    public:
        void DHTInit();
        void DHTStart();
        uint16_t DHTGetTemp();
        uint16_t DHTGetHumidity();
        void DHTStop();
    private:
};

#endif