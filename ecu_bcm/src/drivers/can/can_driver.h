#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include <Arduino.h>

void CAN_Driver_Init();

bool CAN_Driver_Send(
    uint32_t id,
    uint8_t *data,
    uint8_t len
);

bool CAN_Driver_Receive(
    uint32_t *id,
    uint8_t *data,
    uint8_t *len
);

#endif