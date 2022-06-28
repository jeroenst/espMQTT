void growattModbus_init(void(*callback)());
void growattModbus_handle();

#define GROWATTMODBUS_TIMEOUT 10
#define GROWATTMODBUS_WAIT 5
#define GROWATT_MODBUS_DATAMAP_LENGTH 19

enum class GrowattModbus_status {offline, querying, ready};

struct GrowattModbus_DataMap_Struct
{
    struct 
    {
      uint8_t inverter_status_value : 1;
      uint8_t inverter_status : 1;
      uint8_t pv_1_voltage : 1;
      uint8_t pv_1_current : 1;
      uint8_t pv_1_power : 1;
      uint8_t pv_2_voltage : 1;
      uint8_t pv_2_current : 1;
      uint8_t pv_2_power : 1;
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
    } changed;
    uint16_t inverter_status_value = 0;
    const char *inverter_status = "";
    uint16_t pv_1_voltage = 0; // 0.1 volt precision
    uint16_t pv_1_current = 0; // 0.1 volt precision
    uint16_t pv_1_power = 0; // 0.1 watt precision
    uint16_t pv_2_voltage = 0; // 0.1 volt precision
    uint16_t pv_2_current = 0; // 0.1 amp precision
    uint16_t pv_2_power = 0; // 0.1 watt precision
    uint16_t pv_power = 0; // 0.1 watt precision
    uint16_t grid_voltage = 0; // 0.1 volt precision
    uint16_t grid_current = 0;  // 0.01 amp precision
    uint16_t grid_frequency = 0; // 0,1 Hz precision
    uint16_t grid_power = 0; // 0.1 watt precision
    uint16_t fault_temperature = 0; // 0.1 degree precision
    uint16_t fault_type = 0;
    uint16_t temperature = 0; // 0.1 degree precision
    uint16_t grid_today_energy = 0; // 0.1 kWh precision
    uint32_t grid_total_energy = 0; // 0.1 kWh precision
    uint32_t inverter_time = 0; // 0.5 second precision
    enum GrowattModbus_status status = GrowattModbus_status::offline;
};

extern GrowattModbus_DataMap_Struct growattModbus_DataMap;
