#include<Arduino.h>

#define LEFT_INDICATOR_PIN PA2
#define RIGHT_INDICATOR_PIN PA4

void GPIO_Init(){
    pinMode(LEFT_INDICATOR_PIN,OUTPUT);
    pinMode(RIGHT_INDICATOR_PIN,OUTPUT);
}

void GPIO_WRITE(uint8_t pin , uint8_t state){
    digitalWrite(pin,state);
}

uint8_t GPIO_READ(uint8_t pin){
    return digitalRead(pin);
}