#include <Arduino.h>

#include "can_service.h"
#include "drivers/can/can_driver.h"

#include "app/headlights/headlights.h"
#include "app/indicators/indicator.h"
#include "app/horn/horn.h"
#include "app/ignition/ignition.h"
#include "app/input_manager/input_manager.h"

static uint32_t lastTransmitTime = 0;

// BCM Status counter
// 3 bits: 0-7
static uint8_t bcmStatusCounter = 0;


// ==================================================
// BCM_Status Payload Packing
// CAN ID: 0x100
// DLC: 2
//
// BYTE 0
// Bit 0 = LeftIndicator
// Bit 1 = RightIndicator
// Bit 2 = HazardActive
// Bit 3 = HeadlightsActive
// Bit 4 = HornActive
// Bit 5 = IgnitionState
// Bit 6 = BCMFault
// Bit 7 = Reserved
//
// BYTE 1
// Bits 0-2 = BCM_StatusCounter
// Bits 3-7 = Reserved
// ==================================================

static void CAN_Pack_BCM_Status(
    bool leftIndicator,
    bool rightIndicator,
    bool hazardActive,
    bool headlightsActive,
    bool hornActive,
    bool ignitionState,
    bool bcmFault,
    uint8_t counter,
    uint8_t *payload
)
{
    // Clear both bytes
    payload[0] = 0;
    payload[1] = 0;


    // ----------------------------------------------
    // BYTE 0
    // ----------------------------------------------

    if (leftIndicator)
        payload[0] |= (1 << 0);

    if (rightIndicator)
        payload[0] |= (1 << 1);

    if (hazardActive)
        payload[0] |= (1 << 2);

    if (headlightsActive)
        payload[0] |= (1 << 3);

    if (hornActive)
        payload[0] |= (1 << 4);

    if (ignitionState)
        payload[0] |= (1 << 5);

    if (bcmFault)
        payload[0] |= (1 << 6);


    // ----------------------------------------------
    // BYTE 1
    // ----------------------------------------------

    payload[1] |= (counter & 0x07);
}


// ==================================================
// TCU_Request Decoder
// CAN ID: 0x200
// DLC: 1
//
// Bit 0     = RemoteHeadlightRequest
// Bit 1     = RemoteHazardRequest
// Bit 2     = RemoteHornRequest
// Bit 3     = RemoteIgnitionRequest
// Bits 4-6  = TCU_RequestCounter
// Bit 7     = Reserved
// ==================================================

static void CAN_Process_TCU_Request(
    const uint8_t *data
)
{
    Serial.print("TCU REQUEST DATA = 0x");
    Serial.println(data[0], HEX);


    // ----------------------------------------------
    // Decode request
    // ----------------------------------------------

    bool remoteHeadlightRequest =
        (data[0] & (1 << 0)) != 0;

    bool remoteHazardRequest =
        (data[0] & (1 << 1)) != 0;

    bool remoteHornRequest =
        (data[0] & (1 << 2)) != 0;

    bool remoteIgnitionRequest =
        (data[0] & (1 << 3)) != 0;

    uint8_t tcuRequestCounter =
        (data[0] >> 4) & 0x07;


    // ----------------------------------------------
    // Debug
    // ----------------------------------------------

    Serial.print(
        "Remote Headlight Request: "
    );

    Serial.println(
        remoteHeadlightRequest
            ? "ON"
            : "OFF"
    );


    Serial.print(
        "Remote Hazard Request: "
    );

    Serial.println(
        remoteHazardRequest
            ? "ON"
            : "OFF"
    );


    Serial.print(
        "Remote Horn Request: "
    );

    Serial.println(
        remoteHornRequest
            ? "ON"
            : "OFF"
    );


    Serial.print(
        "Remote Ignition Request: "
    );

    Serial.println(
        remoteIgnitionRequest
            ? "ON"
            : "OFF"
    );


    Serial.print(
        "TCU Counter: "
    );

    Serial.println(
        tcuRequestCounter
    );


    // =================================================
    // REMOTE HEADLIGHT
    // =================================================

    if (remoteHeadlightRequest)
    {
        Serial.println(
            "ACTION: Remote HEADLIGHT ON"
        );

        setRemoteHeadLightState(100);
    }
    else
    {
        Serial.println(
            "ACTION: Remote HEADLIGHT OFF"
        );

        setRemoteHeadLightState(0);
    }


    // =================================================
    // REMOTE HAZARD
    // =================================================

    RemoteHazardIndicator_setRequest(
        remoteHazardRequest
    );


    // =================================================
    // REMOTE HORN
    // =================================================

    Horn_setRemoteRequest(
        remoteHornRequest
    );

    Serial.print(
        "ACTION: Remote HORN "
    );

    Serial.println(
        remoteHornRequest
            ? "ON"
            : "OFF"
    );


    // =================================================
    // REMOTE IGNITION
    // =================================================

    Ignition_setRemoteRequest(
        remoteIgnitionRequest
    );

    Serial.print(
        "ACTION: Remote IGNITION "
    );

    Serial.println(
        remoteIgnitionRequest
            ? "ON"
            : "OFF"
    );
}


