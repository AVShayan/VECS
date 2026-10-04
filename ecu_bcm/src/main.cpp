#include<Arduino.h>
#include "services/scheduler/scheduler.h"
#include "app/input_manager/input_manager.h"
#include "services/can/can_service.h"
#include "drivers/can/can_driver.h"
#include "app/states/vehicle_state/vehicle_state.h"
#include "app/indicators/indicator.h"
#include "app/headlights/headlights.h"
#include "app/horn/horn.h"
#include "app/ignition/ignition.h"
#include "drivers/gpio/gpio_driver.h"

void setup(){
  Serial.begin(115200);
  Scheduler_Init();
  CAN_Driver_Init();
  CAN_Service_Init();
  GPIO_Init();
  VehicleState_Init();
  InputManager_Init();
  Indicator_Init();
  HeadLights_Init();
  Horn_Init();
  Ignition_Init();
  pinMode(PC13,OUTPUT);
  digitalWrite(PC13,LOW);
}

void loop(){
  // Simulate 1ms ticks
  delay(1);
  Scheduler_Tick();
  Scheduler_Run();
}