#ifndef HEADLIGHTS_H
#define HEADLIGHTS_H

#include <stdint.h>

typedef enum {
    HEADLIGHT_OFF = 0,
    HEADLIGHT_HIGH_BEAM
} HeadlightState_t;

void HeadLights_Init(void);
void setHeadLightState(HeadlightState_t state);
void setRemoteHeadLightState(uint8_t percent); // Updated for 0-100% TCU input
void HeadLights_Update(void);
void HeadLights_Apply(void);
uint8_t HeadLights_GetOutputState(void);

#endif