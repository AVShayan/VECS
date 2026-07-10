/*
#ifndef DTC_H
#define DTC_H

static int dtc_status[DTC_MAX];

typedef enum {
    DTC_NONE,
    DTC_LEFT_INDICATOR_MALFUNCTION,
    DTC_RIGHT_INDICATOR_MALFUNCTION,
    DTC_HEADLIGHT_MALFUNCTION,
    DTC_HORN_MALFUNCTION,
    DTC_MAX
} DTC_Code_t;

void DTC_Init();
void DTC_Set(DTC_Code_t);
void DTC_Clear(DTC_Code_t);
void DTC_ClearAll();
uint8_t DTC_isActive(DTC_Code_t code);
void DTC_PrintAll();

#endif
*/