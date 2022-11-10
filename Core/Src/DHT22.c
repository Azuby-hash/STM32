#include "DHT22.h"
#include "stdbool.h"

bool isDHT_BeginSteps[6];
bool isDHT_ReadBits[33];
bool isDHT_EndStep = false;

uint16_t DHTTemp, DHTHumi = 0;

void DHT_SetPinAsOutput(DHT_Prop *dht) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = dht->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(dht->port, &GPIO_InitStruct);
}
void DHT_SetPinAsInput(DHT_Prop *dht) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = dht->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(dht->port, &GPIO_InitStruct);
}

// Call outside
 
void DHT_Init(DHT_Prop *dht, TIM_HandleTypeDef *timer, GPIO_TypeDef *port, uint16_t pin) {
    dht->pin = pin;
    dht->port = port;
    dht->timer = timer;

    uint8_t i;

    for(i = 0; i < 5; i++) {
        isDHT_BeginSteps[i] = false;
    }
    for(i = 0; i < 33; i++) {
        isDHT_ReadBits[i] = false;
    }

    HAL_TIM_Base_Start(timer);
}
void DHT_ReadTempHum(DHT_Prop *dht) {
    if(!isDHT_BeginSteps[1]) {
        isDHT_EndStep = false;
    	DHT_SetPinAsOutput(dht);
        HAL_GPIO_WritePin(dht->port, dht->pin, 0);
        isDHT_BeginSteps[1] = true;
        __HAL_TIM_SET_COUNTER(dht->timer,0);
    }
    if(!isDHT_BeginSteps[2]) {
        if (__HAL_TIM_GET_COUNTER(dht->timer) < 18000) {
            return;
        }
        DHT_SetPinAsInput(dht);
        isDHT_BeginSteps[2] = true;
        __HAL_TIM_SET_COUNTER(dht->timer,0);
    }
    if(!isDHT_BeginSteps[3]) {
        if (__HAL_TIM_GET_COUNTER(dht->timer) < 40) {
            return;
        }
        if (!HAL_GPIO_ReadPin(dht->port, dht->pin)) {
            isDHT_BeginSteps[3] = true;
            __HAL_TIM_SET_COUNTER(dht->timer,0);
        } else {
            isDHT_BeginSteps[3] = true
        }
    }
    if(!isDHT_BeginSteps[4]) {
        if (__HAL_TIM_GET_COUNTER(dht->timer) < 40) {
            return;
        }
        if (!HAL_GPIO_ReadPin(dht->port, dht->pin)) {
            return;
        }

        isDHT_BeginSteps[4] = true;
        __HAL_TIM_SET_COUNTER(dht->timer,0);
    }

    uint8_t i = 0;

    for (i = 0; i < 16; i++) {
        if (!isDHT_ReadBits[i]) {
            if (HAL_GPIO_ReadPin(dht->port, dht->pin)) {
                return;
            }
            if (__HAL_TIM_GET_COUNTER(dht->timer) < 40) {
                return;
            }
            if (!HAL_GPIO_ReadPin(dht->port, dht->pin)) {
                Humi &= ~(1<<(15-i));	
            } else {
                Humi |= 1<<(15-i);
            }
            if (!HAL_GPIO_ReadPin(dht->port, dht->pin)) {
                return;
            }
            isDHT_ReadBits[i] = true
            __HAL_TIM_SET_COUNTER(dht->timer,0);
        }
    }
    for (i = 16; i < 32; i++) {
        if (!isDHT_ReadBits[i]) {
            if (HAL_GPIO_ReadPin(dht->port, dht->pin)) {
                return;
            }
            if (__HAL_TIM_GET_COUNTER(dht->timer) < 40) {
                return;
            }
            if (!HAL_GPIO_ReadPin(dht->port, dht->pin)) {
                Temp &= ~(1<<(15-i));	
            } else {
                Temp |= 1<<(15-i);
            }
            if (!HAL_GPIO_ReadPin(dht->port, dht->pin)) {
                return;
            }
            isDHT_ReadBits[i] = true
            __HAL_TIM_SET_COUNTER(dht->timer,0);
        }
    }
    if (!isDHT_EndStep) {
        for(i = 0; i < 5; i++) {
            isDHT_BeginSteps[i] = false;
        }
        for(i = 0; i < 33; i++) {
            isDHT_ReadBits[i] = false;
        }
        isDHT_EndStep = true;
    }
}