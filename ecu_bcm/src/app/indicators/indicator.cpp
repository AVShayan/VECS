#include<Arduino.h>
#include "drivers/gpio/gpio_driver.h"
#include "app/states/vehicle_state/vehicle_state.h"

// GPIO A02 and A04 are comnnected to the Bike's Indicators
#define LEFT_INDICATOR PA2
#define RIGHT_INDICATOR PA4

static uint8_t left_indicator_state = 0;
static uint8_t left_indicator_counter = 0;
static uint8_t right_indicator_state = 0;
static uint8_t right_indicator_counter = 0;

static uint8_t left_indicator_request = 1;
static uint8_t right_indicator_request = 1;

void Indicator_Init(){
    pinMode(LEFT_INDICATOR,OUTPUT);
    pinMode(RIGHT_INDICATOR,OUTPUT);
    // Flasher Relay Variables
    left_indicator_state = 0;
    left_indicator_counter = 0;
    right_indicator_state = 0;
    right_indicator_counter = 0;
    left_indicator_request = 0;
    right_indicator_request = 0;
}

void LeftIndicator_setRequest(uint8_t request){
    left_indicator_request = request;
}

void RightIndicator_setRequest(uint8_t request){
    right_indicator_request = request;
}

//Hazard Indicator is a special case where both left and right indicators are turned on simultaneously.
void HazardIndicator_setRequest(uint8_t request){
    left_indicator_request = request;
    right_indicator_request = request;
}

void Indicator_Update(){

    // Toggle Indicators only for READY & DRIVE States
    // if(VehicleState_Get() == VEH_OFF || VehicleState_Get() == VEH_BOOT || VehicleState_Get() == VEH_FAULT){
    //     left_indicator_request = 0;
    //     right_indicator_request = 0;
    //     return;
    // }

    // Flasher Relay Logic
    if(left_indicator_request){
        left_indicator_counter++;
        if(left_indicator_counter >= 50){
            left_indicator_state = !left_indicator_state;
            left_indicator_counter = 0;
        }
    }
    else{
        left_indicator_state = 0;
        left_indicator_counter = 0;
    }
    if(right_indicator_request){
        right_indicator_counter++;
        if(right_indicator_counter >= 50){
            right_indicator_state = !right_indicator_state;
            right_indicator_counter = 0;
        }
    }
    else{
        right_indicator_state = 0;
        right_indicator_counter = 0;
    }
}

void Indicator_Apply(){
    // Drive the indicator relays
    GPIO_WRITE(LEFT_INDICATOR,left_indicator_state);
    GPIO_WRITE(RIGHT_INDICATOR,right_indicator_state);
}