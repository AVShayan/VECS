#include<Arduino.h>

// GPIO A02 and A04 are comnnected to the Bike's Indicators
#define LEFT_INDICATOR PA2
#define RIGHT_INDICATOR PA4
//#define HORN_PIN PA7
#define HEADLIGHT_PIN PA8

void GPIO_Init(){
    // Pin Configurations for Relays and MOSFETs
    pinMode(LEFT_INDICATOR,OUTPUT);
    pinMode(RIGHT_INDICATOR,OUTPUT);
//    pinMode(HORN_PIN,OUTPUT);
    pinMode(HEADLIGHT_PIN,OUTPUT);
}

void GPIO_WRITE(uint8_t pin , uint8_t state){
    digitalWrite(pin,state);
}

uint8_t GPIO_READ(uint8_t pin){
    return digitalRead(pin);
}