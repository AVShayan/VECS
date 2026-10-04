#include<Arduino.h>

void GPIO_Init(){
}

void GPIO_WRITE(uint8_t pin , uint8_t state){
    digitalWrite(pin,state);
}

uint8_t GPIO_READ(uint8_t pin){
    return digitalRead(pin);
}