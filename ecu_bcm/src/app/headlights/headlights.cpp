#include<Arduino.h>
#include "drivers/gpio/gpio_driver.h"
#include "app/states/vehicle_state/vehicle_state.h"
#include "app/headlights/headlights.h"

static HeadlightState_t current_state = HEADLIGHT_OFF;
// Headlight Relay is connected to GPIO 8 in STM32.
#define HEADLIGHT_PIN PA7

void HeadLights_Init(){
    pinMode(HEADLIGHT_PIN,OUTPUT);
    current_state = HEADLIGHT_OFF;
}

void setHeadLightState(HeadlightState_t state){
    current_state = state;
}

void HeadLights_Update(){
    // Trigger only when STATE = READY OR DRIVE
    // if(VehicleState_Get() != VEH_READY || VehicleState_Get() != VEH_DRIVE){
    //     current_state = HEADLIGHT_OFF;
    //     return;
    // }
    // The logic here is to not allow headlights to turn on when vehicle is OFF or in LIMP Mode.
}

void HeadLights_Apply(){
    switch (current_state){
        case HEADLIGHT_OFF:
            GPIO_WRITE(HEADLIGHT_PIN,0);
            break;
        
        case HEADLIGHT_ON:
            GPIO_WRITE(HEADLIGHT_PIN,1);
            break;
        default:
            break;
    }
}