#include<Arduino.h>
#include "services/scheduler/scheduler.h"
#include "app/input_manager/input_manager.h"
#include "app/states/vehicle_state/vehicle_state.h"
#include "app/indicators/indicator.h"
#include "app/headlights/headlights.h"
#include "app/horn/horn.h"
#include "drivers/gpio/gpio_driver.h"

void setup(){
  Scheduler_Init();
  GPIO_Init();
  InputManager_Init();
  VehicleState_Init();
  Indicator_Init();
  HeadLights_Init();
  Horn_Init();
}

void loop(){
  // Simulate 1ms ticks
  delay(1);
  Scheduler_Tick();
  Scheduler_Run();
}