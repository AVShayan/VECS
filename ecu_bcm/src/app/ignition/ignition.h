#ifndef IGNITION_H
#define IGNITION_H

#include <Arduino.h>

void Ignition_Init();

void Ignition_Update();

void Ignition_Apply();

// void Ignition_setRequest(
//     uint8_t request
// );

void Ignition_setRemoteRequest(
    uint8_t request
);

uint8_t Ignition_GetOutputState();

#endif