// ==================================================
// CAN Service Initialization
// ==================================================

void CAN_Service_Init()
{
    lastTransmitTime = millis();

    bcmStatusCounter = 0;
}


// ==================================================
// CAN Service Update
// ==================================================

void CAN_Service_Update()
{
    uint32_t now = millis();


    // =================================================
    // CAN RECEIVE
    // =================================================

    uint32_t id;
    uint8_t data[8];
    uint8_t length;


    while (
        CAN_Driver_Receive(
            &id,
            data,
            &length
        )
    )
    {
        // ---------------------------------------------
        // DEBUG: ANY FRAME RECEIVED
        // ---------------------------------------------

        Serial.print(
            "CAN RX ID=0x"
        );

        Serial.print(
            id,
            HEX
        );


        Serial.print(
            " DLC="
        );

        Serial.print(
            length
        );


        Serial.print(
            " DATA="
        );


        for (
            uint8_t i = 0;
            i < length;
            i++
        )
        {
            Serial.print(
                "0x"
            );


            if (data[i] < 0x10)
                Serial.print("0");


            Serial.print(
                data[i],
                HEX
            );


            Serial.print(
                " "
            );
        }


        Serial.println();


        // ---------------------------------------------
        // TCU REQUEST
        // ---------------------------------------------

        if (
            id == 0x200 &&
            length >= 1
        )
        {
            Serial.println(
                "CAN: TCU REQUEST RECEIVED"
            );


            CAN_Process_TCU_Request(
                data
            );
        }
    }


    // =================================================
    // BCM STATUS TRANSMISSION
    // =================================================

    if (
        now - lastTransmitTime >= 100
    )
    {
        lastTransmitTime = now;


        // ---------------------------------------------
        // Actual indicator output
        // ---------------------------------------------

        bool leftIndicator =
            Indicator_GetLeftOutput();


        bool rightIndicator =
            Indicator_GetRightOutput();


        // ---------------------------------------------
        // Hazard state
        // ---------------------------------------------

        bool hazardActive =
            Indicator_IsHazardActive();


        // ---------------------------------------------
        // Actual headlight output
        // ---------------------------------------------

        bool headlightsActive =
            HeadLights_GetOutputState() > 0;


        // ---------------------------------------------
        // Actual horn output
        // ---------------------------------------------

        bool hornActive =
            Horn_GetOutputState();


        // ---------------------------------------------
        // Actual ignition state
        // ---------------------------------------------

        bool ignitionState =
            getIgnitionState();


        // ---------------------------------------------
        // BCM fault
        // ---------------------------------------------

        bool bcmFault = false;


        // ---------------------------------------------
        // Pack BCM status
        // ---------------------------------------------

        uint8_t payload[2];


        CAN_Pack_BCM_Status(
            leftIndicator,
            rightIndicator,
            hazardActive,
            headlightsActive,
            hornActive,
            ignitionState,
            bcmFault,
            bcmStatusCounter,
            payload
        );


        // ---------------------------------------------
        // Transmit BCM Status
        // ---------------------------------------------

        CAN_Driver_Send(
            0x100,
            payload,
            2
        );


        // ---------------------------------------------
        // Increment counter
        // ---------------------------------------------

        bcmStatusCounter++;


        if (bcmStatusCounter > 7)
        {
            bcmStatusCounter = 0;
        }
    }
}