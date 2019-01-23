#include "espMQTT.h"

void ducobox_init(uint8_t ducobox_relay0, uint8_t ducobox_relay1, uint8_t ducobox_refreshtime, void(*callback)(String,String));
void ducobox_setfan(uint8_t value);
void ducobox_handle();
