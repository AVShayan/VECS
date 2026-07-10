/*
include "app/diagnostics/dtc.h"

void DTC_Init(){
    DTC_NONE = 0;
    DTC_MAX = 5; 
}

void DTC_Set(DTC_Code_t){
    dtc_status[code] = 1;
}

void DTC_Clear(DTC_Code_t){
    dtc_status[code] = 0;
}

uint8_t DTC_isActive(DTC_Code_t){
    return dtc_status[DTC_Code_t];
}

void DTC_ClearAll(){
    static uint8_t counter = 0;
    // Clear all the active DTCs
    while(counter < DTC_MAX){
        if(dtc_status[counter] == 1)
            dtc_status[counter] = 0;
        counter++;
    }
    Serial.println("All active DTCs Cleared!");
}
void DTC_PrintAll(){
    static uint8_t counter = 0;
    // Log all the active DTCs
    while(counter < DTC_MAX){
        if(dtc_status[counter] == 1)
            Serial.println(counter);
        counter++;
    }
}*/