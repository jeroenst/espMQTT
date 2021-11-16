/*
   Based on: https://github.com/timjong93/growatt_mqtt
   Depends on: https://github.com/tekk/modbusmaster-esp8266

*/

#include "espMQTT.h"
#include "growattmodbus.h"
#include "modbus.h"

bool growattModbus_RxReady = false;

void(*_growattModbus_callback)(const char*, String);

void growattModbus_init(void(*callback)(const char *, String), int fanpin)
{
  _growattModbus_callback = callback;
  Serial.setRxBufferSize(10);
  Serial.begin(9600);  //Init serial 9600 baud
  Serial.setDebugOutput(false);

  _growattModbus_callback("grid/today/kwh", "-");
  _growattModbus_callback("grid/total/kwh", "-");
}

uint8_t growattModbus_itteration = 0;

void growattModbus_request() {
  growattModbus_RxReady = false;

  if (growattModbus_itteration > 3) growattModbus_itteration = 0;

  switch (growattModbus_itteration)
  {
    default:
    case 0:
      growattModbus_itteration = 0;
      _growattModbus_callback("status", "querying");
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
}

int8_t growattModbus_read() 
{
  // do something with data if read is successful
  if (modbus_rx_ready()) {
    growattModbus_RxReady = true;
    switch (growattModbus_itteration)
    {
      case 0:
        switch (modbus_get_register(0))
        {
          case 0: _growattModbus_callback("inverter/status", "waiting");
            break;
          case 1: _growattModbus_callback("inverter/status", "normal");
            break;
          case 3: _growattModbus_callback("inverter/status", "fault");
            break;
        }


        _growattModbus_callback("inverter/status/value", String((int)modbus_get_register(0)));

        _growattModbus_callback("pv/watt", String(modbus_get_two_register_double(1, 10), 1));

        _growattModbus_callback("pv/1/volt", String((float)modbus_get_register(3) / 10, 1));
        _growattModbus_callback("pv/1/amp",  String((float)modbus_get_register(4) / 10, 1));
        _growattModbus_callback("pv/1/watt", String(modbus_get_two_register_double(5, 10), 1));

        _growattModbus_callback("pv/2/volt", String((float)modbus_get_register(7) / 10, 1));
        _growattModbus_callback("pv/2/amp",  String((float)modbus_get_register(8) / 10, 1));
        _growattModbus_callback("pv/2/watt", String(modbus_get_two_register_double(9, 10), 1));
        break;

      case 1:
        _growattModbus_callback("grid/watt", String(modbus_get_two_register_double(0, 10), 1));
        _growattModbus_callback("grid/frequency", String((float)modbus_get_register(2) / 100, 1));
        _growattModbus_callback("grid/volt", String((float)modbus_get_register(3) / 10, 1));
        _growattModbus_callback("grid/amp", String((float)modbus_get_register(4) / 10, 1));
        break;

      case 2:
        _growattModbus_callback("grid/today/kwh", String(modbus_get_two_register_double(0, 10), 1));
        _growattModbus_callback("grid/total/kwh", String(modbus_get_two_register_double(2, 10), 1));
        _growattModbus_callback("inverter/seconds", String(modbus_get_two_register_double(4, 10), 1));
        break;

      case 3:
        _growattModbus_callback("inverter/temperature", String((float)modbus_get_byte(4) / 10, 1));
        _growattModbus_callback("status", "ready");
        break;
    }
    modbus_clear_buffer();
    growattModbus_itteration++;
    return growattModbus_itteration - 1;
  }
  else return -1;
}

void growattModbus_handle()
{
  static long long nextupdatetime = 0;

  if ((millis() > nextupdatetime) && (millis() > 5000))
  {
    if (growattModbus_RxReady)
    {
      nextupdatetime = millis() + (GROWATTMODBUS_POLL_SHORT_TIMER * 1000);
    }
    else
    {
      _growattModbus_callback("inverter/status", "offline");
      _growattModbus_callback("inverter/status/value", "-");
      _growattModbus_callback("pv/watt", "-");
      _growattModbus_callback("pv/1/volt", "-");
      _growattModbus_callback("pv/1/amp", "-");
      _growattModbus_callback("pv/1/watt", "-");
      _growattModbus_callback("pv/2/volt", "-");
      _growattModbus_callback("pv/2/amp", "-");
      _growattModbus_callback("pv/2/watt", "-");
      _growattModbus_callback("grid/volt", "-");
      _growattModbus_callback("grid/amp", "-");
      _growattModbus_callback("grid/frequency", "-");
      _growattModbus_callback("grid/watt", "-");
      _growattModbus_callback("inverter/temperature", "-");
      _growattModbus_callback("status", "commerror");
      modbus_clear_buffer();
      growattModbus_itteration = 0;
      nextupdatetime = millis() + (GROWATTMODBUS_POLL_LONG_TIMER * 1000);
    }
    growattModbus_request();
  }

  modbus_handle();
  growattModbus_read();
}
