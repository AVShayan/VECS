#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include<stdint.h>

#define GPIO_LOW 0
#define GPIO_HIGH 1

void GPIO_Init(void);

void GPIO_WRITE(uint8_t pin , uint8_t state);

uint8_t GPIO_READ(uint8_t pin);

#endif