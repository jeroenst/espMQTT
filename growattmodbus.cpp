/*
   Based on: https://github.com/timjong93/growatt_mqtt
   Depends on: https://github.com/tekk/modbusmaster-esp8266

*/

#include "espMQTT.h"
#include "growattmodbus.h"
#include "modbus.h"

GrowattModbus_DataMap_Struct growattModbus_DataMap;

bool growattModbus_RxReady = false;
uint8_t growattModbus_itteration = 0;
uint8_t growattModbus_errorCounter = 0;

void(*_growattModbus_callback)();

void growattModbus_init(void(*callback)())
{
  _growattModbus_callback = callback;
  Serial.setRxBufferSize(100);
  Serial.begin(9600);  //Init serial 9600 baud
  Serial.setDebugOutput(false);
}


uint8_t growattModbus_request() {
  growattModbus_RxReady = false;

  if (growattModbus_itteration > 3) growattModbus_itteration = 0;

  switch (growattModbus_itteration)
  {
    default:
    case 0:
      growattModbus_itteration = 0;
      growattModbus_DataMap.status = GrowattModbus_status::querying;
      _growattModbus_callback();
      modbus_request_function_code(1, 4, 0, 11);
      break;
    case 1:
      modbus_request_function_code(1, 4, 35, 5);
      break;
    case 2:
      modbus_request_function_code(1, 4, 53, 6);
      break;
    case 3:
      modbus_request_function_code(1, 4, 93, 1);
      break;
  }
  return growattModbus_itteration;
}

int8_t growattModbus_read()
{
  // do something with data if read is successful
  if (modbus_rx_ready())
  {
    growattModbus_RxReady = true;
    switch (growattModbus_itteration++)
    {
      case 0:
        switch (modbus_get_register(0))
        {
          case 0: growattModbus_DataMap.inverter_status = "waiting";
            break;
          case 1: growattModbus_DataMap.inverter_status = "normal";
            break;
          case 3: growattModbus_DataMap.inverter_status = "fault";
            break;
        }

        growattModbus_DataMap.changed.inverter_status = modbus_get_register(0) != growattModbus_DataMap.inverter_status_value;
        growattModbus_DataMap.changed.inverter_status_value = modbus_get_register(0) != growattModbus_DataMap.inverter_status_value;
        growattModbus_DataMap.inverter_status_value = modbus_get_register(0);

        growattModbus_DataMap.changed.pv_power = growattModbus_DataMap.pv_power != modbus_get_two_register_uint(1);
        growattModbus_DataMap.pv_power = modbus_get_two_register_uint(1);

        growattModbus_DataMap.changed.pv_1_voltage = growattModbus_DataMap.pv_1_voltage != modbus_get_register(3);
        growattModbus_DataMap.pv_1_voltage = modbus_get_register(3);
        growattModbus_DataMap.changed.pv_1_current = growattModbus_DataMap.pv_1_current != modbus_get_register(4);
        growattModbus_DataMap.pv_1_current = modbus_get_register(4);
        growattModbus_DataMap.changed.pv_1_power = growattModbus_DataMap.pv_1_power != modbus_get_two_register_uint(5);
        growattModbus_DataMap.pv_1_power = modbus_get_two_register_uint(5);

        growattModbus_DataMap.changed.pv_2_voltage = growattModbus_DataMap.pv_2_voltage != modbus_get_register(7);
        growattModbus_DataMap.pv_2_voltage = modbus_get_register(7);
        growattModbus_DataMap.changed.pv_2_current = growattModbus_DataMap.pv_2_current != modbus_get_register(8);
        growattModbus_DataMap.pv_2_current = modbus_get_register(8);
        growattModbus_DataMap.changed.pv_2_power = growattModbus_DataMap.pv_2_power != modbus_get_two_register_uint(9);
        growattModbus_DataMap.pv_2_power = modbus_get_two_register_uint(9);
        break;

      case 1:
        growattModbus_DataMap.changed.grid_power = growattModbus_DataMap.grid_power != modbus_get_two_register_uint(0);
        growattModbus_DataMap.grid_power = modbus_get_two_register_uint(0);
        growattModbus_DataMap.changed.grid_frequency = growattModbus_DataMap.grid_frequency != modbus_get_register(2);
        growattModbus_DataMap.grid_frequency = modbus_get_register(2);
        growattModbus_DataMap.changed.grid_voltage = growattModbus_DataMap.grid_voltage != modbus_get_register(3);
        growattModbus_DataMap.grid_voltage = modbus_get_register(3);
        growattModbus_DataMap.changed.grid_current = growattModbus_DataMap.grid_current != modbus_get_register(4);
        (growattModbus_DataMap.grid_current = modbus_get_register(4));
        break;

      case 2:
        growattModbus_DataMap.changed.grid_today_energy = growattModbus_DataMap.grid_today_energy != modbus_get_two_register_uint(0);
        growattModbus_DataMap.grid_today_energy = modbus_get_two_register_uint(0);
        growattModbus_DataMap.changed.grid_total_energy = growattModbus_DataMap.grid_total_energy != modbus_get_two_register_uint(2);
        growattModbus_DataMap.grid_total_energy = modbus_get_two_register_uint(2);
        growattModbus_DataMap.changed.inverter_time = growattModbus_DataMap.inverter_time != modbus_get_two_register_uint(4);
        growattModbus_DataMap.inverter_time = modbus_get_two_register_uint(4);
        break;

      case 3:
        growattModbus_DataMap.changed.temperature = growattModbus_DataMap.temperature != modbus_get_register(0);
        growattModbus_DataMap.temperature = modbus_get_register(0);
        growattModbus_DataMap.status = GrowattModbus_status::ready;
        growattModbus_DataMap.dataReady = true;
        _growattModbus_callback();
        break;
    }
    modbus_clear_buffer();
    return growattModbus_itteration;
  }
  else return -1;
}

void growattModbus_handle()
{
  static long long timeout = GROWATTMODBUS_WAIT;
  static bool communicationFinished = false;

  // When receiving has finished request next packet if it's not the last packet of sequence
  // and set timeout for next request round
  if (growattModbus_RxReady)
  {
    growattModbus_RxReady = false;
    if (growattModbus_itteration < 4) 
    {
      growattModbus_request();
      communicationFinished = false;
    }
    else 
    {
      modbus_clear_buffer();
      communicationFinished = true;
    }
    growattModbus_errorCounter = 0;
    timeout = millis() + (GROWATTMODBUS_WAIT * 1000);
  }

  // If timeout has reached errorcounter is increased and a retry to get modbus data
  // is performed
  if ((millis() > timeout))
  {
      if (growattModbus_errorCounter > 4)
      {
        growattModbus_DataMap.inverter_status = "offline";
        growattModbus_DataMap.status = GrowattModbus_status::offline;
        growattModbus_DataMap.dataReady = false;
        _growattModbus_callback();
      }
      else 
      {
        if (!communicationFinished) DEBUG_E ("Modbus Receive Error!\n");
        growattModbus_errorCounter++;
      }
      modbus_clear_buffer();
      timeout = millis() + (GROWATTMODBUS_TIMEOUT * 1000);
      growattModbus_itteration = 0;
      growattModbus_request();
  }

  modbus_handle();
  growattModbus_read();
}
