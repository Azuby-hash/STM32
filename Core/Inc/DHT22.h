#ifndef __DHT22_H
#define __DHT22_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stdint.h"

typedef struct {
    uint32_t time;
    GPIO_TypeDef* port;
    uint16_t pin;
    float temp;
    float humidity;
}DHTHandler;

void dhtInitHandler(DHTHandler* handler, GPIO_TypeDef* port, uint16_t pin);
uint16_t dhtGetTemp();
uint16_t dhtGetHumidity();

#endif