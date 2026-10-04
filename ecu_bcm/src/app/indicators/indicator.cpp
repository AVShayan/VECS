#include <Arduino.h>
#include "drivers/gpio/gpio_driver.h"
#include "app/states/vehicle_state/vehicle_state.h"
#include "app/indicators/indicator.h"

// GPIO A02 and A04 are connected to the bike's indicators
#define LEFT_INDICATOR  PA2
#define RIGHT_INDICATOR PA4

// Actual flasher output states
static uint8_t left_indicator_state = 0;
static uint8_t right_indicator_state = 0;

// Flasher counters
static uint8_t left_indicator_counter = 0;
static uint8_t right_indicator_counter = 0;

// Local requests
static uint8_t local_left_indicator_request = 0;
static uint8_t local_right_indicator_request = 0;
static uint8_t local_hazard_request = 0;

// Remote TCU request
static uint8_t remote_hazard_request = 0;

void Indicator_Init()
{
    pinMode(LEFT_INDICATOR, OUTPUT);
    pinMode(RIGHT_INDICATOR, OUTPUT);

    left_indicator_state = 0;
    right_indicator_state = 0;

    left_indicator_counter = 0;
    right_indicator_counter = 0;

    local_left_indicator_request = 0;
    local_right_indicator_request = 0;
    local_hazard_request = 0;

    remote_hazard_request = 0;
}

void LeftIndicator_setRequest(uint8_t request)
{
    local_left_indicator_request = request;
}

void RightIndicator_setRequest(uint8_t request)
{
    local_right_indicator_request = request;
}

void HazardIndicator_setRequest(uint8_t request)
{
    local_hazard_request = request;
}

void RemoteHazardIndicator_setRequest(uint8_t request)
{
    remote_hazard_request = request;
}

void Indicator_Update(){
    uint8_t hazard_request =
        local_hazard_request || remote_hazard_request;

    uint8_t left_request =
        local_left_indicator_request || hazard_request;

    uint8_t right_request =
        local_right_indicator_request || hazard_request;

    if (VehicleState_Get() == VEH_OFF)
    {
        left_indicator_state = 0;
        right_indicator_state = 0;

        left_indicator_counter = 0;
        right_indicator_counter = 0;

        return;
    }

    // LEFT flasher
    if (left_request)
    {
        left_indicator_counter++;

        if (left_indicator_counter >= 50)
        {
            left_indicator_state = !left_indicator_state;
            left_indicator_counter = 0;
        }
    }
    else
    {
        left_indicator_state = 0;
        left_indicator_counter = 0;
    }

    // RIGHT flasher
    if (right_request)
    {
        right_indicator_counter++;

        if (right_indicator_counter >= 50)
        {
            right_indicator_state = !right_indicator_state;
            right_indicator_counter = 0;
        }
    }
    else
    {
        right_indicator_state = 0;
        right_indicator_counter = 0;
    }
}

void Indicator_Apply(){
    GPIO_WRITE(LEFT_INDICATOR, left_indicator_state);
    GPIO_WRITE(RIGHT_INDICATOR, right_indicator_state);
}

uint8_t Indicator_GetLeftOutput()
{
    return left_indicator_state;
}

uint8_t Indicator_GetRightOutput()
{
    return right_indicator_state;
}

uint8_t Indicator_IsHazardActive(){
    return local_hazard_request || remote_hazard_request;
}