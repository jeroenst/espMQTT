/*
   Based on: https://github.com/timjong93/growatt_mqtt
   Depends on: https://github.com/tekk/modbusmaster-esp8266

*/

#include "espMQTT.h"
#include "growattmodbus.h"
#include "modbus.h"

bool growattModbus_RxReady = false;
uint8_t growattModbus_itteration = 255;
uint8_t growattModbus_errorCounter = 4; // After boot set topics asap


void growattModbus_init()
{
  Serial.setRxBufferSize(100);
  Serial.begin(9600);  //Init serial 9600 baud
  Serial.setDebugOutput(false);

  putdatamap(cF("grid/today/kwh"), "-");
  putdatamap(cF("grid/total/kwh"), "-");

#if defined(ESPMQTT_GROWATT_MODBUS_2)
  // Write AC Low Limit 1 and AC High Limit 1
  // uint16_t values[] = {1850, 2530};
  // modbus_write_holding_registers(1, 52, 2, values);
#endif
}


uint8_t growattModbus_request() {
growattModbus_RxReady = false;

#if defined(ESPMQTT_GROWATT_MODBUS_1)
  switch (growattModbus_itteration)
  {
    case 0:
      putdatamap(cF("status"), sF("querying"));
      modbus_request_input_registers(1, 0, 14);
      break;
    case 1:
      modbus_request_input_registers(1, 14, 12);
      break;
    case 2:
      modbus_request_input_registers(1, 26, 7);
      break;
    case 3:
      modbus_request_holding_registers(1, 19, 4);
      break;
    case 4:
      modbus_request_holding_registers(1, 35, 8);
      break;
    case 5:
      modbus_request_holding_registers(1, 184, 6);
      break;
    default:
      growattModbus_itteration = 255;
    break;
  }
#elif defined(ESPMQTT_GROWATT_MODBUS_2)
  switch (growattModbus_itteration)
  {
    case 0:
      putdatamap(cF("status"), sF("querying"));
      modbus_request_input_registers(1, 0, 11);
      break;
    case 1:
      modbus_request_input_registers(1, 35, 15);
      break;
    case 2:
      modbus_request_input_registers(1, 53, 7);
      break;
    case 3:
      modbus_request_input_registers(1, 93, 1);
      break;
    case 4:
      modbus_request_holding_registers(1, 52, 14);
      break;
    case 5:
      modbus_request_holding_registers(1, 80, 1);
      break;
    default:
      growattModbus_itteration = 255;
    break;
  }
#endif

  return growattModbus_itteration;
}

