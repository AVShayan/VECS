#include<Arduino.h>
#include "app/input_manager/input_manager.h"
#include "app/headlights/headlights.h"
#include "app/indicators/indicator.h"
#include "app/horn/horn.h"
#include "drivers/gpio/gpio_driver.h"

// The following GPIO pins are assigned as INPUTS:-
static const uint8_t LEFT_INDICATOR_SWITCH = PA0;
static const uint8_t RIGHT_INDICATOR_SWITCH = PA1;
static const uint8_t HEADLIGHT_SWITCH = PA2;
static const uint8_t HORN_SWITCH = PA3;
// IGNITION_SWITCH is a signal for BIKE STATE (ON | OFF)
static const uint8_t IGNITION_SWITCH = PA4;

void InputManager_Init(){
    pinMode(LEFT_INDICATOR_SWITCH,INPUT_PULLUP);
    pinMode(RIGHT_INDICATOR_SWITCH,INPUT_PULLUP);
    pinMode(HEADLIGHT_SWITCH,INPUT_PULLUP);
    pinMode(HORN_SWITCH,INPUT_PULLUP);
    pinMode(IGNITION_SWITCH,INPUT_PULLUP);
}

void InputManager_Update(){
    if(GPIO_READ(LEFT_INDICATOR_SWITCH) == LOW){
        LeftIndicator_setRequest(1);
    }
    else
        LeftIndicator_setRequest(0);

    if(GPIO_READ(RIGHT_INDICATOR_SWITCH) == LOW){
        RightIndicator_setRequest(1);
    }
    else
        RightIndicator_setRequest(0);

    if(GPIO_READ(HEADLIGHT_SWITCH) == LOW)
        setHeadLightState(HEADLIGHT_LOW);
    else
        setHeadLightState(HEADLIGHT_OFF);

    if(GPIO_READ(HORN_SWITCH) == LOW)
        Horn_setRequest(1);
    else
        Horn_setRequest(0);
}

bool getIgnitionState(){
    return (GPIO_READ(IGNITION_SWITCH) == LOW) ? true : false;
}