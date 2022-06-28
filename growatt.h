void growatt_init(void(*callback)(), int fanpin = -1);
void growatt_handle();

#define GROWATT_FANSPEED_OFFSET 150
#define GROWATT_FANSPEED_TEMP 470
#define GROWATT_POLL_TIMER 3

enum class Growatt_status {offline, querying, ready};

struct Growatt_DataMap_Struct
{
    struct 
    {
      uint8_t inverter_status_value : 1;
      uint8_t inverter_status : 1;
      uint8_t pv_1_voltage : 1;
      uint8_t pv_2_voltage : 1;
      uint8_t pv_power : 1;
      uint8_t grid_voltage : 1;
      uint8_t grid_current : 1;
      uint8_t grid_frequency : 1;
      uint8_t grid_power : 1;
      uint8_t fault_temperature : 1;
      uint8_t fault_type : 1;
      uint8_t temperature : 1;    
      uint8_t grid_today_energy : 1;
      uint8_t grid_total_energy : 1;
      uint8_t inverter_time : 1;
      uint8_t fanspeed : 1;
    } changed;
    uint16_t inverter_status_value;
    const char *inverter_status;
    uint16_t pv_1_voltage; // 0.1 volt precision
    uint16_t pv_2_voltage; // 0.1 volt precision
    uint16_t pv_power; // 0.1 watt precision
    uint16_t grid_voltage; // 0.1 volt precision
    uint16_t grid_current;  // 0.01 amp precision
    uint16_t grid_frequency; // 0,1 Hz precision
    uint16_t grid_power; // 0.1 watt precision
    uint16_t fault_temperature; // 0.1 degree precision
    uint16_t fault_type;
    uint16_t temperature; // 0.1 degree precision
    uint16_t grid_today_energy; // 0.1 kWh precision
    uint32_t grid_total_energy; // 0.1 kWh precision
    uint32_t inverter_time; // hour precision
    uint16_t fanspeed;
    enum Growatt_status status = Growatt_status::offline;
};

extern Growatt_DataMap_Struct growatt_DataMap;
