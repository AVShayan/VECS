#include <Arduino.h>
#include "app/input_manager/input_manager.h"
#include "app/headlights/headlights.h"
#include "app/indicators/indicator.h"
#include "app/horn/horn.h"
#include "drivers/gpio/gpio_driver.h"

// The following GPIO pins are assigned as INPUTS:-
static const uint8_t LEFT_INDICATOR_SWITCH  = PA1;
static const uint8_t RIGHT_INDICATOR_SWITCH = PA3;
static const uint8_t HAZARD_SWITCH          = PA6;
static const uint8_t HEADLIGHT_SWITCH       = PA5;
//static const uint8_t HORN_SWITCH            = PA8;

// IGNITION_SWITCH is a signal for Bike State (ON | OFF)
//static const uint8_t IGNITION_SWITCH = PA6;

void InputManager_Init()
{
    pinMode(LEFT_INDICATOR_SWITCH, INPUT_PULLUP);
    pinMode(RIGHT_INDICATOR_SWITCH, INPUT_PULLUP);
    pinMode(HAZARD_SWITCH, INPUT_PULLUP);
    pinMode(HEADLIGHT_SWITCH, INPUT_PULLUP);
//    pinMode(HORN_SWITCH, INPUT_PULLUP);
//    pinMode(IGNITION_SWITCH, INPUT_PULLUP);
}


uint8_t Debounce(uint8_t raw_state)
{
    static uint8_t last_state = HIGH;
    static uint8_t stable_state = HIGH;
    static uint8_t counter = 0;

    if (raw_state != last_state)
    {
        last_state = raw_state;
        counter = 0;
    }
    else
    {
        if (counter < 3)
            counter++;

        if (counter >= 3)
            stable_state = last_state;
    }

    return stable_state;
}


void InputManager_Update()
{

    if (GPIO_READ(HAZARD_SWITCH) == LOW)
    {
        HazardIndicator_setRequest(1);
    }
    else
    {
        if (GPIO_READ(LEFT_INDICATOR_SWITCH) == LOW)
        {
            LeftIndicator_setRequest(1);
        }
        else
        {
            LeftIndicator_setRequest(0);
        }

        if (GPIO_READ(RIGHT_INDICATOR_SWITCH) == LOW)
        {
            RightIndicator_setRequest(1);
        }
        else
        {
            RightIndicator_setRequest(0);
        }
    }


    uint8_t headlight_switch =
        Debounce(GPIO_READ(HEADLIGHT_SWITCH));

    if (headlight_switch == LOW)
    {
        setHeadLightState(HEADLIGHT_ON);
    }
    else
    {
        setHeadLightState(HEADLIGHT_OFF);
    }


    // if (GPIO_READ(HORN_SWITCH) == LOW)
    // {
    //     Horn_setRequest(1);
    // }
    // else
    // {
    //     Horn_setRequest(0);
    // }
}