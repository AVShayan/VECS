#include "can_driver.h"
#include <Arduino.h>

#define UART_START_BYTE 0xAA
#define UART_MAX_DATA   8
#define RX_TIMEOUT_MS   50  // Timeout to prevent state machine lockout

static HardwareSerial *TCU_UART = nullptr;

enum UART_RX_STATE
{
    WAIT_START,
    READ_ID_LOW,
    READ_ID_HIGH,
    READ_DLC,
    READ_DATA
};

static UART_RX_STATE rxState = WAIT_START;

static uint8_t rxIdLow = 0;
static uint8_t rxIdHigh = 0;
static uint8_t rxDlc = 0;
static uint8_t rxData[UART_MAX_DATA];
static uint8_t rxDataIndex = 0;
static uint32_t lastByteTime = 0;

void CAN_Driver_Init()
{
    // Safely instantiate hardware serial during setup execution
    if (TCU_UART == nullptr){
        TCU_UART = new HardwareSerial(PB11, PB10); // RX=PB11, TX=PB10 (USART3)
    }
    TCU_UART->begin(115200);
    delay(100);
}

bool CAN_Driver_Send(uint32_t id, uint8_t *data, uint8_t len)
{
    if (TCU_UART == nullptr || len > UART_MAX_DATA || id > 0x7FF)
        return false;

    TCU_UART->write(UART_START_BYTE);
    TCU_UART->write((uint8_t)(id & 0xFF));
    TCU_UART->write((uint8_t)((id >> 8) & 0xFF));
    TCU_UART->write(len);

    for (uint8_t i = 0; i < len; i++)
    {
        TCU_UART->write(data[i]);
    }

    TCU_UART->flush();

    Serial.print("UART TX: ID=0x");
    Serial.print(id, HEX);
    Serial.print(" DLC=");
    Serial.print(len);
    Serial.print(" DATA=");

    for (uint8_t i = 0; i < len; i++)
    {
        if (data[i] < 0x10) Serial.print("0");
        Serial.print(data[i], HEX);
        Serial.print(" ");
    }
    Serial.println();

    return true;
}

bool CAN_Driver_Receive(uint32_t *id, uint8_t *data, uint8_t *len)
{
    if (TCU_UART == nullptr || !id || !data || !len)
        return false;

    // Reset state machine if frame construction timed out mid-stream
    if (rxState != WAIT_START && (millis() - lastByteTime > RX_TIMEOUT_MS))
    {
        rxState = WAIT_START;
    }

    while (TCU_UART->available())
    {
        uint8_t byte = TCU_UART->read();
        lastByteTime = millis();

        switch (rxState)
        {
            case WAIT_START:
                if (byte == UART_START_BYTE)
                {
                    rxState = READ_ID_LOW;
                }
                break;

            case READ_ID_LOW:
                rxIdLow = byte;
                rxState = READ_ID_HIGH;
                break;

            case READ_ID_HIGH:
                rxIdHigh = byte;
                rxState = READ_DLC;
                break;

            case READ_DLC:
                rxDlc = byte;
                if (rxDlc > UART_MAX_DATA)
                {
                    rxState = WAIT_START;
                }
                else if (rxDlc == 0)
                {
                    *id = ((uint32_t)rxIdHigh << 8) | rxIdLow;
                    *len = 0;
                    rxState = WAIT_START;
                    return true;
                }
                else
                {
                    rxDataIndex = 0;
                    rxState = READ_DATA;
                }
                break;

            case READ_DATA:
                rxData[rxDataIndex++] = byte;
                if (rxDataIndex >= rxDlc)
                {
                    *id = ((uint32_t)rxIdHigh << 8) | rxIdLow;
                    *len = rxDlc;

                    for (uint8_t i = 0; i < rxDlc; i++)
                    {
                        data[i] = rxData[i];
                    }

                    rxState = WAIT_START;
                    return true;
                }
                break;
        }
    }

    return false;
}