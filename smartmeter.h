int8_t smartmeter_handle();
void smartmeter_init(void(*callback)(void));

enum class Smartmeter_status {disconnected, receiving, ready};

struct Smartmeter_DataMap_Struct
{
  enum Smartmeter_status status = Smartmeter_status::disconnected;
  struct
  {
    struct 
    {
      uint8_t watt : 1;
      uint8_t watt_using : 1;
      uint8_t watt_providing : 1;
      uint8_t wh : 1;
      uint8_t wh_used1 : 1;
      uint8_t wh_used2 : 1;
      uint8_t wh_provided1 : 1;
      uint8_t wh_provided2 : 1;
    } changed;
    int16_t watt;
    uint16_t watt_using;
    uint16_t watt_providing;
    int32_t wh;
    uint32_t wh_used1;
    uint32_t wh_used2;
    uint32_t wh_provided1;
    uint32_t wh_provided2;
  } electricity;
  struct
  {
    struct 
    {
      uint8_t lh : 1;
      uint8_t liter : 1;
      uint8_t datetime : 1;
    } changed;
    uint16_t lh;
    uint32_t liter;
    char datetime[20];
  } gas;
  bool dataReady = false;
};


extern Smartmeter_DataMap_Struct smartmeter_DataMap;
