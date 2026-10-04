#ifndef HORN_H
#define HORN_H

#include <Arduino.h>

void Horn_Init();

void Horn_Update();

void Horn_Apply();

void Horn_setRequest(
    uint8_t request
);

void Horn_setRemoteRequest(
    uint8_t request
);

uint8_t Horn_GetOutputState();

#endif