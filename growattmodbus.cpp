/*
   Based on: https://github.com/timjong93/growatt_mqtt
   Depends on: https://github.com/tekk/modbusmaster-esp8266

*/

#include "espMQTT.h"
#include "growattmodbus.h"
#include "modbus.h"

bool growattModbus_RxReady = false;
uint8_t growattModbus_itteration = 0;
uint8_t growattModbus_errorCounter = 4; // After boot set topics asap


void growattModbus_init()
{
  Serial.setRxBufferSize(100);
  Serial.begin(9600);  //Init serial 9600 baud
  Serial.setDebugOutput(false);

  putdatamap(cF("grid/today/kwh"), "-");
  putdatamap(cF("grid/total/kwh"), "-");
}


uint8_t growattModbus_request() {
  growattModbus_RxReady = false;

  if (growattModbus_itteration > 3) growattModbus_itteration = 0;

  switch (growattModbus_itteration)
  {
    default:
    case 0:
      growattModbus_itteration = 0;
      putdatamap(cF("status"), sF("querying"));
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
          case 0: putdatamap(cF("inverter/status"), sF("waiting"));
            break;
          case 1: putdatamap(cF("inverter/status"), sF("normal"));
            break;
          case 3: putdatamap(cF("inverter/status"), sF("fault"));
            break;
        }


        putdatamap(cF("inverter/status/value"), String((int)modbus_get_register(0)));

        putdatamap(cF("pv/watt"), String(modbus_get_two_register_double(1, 10), 1));

        putdatamap(cF("pv/1/volt"), String((float)modbus_get_register(3) / 10, 1));
        putdatamap(cF("pv/1/amp"),  String((float)modbus_get_register(4) / 10, 1));
        putdatamap(cF("pv/1/watt"), String(modbus_get_two_register_double(5, 10), 1));

        putdatamap(cF("pv/2/volt"), String((float)modbus_get_register(7) / 10, 1));
        putdatamap(cF("pv/2/amp"),  String((float)modbus_get_register(8) / 10, 1));
        putdatamap(cF("pv/2/watt"), String(modbus_get_two_register_double(9, 10), 1));
        break;

      case 1:
        putdatamap(cF("grid/watt"), String(modbus_get_two_register_double(0, 10), 1));
        putdatamap(cF("grid/frequency"), String((float)modbus_get_register(2) / 100, 1));
        putdatamap(cF("grid/volt"), String((float)modbus_get_register(3) / 10, 1));
        putdatamap(cF("grid/amp"), String((float)modbus_get_register(4) / 10, 1));
        break;

      case 2:
        putdatamap(cF("grid/today/kwh"), String(modbus_get_two_register_double(0, 10), 1));
        putdatamap(cF("grid/total/kwh"), String(modbus_get_two_register_double(2, 10), 1));
        putdatamap(cF("inverter/seconds"), String(modbus_get_two_register_double(4, 10), 1));
        break;

      case 3:
        putdatamap(cF("inverter/temperature"), String((float)modbus_get_register(0) / 10, 1));
        putdatamap(cF("status"), sF("ready"));
        break;
    }
    modbus_clear_buffer();
    return growattModbus_itteration;
  }
  else return -1;
}

void growattModbus_handle()
{
  static long long timeout = GROWATTMODBUS_WAIT_AFTER_STARTUP_TIMEOUT;
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
    timeout = millis() + (GROWATTMODBUS_TIMEOUT * 1000);
  }

  // If timeout has reached errorcounter is increased and a retry to get modbus data
  // is performed
  if ((millis() > timeout))
  {
      if (growattModbus_errorCounter > 4)
      {
        putdatamap(cF("inverter/status"), sF("offline"));
        putdatamap(cF("inverter/status/value"), sF("-"));
        putdatamap(cF("pv/watt"), sF("-"));
        putdatamap(cF("pv/1/volt"), sF("-"));
        putdatamap(cF("pv/1/amp"), sF("-"));
        putdatamap(cF("pv/1/watt"), sF("-"));
        putdatamap(cF("pv/2/volt"), sF("-"));
        putdatamap(cF("pv/2/amp"), sF("-"));
        putdatamap(cF("pv/2/watt"), sF("-"));
        putdatamap(cF("grid/volt"), sF("-"));
        putdatamap(cF("grid/amp"), sF("-"));
        putdatamap(cF("grid/frequency"), sF("-"));
        putdatamap(cF("grid/watt"), sF("-"));
        putdatamap(cF("inverter/temperature"), sF("-"));
        putdatamap(cF("status"), sF("commerror"));
      }
      else 
      {
        if (!communicationFinished) if (Debug.isActive(Debug.ERROR)) Debug.printf(cF("Modbus Receive Error!\n"));
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
