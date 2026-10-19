#include "can_driver.h"
#include <Arduino.h>
#include <STM32_CAN.h>

// We use STM32 built-in bxCAN
// PB8 = CAN RX
// PB9 = CAN TX

#define CAN_BAUDRATE 500000
#define CAN_MAX_DATA 8

STM32_CAN Can1(PB_8, PB_9); 

// Initialzing the CAN Driver
void CAN_Driver_Init(){
    // Force enable the Alternate Function clock gate
    __HAL_RCC_AFIO_CLK_ENABLE();
    // Explicitly command the HAL to move CAN1 pins to Remap position 2 (PB8/PB9)
    __HAL_AFIO_REMAP_CAN1_2();
    Serial.println("Initializing CAN...");

    Can1.setBaudRate(500000);
    Can1.begin();
    Serial.println("CAN Initialized.");
}

bool CAN_Driver_Send(uint32_t id, uint8_t *data, uint8_t len){
    // Check for valid parameters
    if (data == nullptr || len > CAN_MAX_DATA || id > 0x7FF)
        return false;

    CAN_message_t txMsg = {};

    txMsg.id = id;
    txMsg.len = len;

    for (uint8_t i = 0; i < len; i++){
        txMsg.buf[i] = data[i];
    }

    bool msgSent = Can1.write(txMsg);
    Serial.print("CAN write result: ");
    Serial.println(msgSent ? "SUCCESS" : "FAILED");
    if (msgSent){
        Serial.print("CAN TX: ID=0x");
        Serial.print(id, HEX);
        Serial.print(" DLC=");
        Serial.print(len);
        Serial.print(" DATA=");
        for (uint8_t i = 0; i < len; i++){
            if (data[i] < 0x10)
                Serial.print("0");
            Serial.print(data[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
    return msgSent;
}

bool CAN_Driver_Receive(uint32_t *id, uint8_t *data, uint8_t *len){
    // Check for valid parameters
    if (id == nullptr || data == nullptr || len == nullptr)
        return false;

    CAN_message_t rxMsg;

    if (!Can1.read(rxMsg))
        return false;

    // Accept standard 11-bit data frames only.
    if (rxMsg.flags.extended || rxMsg.flags.remote)
        return false;

    if (rxMsg.len > CAN_MAX_DATA)
        return false;

    *id = rxMsg.id;
    *len = rxMsg.len;

    for (uint8_t i = 0; i < rxMsg.len; i++){
        data[i] = rxMsg.buf[i];
    }

    Serial.print("CAN RX: ID=0x");
    Serial.print(*id, HEX);
    Serial.print(" DLC=");
    Serial.print(*len);
    Serial.print(" DATA=");

    for (uint8_t i = 0; i < *len; i++){
        if (data[i] < 0x10)
            Serial.print("0");

        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
    return true;
}