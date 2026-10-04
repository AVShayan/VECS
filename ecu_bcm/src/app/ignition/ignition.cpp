#include <Arduino.h>

#include "app/ignition/ignition.h"
#include "app/states/vehicle_state/vehicle_state.h"
#include "drivers/gpio/gpio_driver.h"

#define IGNITION_PIN PB5


// ==================================================
// Ignition State
// ==================================================

static uint8_t remote_ignition_request = 0;
static uint8_t ignition_output = 0;


// ==================================================
// Initialization
// ==================================================

void Ignition_Init()
{
    pinMode(IGNITION_PIN, OUTPUT);

    remote_ignition_request = 0;
    ignition_output = 0;

    GPIO_WRITE(IGNITION_PIN, 0);
}


// ==================================================
// Ignition Update
// ==================================================

void Ignition_Update()
{
    // Vehicle fault → ignition OFF
    if (VehicleState_Get() == VEH_FAULT)
    {
        ignition_output = 0;
        return;
    }

    // BCM output follows TCU remote ignition request
    ignition_output = remote_ignition_request;
}


// ==================================================
// Apply Output
// ==================================================

void Ignition_Apply()
{
    GPIO_WRITE(
        IGNITION_PIN,
        ignition_output
    );
}


// ==================================================
// Remote Ignition Request
// ==================================================

void Ignition_setRemoteRequest(uint8_t request)
{
    remote_ignition_request =
        request ? 1 : 0;
}


// ==================================================
// Get Actual Ignition Output
// ==================================================

uint8_t Ignition_GetOutputState()
{
    return ignition_output;
}