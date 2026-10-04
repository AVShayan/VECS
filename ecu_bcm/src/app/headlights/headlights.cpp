#include <Arduino.h>
#include "drivers/gpio/gpio_driver.h"
#include "app/states/vehicle_state/vehicle_state.h"
#include "app/headlights/headlights.h"

static HeadlightState_t local_state   = HEADLIGHT_OFF;
static uint8_t remote_brightness_pct  = 0;  // 0% to 100% from TCU
static uint8_t output_duty_cycle      = 0;  // Final 8-bit PWM value (0 - 255)
static uint8_t prev_duty_cycle        = 255; // For change-detection printing

// MOSFET PWM Control Pin
#define HEADLIGHT_PIN PB6

// Standard Duty Cycle Definitions
#define PWM_DUTY_OFF       0
#define PWM_DUTY_HIGH_BEAM 255  // 100% brightness

// Debug Timing
static uint32_t last_debug_time = 0;
#define DEBUG_INTERVAL_MS 500  // Print status every 500ms

void HeadLights_Init(){
    pinMode(HEADLIGHT_PIN, OUTPUT);
    
    // Configure PWM frequency to 1kHz for clean MOSFET dimming
    analogWriteFrequency(1000);

    local_state          = HEADLIGHT_OFF;
    remote_brightness_pct = 0;
    output_duty_cycle     = 0;

    // Ensure MOSFET starts in OFF state
    analogWrite(HEADLIGHT_PIN, PWM_DUTY_OFF);
}

// Local switch handler: Toggles ONLY between OFF (0%) and HIGH_BEAM (100%)
void setHeadLightState(HeadlightState_t state){
    if (state != HEADLIGHT_OFF){
        local_state = HEADLIGHT_HIGH_BEAM;
    }
    else{
        local_state = HEADLIGHT_OFF;
    }
}

// TCU Remote handler: Accepts direct brightness percentage (0 - 100%)
void setRemoteHeadLightState(uint8_t percent){
    if (percent > 100){
        percent = 100;
    }
    remote_brightness_pct = percent;
}

void HeadLights_Update(){
    uint8_t remote_duty =
        map(remote_brightness_pct, 0, 100, 0, 255);

    uint8_t local_duty = 0;

    // Local physical switch is only allowed when vehicle is ON.
    if (VehicleState_Get() != VEH_OFF)
    {
        local_duty =
            (local_state == HEADLIGHT_HIGH_BEAM)
                ? PWM_DUTY_HIGH_BEAM
                : PWM_DUTY_OFF;
    }

    // Remote TCU headlight remains available even when vehicle is OFF.
    if (local_duty >= remote_duty)
        output_duty_cycle = local_duty;
    else
        output_duty_cycle = remote_duty;
}

// Applies PWM to MOSFET Gate and handles Serial Debug prints
void HeadLights_Apply(){
    // Write hardware PWM signal to PB6
    analogWrite(HEADLIGHT_PIN, output_duty_cycle);

    uint32_t now = millis();
    bool state_changed = (output_duty_cycle != prev_duty_cycle);

    // Print immediately on state change, or periodically every 500ms
    if (state_changed || (now - last_debug_time >= DEBUG_INTERVAL_MS)){
        last_debug_time = now;
        prev_duty_cycle = output_duty_cycle;

        uint8_t active_pct = map(output_duty_cycle, 0, 255, 0, 100);
    }
}

// Returns current active brightness percentage (0 - 100%) for telemetry
uint8_t HeadLights_GetOutputState(){
    return map(output_duty_cycle, 0, 255, 0, 100);
}