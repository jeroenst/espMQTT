void tuya_init(void(*callback)(const char *,String));
void tuya_handle();
void tuya_connected();
void tuya_connectedMQTT();
void tuya_disconnected();
void tuya_apmode();

void tuya_2gangdimmerv2_setdimvalue(uint8_t dimvalue, uint8_t dimchannel);
void tuya_2gangdimmerv2_setdimstate(bool dimstate, uint8_t dimchannel);
