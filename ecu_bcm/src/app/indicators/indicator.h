#ifndef INDICATOR_H
#define INDICATOR_H

#include<stdint.h>

void Indicator_Init();
void Indicator_Update();
void Indicator_Apply();
void LeftIndicator_setRequest(uint8_t request);
void RightIndicator_setRequest(uint8_t request);
void HazardIndicator_setRequest(uint8_t request);

#endif 