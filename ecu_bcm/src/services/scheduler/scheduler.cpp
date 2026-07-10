#include <cstdint>
#include "app/input_manager/input_manager.h"
#include "app/states/vehicle_state/vehicle_state.h"
#include "app/indicators/indicator.h"
#include "app/headlights/headlights.h"
#include "app/horn/horn.h"

static uint32_t tick_1ms = 0;
static uint32_t tick_10ms = 0;

void Scheduler_Init(){
    tick_1ms = 0;
    tick_10ms = 0;
}

void Scheduler_Tick(){
    // 1ms counter
    tick_1ms++;
    if(tick_1ms % 10 == 0)
        // Signal to run 10ms tasks
        tick_10ms = 1;
}

void Scheduler_Run(){
    
    // Run 1ms tasks here

    if(tick_10ms){
        // Run 10ms tasks
        InputManager_Update();
        VehicleState_Update();
        Indicator_Update();
        Indicator_Apply();
        HeadLights_Update();
        HeadLights_Apply();
        Horn_Update();
        Horn_Apply();
    }
}
