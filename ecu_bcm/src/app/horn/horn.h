#ifndef HORN_H
#define HORN_H

#include<stdint.h>

void Horn_Init();
void Horn_Update();
void Horn_Apply();
void Horn_setRequest(uint8_t request);

#endif