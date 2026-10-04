#include <Arduino.h>

#include "app/input_manager/input_manager.h"
#include "app/headlights/headlights.h"
#include "app/indicators/indicator.h"
#include "app/horn/horn.h"
#include "drivers/gpio/gpio_driver.h"

static const uint8_t LEFT_INDICATOR_SWITCH  = PA1;
static const uint8_t RIGHT_INDICATOR_SWITCH = PA3;
static const uint8_t HORN_SWITCH            = PA5;
static const uint8_t HEADLIGHT_SWITCH       = PA6;
static const uint8_t HAZARD_SWITCH          = PA8;
static const uint8_t IGNITION_SWITCH        = PB1;

// Solder connections for HAZARD, IGNITION, RX and TX in MCU -> DONE

void InputManager_Init(){
    pinMode(LEFT_INDICATOR_SWITCH, INPUT_PULLUP);
    pinMode(RIGHT_INDICATOR_SWITCH, INPUT_PULLUP);
    pinMode(HAZARD_SWITCH, INPUT_PULLUP);
    pinMode(HEADLIGHT_SWITCH, INPUT_PULLUP);
    pinMode(HORN_SWITCH, INPUT_PULLUP);
    pinMode(IGNITION_SWITCH,INPUT_PULLDOWN);
}

uint8_t DebounceHeadlight(uint8_t raw_state)
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

uint8_t DebounceHorn(uint8_t raw_state)
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
        if (counter < 5)
            counter++;

        if (counter >= 5)
            stable_state = last_state;
    }

    return stable_state;
}

void InputManager_Update()
{
    // =================================================
    // HAZARD / INDICATORS
    // =================================================

    if (GPIO_READ(HAZARD_SWITCH) == LOW)
    {
        HazardIndicator_setRequest(1);
    }
    else
    {
        HazardIndicator_setRequest(0);

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
        DebounceHeadlight(GPIO_READ(HEADLIGHT_SWITCH));

    if (headlight_switch == LOW)
    {
        setHeadLightState(HEADLIGHT_HIGH_BEAM);
    }
    else
    {
        setHeadLightState(HEADLIGHT_OFF);
    }

    uint8_t horn_switch =
        DebounceHorn(GPIO_READ(HORN_SWITCH));

    if (horn_switch == LOW)
    {
        Horn_setRequest(1);
    }
    else
    {
        Horn_setRequest(0);
    }
}

// Since IGNITION_SWITCH is a pull down input, it is normally OFF.
// Requires a 5ms debounce to avoid false positives from switch bounce.
bool getIgnitionState(){
    static bool last_raw_state = false;
    static bool stable_state = false;
    static uint8_t counter = 0;

    bool raw_state = GPIO_READ(IGNITION_SWITCH);

    if (raw_state != last_raw_state){
        last_raw_state = raw_state;
        counter = 0;
    }
    else{
    if (counter < 5)
            counter++;

    if (counter >= 5){
            stable_state = last_raw_state;
        }
    }
    return stable_state;
}