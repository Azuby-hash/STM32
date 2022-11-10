#ifndef __DHT22_H
#define __DHT22_H

#include "stm32f1xx_hal.h"
#include "stdint.h"
#include "stdbool.h"

typedef struct {
	TIM_HandleTypeDef* timer;
	uint16_t pin;
	GPIO_TypeDef* port;
	float temp;
	float humi;
} DHT_Prop;

void DHT_Init(DHT_Prop* DHT, TIM_HandleTypeDef* Timer, GPIO_TypeDef* DH_PORT, uint16_t DH_Pin);
void DHT_ReadTempHum(DHT_Prop* DHT);

#endif