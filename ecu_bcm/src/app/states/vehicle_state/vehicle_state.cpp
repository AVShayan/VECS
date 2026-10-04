#include "app/states/vehicle_state/vehicle_state.h"
#include "app/input_manager/input_manager.h"

static VehicleState_t current_state;

void VehicleState_Init(){
    current_state = VEH_BOOT;
}

// 500ms allocated to initialize all drivers and states (BOOT Mode for BCM)
void VehicleState_Update(){
    static int boot_counter = 0;
    // If bike is off, then transition to OFF state.
    if(!getIgnitionState()){ 
        current_state = VEH_OFF;
        boot_counter = 0;
    }
    // If bike is turned on, then transition to BOOT state.
    else{
        //Transition from OFF -> BOOT
        if(current_state != VEH_READY)
        {
            current_state = VEH_BOOT;
            // Transition from BOOT -> READY
            boot_counter++;
            if(current_state == VEH_BOOT && boot_counter > 500){
                // BCM is ready to function
                current_state = VEH_READY;
            }
        }
    }
}

VehicleState_t VehicleState_Get(){
    return current_state;
}