/*#include "diag_service.h"
#include "app/diagnostics/dtc.h"
#include "app/states/vehicle_state/vehicle_state.h"

static char RX_command;

void DiagService_Init(){
    Serial.begin(115200);
}

void DiagService_Update(){
    while(Serial.available()){
        char c = Serial.read();
        if(c == '\n'){
            processCommand(RX_command);
        }else{
            RX_command += c;
        }
    }
}

void processCommand(String cmd){
    if(cmd == 'HELP'){
        Serial.println("-------------  VECS BCM  ------------");
        Serial.println("GET_STATE");
        Serial.println("GET_DTC");
        Serial.println("CLEAR_DTC");
    }
    else if(cmd == "GET_STATE"){
        Serial.println(VehicleState_Get());
    }
    else if(cmd == 'GET_DTC'){
        DTC_PrintAll();
    }
    else if(cmd == 'CLEAR_DTC'){
        DTC_ClearAll();
    }
}
    */