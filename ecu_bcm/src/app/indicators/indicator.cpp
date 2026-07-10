#include<Arduino.h>
#include "drivers/gpio/gpio_driver.h"
#include "app/states/vehicle_state/vehicle_state.h"

// GPIO A02 and A04 are comnnected to the Bike's Indicators
#define LEFT_INDICATOR PA5
#define RIGHT_INDICATOR PA6

static uint8_t left_indicator_state = 0;
static uint8_t left_indicator_counter = 0;
static uint8_t right_indicator_state = 0;
static uint8_t right_indicator_counter = 0;

static uint8_t left_indicator_request = 0;
static uint8_t right_indicator_request = 0;

void Indicator_Init(){
    left_indicator_state = 0;
    left_indicator_counter = 0;
    right_indicator_state = 0
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

void Indicator_Update(){

    // Toggle Indicators only for READY & DRIVE States
    if(VehicleState_Get() == VEH_OFF || VehicleState_Get() == VEH_BOOT || VehicleState_Get() == VEH_FAULT){
        left_indicator_request = 0;
        right_indicator_request = 0;
        return;
    }

    if(left_indicator_request || right_indicator_request){
        // counter++ is called every 10ms by scheduler
        left_indicator_counter++;
        // We switch the state of the light every 500ms (ie 50 counters)
        if(left_indicator_counter >= 50)
            left_indicator_state= !left_indicator_state;

        // counter++ is called every 10ms by scheduler
        right_indicator_counter++;
        // We switch the state of the light every 500ms (ie 50 counters)
        if(right_indicator_counter >= 50)
            right_indicator_state= !right_indicator_state;
        }
}

void Indicator_Apply(){
    // Drive the indicator relays
    GPIO_WRITE(LEFT_INDICATOR,left_indicator_request);
    GPIO_WRITE(RIGHT_INDICATOR,right_indicator_request);
}