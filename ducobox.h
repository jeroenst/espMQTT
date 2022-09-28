void ducobox_init(uint8_t ducobox_relay0, uint8_t ducobox_relay1, void(*callback)());
void ducobox_setfan(uint8_t value);
void ducobox_handle();


enum class Ducobox_status {initializing, commerror, querying, ready};


struct Ducobox_DataMap_Struct
{
  Ducobox_status status = Ducobox_status::initializing;
  uint16_t fanspeed = UINT16_MAX;
  uint16_t minfanspeed = UINT16_MAX;
  uint16_t node_4_co2 = UINT16_MAX;
  uint8_t node_4_co2_retries = 0;
  int16_t node_4_temperature = INT16_MAX;
  uint8_t node_4_temperature_retries = 0;
  uint16_t node_26_humidity = UINT16_MAX;
  int16_t node_26_temperature = INT16_MAX;
};

extern Ducobox_DataMap_Struct ducobox_DataMap;
