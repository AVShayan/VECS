void LeftIndicator_setRequest(uint8_t request);
void RightIndicator_setRequest(uint8_t request);
void HazardIndicator_setRequest(uint8_t request);

void RemoteHazardIndicator_setRequest(uint8_t request);

uint8_t Indicator_GetLeftOutput();
uint8_t Indicator_GetRightOutput();
uint8_t Indicator_IsHazardActive();

void Indicator_Init();
void Indicator_Update();
void Indicator_Apply();