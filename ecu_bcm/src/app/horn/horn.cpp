#include <Arduino.h>

#include "app/horn/horn.h"
#include "app/states/vehicle_state/vehicle_state.h"
#include "drivers/gpio/gpio_driver.h"
#include "app/input_manager/input_manager.h"

#define HORN_PIN PA7


// ==================================================
// Horn State
// ==================================================

static uint8_t local_horn_request = 0;
static uint8_t remote_horn_request = 0;

static uint8_t horn_output = 0;


// ==================================================
// Initialization
// ==================================================

void Horn_Init(){
    pinMode(HORN_PIN, OUTPUT);

    local_horn_request = 0;
    remote_horn_request = 0;
    horn_output = 0;
}


// ==================================================
// Horn Update
// ==================================================

void Horn_Update(){
    // ----------------------------------------------
    // Vehicle OFF / FAULT
    //
    // Local horn is disabled.
    // Remote horn is still allowed because the
    // TCU needs to be able to Ping My Bike while
    // the scooter is OFF.
    // ----------------------------------------------

    if (VehicleState_Get() == VEH_FAULT)
    {
        horn_output = 0;
        return;
    }


    // ----------------------------------------------
    // Remote horn has priority
    //
    // TCU directly controls the horn timing.
    // ----------------------------------------------

    if (remote_horn_request)
    {
        horn_output = 1;
        return;
    }


    // ----------------------------------------------
    // Local horn
    //
    // Local horn only works when the vehicle is
    // active.
    // ----------------------------------------------

    if (VehicleState_Get() == VEH_OFF)
    {
        horn_output = 0;
        return;
    }


    horn_output =
        local_horn_request ? 1 : 0;
}


// ==================================================
// Apply Output
// ==================================================

void Horn_Apply(){
    GPIO_WRITE(
        HORN_PIN,
        horn_output
    );
}


// ==================================================
// Local Horn Request
// ==================================================

void Horn_setRequest(uint8_t request){
    local_horn_request =
        request ? 1 : 0;
}


// ==================================================
// Remote Horn Request
// ==================================================

void Horn_setRemoteRequest(uint8_t request){
    remote_horn_request =
        request ? 1 : 0;
}


// ==================================================
// Get Actual Horn Output
// ==================================================

uint8_t Horn_GetOutputState(){
    return horn_output;
}