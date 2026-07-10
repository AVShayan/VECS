#ifndef HEADLIGHT_H
#define HEADLIGHT_H

typedef enum{
    HEADLIGHT_OFF,
    HEADLIGHT_ON
} HeadlightState_t;

void HeadLights_Init();
void HeadLights_Update();
void HeadLights_Apply();
void setHeadLightState(HeadlightState_t); 

#endif