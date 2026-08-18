#ifndef BRAKELIGHTS_H
#define BRAKELIGHTS_H

typedef enum{
    BRAKELIGHT_OFF,
    BRAKELIGHT_ON
} BrakeLightState_t;

void BrakeLights_Init();
void BrakeLights_Update();
void BrakeLights_Apply();

#endif