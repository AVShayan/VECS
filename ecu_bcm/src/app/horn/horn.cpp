#include<Arduino.h>
#include "app/horn/horn.h"
#include "app/states/vehicle_state/vehicle_state.h"
#include "drivers/gpio/gpio_driver.h"

#define HORN_PIN PA8
static uint8_t horn_request = 0;

void Horn_Init(){
    pinMode(HORN_PIN,OUTPUT);
    horn_request = 0;
}

void Horn_Update(){
    // Logic here is to not trigger Horn when vehicle is OFF or in LIMP Mode
    // if(VehicleState_Get() != VEH_READY || VehicleState_Get() != VEH_DRIVE){
    //     horn_request = 0;
    //     return;
    // }
}
// Simulates a horn request
void Horn_Apply(){
    GPIO_WRITE(HORN_PIN,horn_request);
}

void Horn_setRequest(uint8_t request){
    horn_request = request;
}