int8_t growattModbus_read()
{
  // do something with data if read is successful
  if (modbus_rx_ready())
  {
    growattModbus_RxReady = true;

#if defined(ESPMQTT_GROWATT_MODBUS_1)
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

        putdatamap(cF("grid/watt"), String(modbus_get_two_register_double(11, 10), 1));
        putdatamap(cF("grid/frequency"),  String((float)modbus_get_register(13) / 100, 2));
        break;

      case 1:
        putdatamap(cF("grid/l1/volt"), String((float)modbus_get_register(0) / 10, 1));
        putdatamap(cF("grid/l1/amp"), String((float)modbus_get_register(1) / 10, 1));        
        putdatamap(cF("grid/l1/watt"), String(modbus_get_two_register_double(2, 10), 1));
        putdatamap(cF("grid/l2/volt"), String((float)modbus_get_register(4) / 10, 1));
        putdatamap(cF("grid/l2/amp"), String((float)modbus_get_register(5) / 10, 1));        
        putdatamap(cF("grid/l2/watt"), String(modbus_get_two_register_double(6, 10), 1));
        putdatamap(cF("grid/l3/volt"), String((float)modbus_get_register(8) / 10, 1));
        putdatamap(cF("grid/l3/amp"), String((float)modbus_get_register(9) / 10, 1));        
        putdatamap(cF("grid/l3/watt"), String(modbus_get_two_register_double(10, 10), 1));
        break;

      case 2:
        putdatamap(cF("grid/today/kwh"), String(modbus_get_two_register_double(0, 10), 1));
        putdatamap(cF("grid/total/kwh"), String(modbus_get_two_register_double(2, 10), 1));
        putdatamap(cF("inverter/seconds"), String(modbus_get_two_register_double(4, 10), 1));
        putdatamap(cF("inverter/temperature"), String((float)modbus_get_register(6) / 10, 1));
        break;

      case 3:
        putdatamap(cF("grid/volt/low/1"), String((float)modbus_get_register(0) / 10, 1));
        putdatamap(cF("grid/volt/high/1"), String((float)modbus_get_register(1) / 10, 1));
        putdatamap(cF("grid/freq/low/1"), String((float)modbus_get_register(2) / 100, 1));
        putdatamap(cF("grid/freq/high/1"), String((float)modbus_get_register(3) / 100, 1));
        break;

      case 4:
        putdatamap(cF("grid/volt/low/2"), String((float)modbus_get_register(0) / 10, 1));
        putdatamap(cF("grid/volt/high/2"), String((float)modbus_get_register(1) / 10, 1));
        putdatamap(cF("grid/freq/low/2"), String((float)modbus_get_register(2) / 100, 1));
        putdatamap(cF("grid/freq/high/2"), String((float)modbus_get_register(3) / 100, 1));
        putdatamap(cF("grid/volt/low/connect"), String((float)modbus_get_register(4) / 10, 1));
        putdatamap(cF("grid/volt/high/connect"), String((float)modbus_get_register(5) / 10, 1));
        putdatamap(cF("grid/freq/low/connect"), String((float)modbus_get_register(6) / 100, 1));
        putdatamap(cF("grid/freq/high/connect"), String((float)modbus_get_register(7) / 100, 1));
        break; 

      case 5:
        putdatamap(cF("grid/volt/low/3"), String((float)modbus_get_register(0) / 10, 1));
        putdatamap(cF("grid/volt/high/3"), String((float)modbus_get_register(1) / 10, 1));
        putdatamap(cF("grid/freq/low/3"), String((float)modbus_get_register(4) / 100, 1));
        putdatamap(cF("grid/freq/high/3"), String((float)modbus_get_register(5) / 100, 1));
        break;
    }
#elif defined(ESPMQTT_GROWATT_MODBUS_2)
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
        putdatamap(cF("grid/l1/volt"), String((float)modbus_get_register(3) / 10, 1));
        putdatamap(cF("grid/l1/amp"), String((float)modbus_get_register(4) / 10, 1));
        putdatamap(cF("grid/l1/watt"), String(modbus_get_two_register_double(5, 10), 1));
        putdatamap(cF("grid/l2/volt"), String((float)modbus_get_register(7) / 10, 1));
        putdatamap(cF("grid/l2/amp"), String((float)modbus_get_register(8) / 10, 1));
        putdatamap(cF("grid/l2/watt"), String(modbus_get_two_register_double(9, 10), 1));
        putdatamap(cF("grid/l3/volt"), String((float)modbus_get_register(11) / 10, 1));
        putdatamap(cF("grid/l3/amp"), String((float)modbus_get_register(12) / 10, 1));
        putdatamap(cF("grid/l3/watt"), String(modbus_get_two_register_double(13, 10), 1));
        break;

      case 2:
        putdatamap(cF("grid/today/kwh"), String(modbus_get_two_register_double(0, 10), 1));
        putdatamap(cF("grid/total/kwh"), String(modbus_get_two_register_double(2, 10), 1));
        putdatamap(cF("inverter/seconds"), String(modbus_get_two_register_double(4, 10), 1));
        break;

      case 3:
        putdatamap(cF("inverter/temperature"), String((float)modbus_get_register(0) / 10, 1));
        break;

      case 4:
        putdatamap(cF("grid/volt/low/1"), String((float)modbus_get_register(0) / 10, 1));
        putdatamap(cF("grid/volt/high/1"), String((float)modbus_get_register(1) / 10, 1));
        putdatamap(cF("grid/freq/low/1"), String((float)modbus_get_register(2) / 100, 1));
        putdatamap(cF("grid/freq/high/1"), String((float)modbus_get_register(3) / 100, 1));
        putdatamap(cF("grid/volt/low/2"), String((float)modbus_get_register(4) / 10, 1));
        putdatamap(cF("grid/volt/high/2"), String((float)modbus_get_register(5) / 10, 1));
        putdatamap(cF("grid/freq/low/2"), String((float)modbus_get_register(6) / 100, 1));
        putdatamap(cF("grid/freq/high/2"), String((float)modbus_get_register(7) / 100, 1));
        putdatamap(cF("grid/volt/low/3"), String((float)modbus_get_register(8) / 10, 1));
        putdatamap(cF("grid/volt/high/3"), String((float)modbus_get_register(9) / 10, 1));
        putdatamap(cF("grid/freq/low/3"), String((float)modbus_get_register(10) / 100, 1));
        putdatamap(cF("grid/freq/high/3"), String((float)modbus_get_register(11) / 100, 1));
        putdatamap(cF("grid/volt/low/connect"), String((float)modbus_get_register(12) / 10, 1));
        putdatamap(cF("grid/volt/high/connect"), String((float)modbus_get_register(13) / 10, 1));
        break;
        
      case 5:
        putdatamap(cF("grid/volt/high/10min"), String((float)modbus_get_register(0) / 10, 1));
        putdatamap(cF("status"), sF("ready"));
        break;
    }
#endif

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
    if (growattModbus_itteration < 255) 
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
        putdatamap(cF("grid/l1/volt"), sF("-"));
        putdatamap(cF("grid/l1/amp"), sF("-"));
        putdatamap(cF("grid/l1/watt"), sF("-"));
        putdatamap(cF("grid/l2/volt"), sF("-"));
        putdatamap(cF("grid/l2/amp"), sF("-"));
        putdatamap(cF("grid/l2/watt"), sF("-"));
        putdatamap(cF("grid/l3/volt"), sF("-"));
        putdatamap(cF("grid/l3/amp"), sF("-"));
        putdatamap(cF("grid/l3/watt"), sF("-"));
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
