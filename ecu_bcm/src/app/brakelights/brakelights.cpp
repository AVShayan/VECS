#include<Arduino.h>
#include "drivers/gpio/gpio_driver.h"
#include "app/states/vehicle_state/vehicle_state.h"
#include "app/brakelights/brakelights.h"

static BrakeLightState_t current_state = BRAKELIGHT_OFF;
// Brakelight Relay is connected to GPIO 12 in STM32.
static const uint8_t BRAKELIGHT_PIN = PA12;


void BrakeLights_Init(){
    current_state = BRAKELIGHT_OFF;
}

void setBrakeLightState(BrakeLightState_t state){
    current_state = state;
}

void BrakeLights_Update(){
    // Trigger only when STATE = READY OR DRIVE
    // if(VehicleState_Get() != VEH_READY && VehicleState_Get() != VEH_DRIVE){
    //     current_state = BRAKELIGHT_OFF;
    //     return;
    // }
    // The logic here is to not allow brake lights to turn on when vehicle is OFF or in LIMP Mode.
}

void BrakeLights_Apply(){
    switch (current_state){
        case BRAKELIGHT_OFF:
            GPIO_WRITE(BRAKELIGHT_PIN,0);
            break;
        
        case BRAKELIGHT_ON:
            GPIO_WRITE(BRAKELIGHT_PIN,1);
            break;
        default:
            break;
    }
}