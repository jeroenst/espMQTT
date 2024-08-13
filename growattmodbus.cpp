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

  putdatamap(F("grid/today/kwh"), F("-"));
  putdatamap(F("grid/total/kwh"), F("-"));

#if defined(ESPMQTT_GROWATT_MODBUS_2)
  // Write AC Low Limit 1 and AC High Limit 1 
  //uint16_t values[] = {1840, 2550};
  //modbus_write_holding_registers(1, 52, 2, values);

  // Write AC Low Limit 1 cycles and AC High Limit 1 cycles
  //  uint16_t values[] = {100, 100};
  //  modbus_write_holding_registers(1, 68, 2, values);
#endif
}


uint8_t growattModbus_request() {
growattModbus_RxReady = false;

#if defined(ESPMQTT_GROWATT_MODBUS_1)
  switch (growattModbus_itteration)
  {
    case 0:
      putdatamap(F("status"), F("querying"));
      modbus_request_holding_registers(1, 73, 1);
      break;
    case 1:
      modbus_request_input_registers(1, 0, 14);
      break;
    case 2:
      modbus_request_input_registers(1, 14, 12);
      break;
    case 3:
      modbus_request_input_registers(1, 26, 7);
      break;
    case 4:
      modbus_request_holding_registers(1, 19, 4);
      break;
    case 5:
      modbus_request_holding_registers(1, 35, 8);
      break;
    case 6:
      modbus_request_holding_registers(1, 51, 8);
      break;
    case 7:
      modbus_request_holding_registers(1, 184, 8);
      break;
    case 8:
      modbus_request_holding_registers(1, 112, 1);
      break;
    default:
      growattModbus_itteration = 255;
    break;
  }
#elif defined(ESPMQTT_GROWATT_MODBUS_2)
  switch (growattModbus_itteration)
  {
    case 0:
      putdatamap(F("status"), F("querying"));
      modbus_request_holding_registers(1, 88, 1);
      break;
    case 1:
      modbus_request_input_registers(1, 0, 11);
      break;
    case 2:
      modbus_request_input_registers(1, 35, 15);
      break;
    case 3:
      modbus_request_input_registers(1, 53, 7);
      break;
    case 4:
      modbus_request_input_registers(1, 93, 1);
      break;
    case 5:
      modbus_request_holding_registers(1, 52, 20);
      break;
    case 6:
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
        putdatamap(F("modbus/version"), String((float)modbus_get_register(0) / 100, 2));
        break;

      case 1:
        switch (modbus_get_register(0))
        {
          case 0: putdatamap(F("inverter/status"), F("waiting"));
            break;
          case 1: putdatamap(F("inverter/status"), F("normal"));
            break;
          case 3: putdatamap(F("inverter/status"), F("fault"));
            break;
        }

        putdatamap(F("inverter/status/value"), String((int)modbus_get_register(0)));

        putdatamap(F("pv/watt"), String(modbus_get_two_register_double(1, 10), 1));

        putdatamap(F("pv/1/volt"), String((float)modbus_get_register(3) / 10, 1));
        putdatamap(F("pv/1/amp"),  String((float)modbus_get_register(4) / 10, 1));
        putdatamap(F("pv/1/watt"), String(modbus_get_two_register_double(5, 10), 1));

        putdatamap(F("pv/2/volt"), String((float)modbus_get_register(7) / 10, 1));
        putdatamap(F("pv/2/amp"),  String((float)modbus_get_register(8) / 10, 1));
        putdatamap(F("pv/2/watt"), String(modbus_get_two_register_double(9, 10), 1));

        putdatamap(F("grid/watt"), String(modbus_get_two_register_double(11, 10), 1));
        putdatamap(F("grid/frequency"),  String((float)modbus_get_register(13) / 100, 2));
        break;

      case 2:
        putdatamap(F("grid/l1/volt"), String((float)modbus_get_register(0) / 10, 1));
        putdatamap(F("grid/l1/amp"), String((float)modbus_get_register(1) / 10, 1));        
        putdatamap(F("grid/l1/watt"), String(modbus_get_two_register_double(2, 10), 1));
        putdatamap(F("grid/l2/volt"), String((float)modbus_get_register(4) / 10, 1));
        putdatamap(F("grid/l2/amp"), String((float)modbus_get_register(5) / 10, 1));        
        putdatamap(F("grid/l2/watt"), String(modbus_get_two_register_double(6, 10), 1));
        putdatamap(F("grid/l3/volt"), String((float)modbus_get_register(8) / 10, 1));
        putdatamap(F("grid/l3/amp"), String((float)modbus_get_register(9) / 10, 1));        
        putdatamap(F("grid/l3/watt"), String(modbus_get_two_register_double(10, 10), 1));
        break;

      case 3:
        putdatamap(F("grid/today/kwh"), String(modbus_get_two_register_double(0, 10), 1));
        putdatamap(F("grid/total/kwh"), String(modbus_get_two_register_double(2, 10), 1));
        putdatamap(F("inverter/seconds"), String(modbus_get_two_register_double(4, 2), 1));
        putdatamap(F("inverter/temperature"), String((float)modbus_get_register(6) / 10, 1));
        break;

      case 4:
        putdatamap(F("grid/volt/low/1"), String((float)modbus_get_register(0) / 10, 1));
        putdatamap(F("grid/volt/high/1"), String((float)modbus_get_register(1) / 10, 1));
        putdatamap(F("grid/freq/low/1"), String((float)modbus_get_register(2) / 100, 1));
        putdatamap(F("grid/freq/high/1"), String((float)modbus_get_register(3) / 100, 1));
        break;

      case 5:
        putdatamap(F("grid/volt/low/2"), String((float)modbus_get_register(0) / 10, 1));
        putdatamap(F("grid/volt/high/2"), String((float)modbus_get_register(1) / 10, 1));
        putdatamap(F("grid/freq/low/2"), String((float)modbus_get_register(2) / 100, 1));
        putdatamap(F("grid/freq/high/2"), String((float)modbus_get_register(3) / 100, 1));
        putdatamap(F("grid/volt/low/connect"), String((float)modbus_get_register(4) / 10, 1));
        putdatamap(F("grid/volt/high/connect"), String((float)modbus_get_register(5) / 10, 1));
        putdatamap(F("grid/freq/low/connect"), String((float)modbus_get_register(6) / 100, 1));
        putdatamap(F("grid/freq/high/connect"), String((float)modbus_get_register(7) / 100, 1));
        break;

      case 6:
        putdatamap(F("grid/time/low/1"), String(modbus_get_register(0)));
        putdatamap(F("grid/time/high/1"), String(modbus_get_register(1)));
        putdatamap(F("grid/time/low/2"), String(modbus_get_register(2)));
        putdatamap(F("grid/time/high/2"), String(modbus_get_register(3)));
        break; 

      case 7:
        putdatamap(F("grid/volt/low/3"), String((float)modbus_get_register(0) / 10, 1));
        putdatamap(F("grid/volt/high/3"), String((float)modbus_get_register(1) / 10, 1));
        putdatamap(F("grid/time/low/3"), String(modbus_get_register(2)));
        putdatamap(F("grid/time/high/3"), String(modbus_get_register(3)));
        putdatamap(F("grid/freq/low/3"), String((float)modbus_get_register(4) / 100, 1));
        putdatamap(F("grid/freq/high/3"), String((float)modbus_get_register(5) / 100, 1));
        break;

      case 8:
        putdatamap(F("grid/volt/high/10min"), String((float)modbus_get_register(0) / 10, 1));
        putdatamap(F("status"), F("ready"));
        break;
    }
#elif defined(ESPMQTT_GROWATT_MODBUS_2)
    switch (growattModbus_itteration++)
    {
      case 0:
        putdatamap(F("modbus/version"), String((float)modbus_get_register(0) / 100, 2));
        break;

      case 1:
        switch (modbus_get_register(0))
        {
          case 0: putdatamap(F("inverter/status"), F("waiting"));
            break;
          case 1: putdatamap(F("inverter/status"), F("normal"));
            break;
          case 3: putdatamap(F("inverter/status"), F("fault"));
            break;
        }


        putdatamap(F("inverter/status/value"), String((int)modbus_get_register(0)));

        putdatamap(F("pv/watt"), String(modbus_get_two_register_double(1, 10), 1));

        putdatamap(F("pv/1/volt"), String((float)modbus_get_register(3) / 10, 1));
        putdatamap(F("pv/1/amp"),  String((float)modbus_get_register(4) / 10, 1));
        putdatamap(F("pv/1/watt"), String(modbus_get_two_register_double(5, 10), 1));

        putdatamap(F("pv/2/volt"), String((float)modbus_get_register(7) / 10, 1));
        putdatamap(F("pv/2/amp"),  String((float)modbus_get_register(8) / 10, 1));
        putdatamap(F("pv/2/watt"), String(modbus_get_two_register_double(9, 10), 1));
        break;

      case 2:
        putdatamap(F("grid/watt"), String(modbus_get_two_register_double(0, 10), 1));
        putdatamap(F("grid/frequency"), String((float)modbus_get_register(2) / 100, 1));
        putdatamap(F("grid/l1/volt"), String((float)modbus_get_register(3) / 10, 1));
        putdatamap(F("grid/l1/amp"), String((float)modbus_get_register(4) / 10, 1));
        putdatamap(F("grid/l1/watt"), String(modbus_get_two_register_double(5, 10), 1));
        putdatamap(F("grid/l2/volt"), String((float)modbus_get_register(7) / 10, 1));
        putdatamap(F("grid/l2/amp"), String((float)modbus_get_register(8) / 10, 1));
        putdatamap(F("grid/l2/watt"), String(modbus_get_two_register_double(9, 10), 1));
        putdatamap(F("grid/l3/volt"), String((float)modbus_get_register(11) / 10, 1));
        putdatamap(F("grid/l3/amp"), String((float)modbus_get_register(12) / 10, 1));
        putdatamap(F("grid/l3/watt"), String(modbus_get_two_register_double(13, 10), 1));
        break;

      case 3:
        putdatamap(F("grid/today/kwh"), String(modbus_get_two_register_double(0, 10), 1));
        putdatamap(F("grid/total/kwh"), String(modbus_get_two_register_double(2, 10), 1));
        putdatamap(F("inverter/seconds"), String(modbus_get_two_register_double(4, 2), 1));
        break;

      case 4:
        putdatamap(F("inverter/temperature"), String((float)modbus_get_register(0) / 10, 1));
        break;

      case 5:
        putdatamap(F("grid/volt/low/1"), String((float)modbus_get_register(0) / 10, 1));
        putdatamap(F("grid/volt/high/1"), String((float)modbus_get_register(1) / 10, 1));
        putdatamap(F("grid/freq/low/1"), String((float)modbus_get_register(2) / 100, 1));
        putdatamap(F("grid/freq/high/1"), String((float)modbus_get_register(3) / 100, 1));
        putdatamap(F("grid/volt/low/2"), String((float)modbus_get_register(4) / 10, 1));
        putdatamap(F("grid/volt/high/2"), String((float)modbus_get_register(5) / 10, 1));
        putdatamap(F("grid/freq/low/2"), String((float)modbus_get_register(6) / 100, 1));
        putdatamap(F("grid/freq/high/2"), String((float)modbus_get_register(7) / 100, 1));
        putdatamap(F("grid/volt/low/3"), String((float)modbus_get_register(8) / 10, 1));
        putdatamap(F("grid/volt/high/3"), String((float)modbus_get_register(9) / 10, 1));
        putdatamap(F("grid/freq/low/3"), String((float)modbus_get_register(10) / 100, 1));
        putdatamap(F("grid/freq/high/3"), String((float)modbus_get_register(11) / 100, 1));
        putdatamap(F("grid/volt/low/connect"), String((float)modbus_get_register(12) / 10, 1));
        putdatamap(F("grid/volt/high/connect"), String((float)modbus_get_register(13) / 10, 1));
        putdatamap(F("grid/freq/low/connect"), String((float)modbus_get_register(14) / 100, 1));
        putdatamap(F("grid/freq/high/connect"), String((float)modbus_get_register(15) / 100, 1));
        putdatamap(F("grid/time/low/1"), String(modbus_get_register(16)));
        putdatamap(F("grid/time/high/1"), String(modbus_get_register(17)));
        putdatamap(F("grid/time/low/2"), String(modbus_get_register(18)));
        putdatamap(F("grid/time/high/2"), String(modbus_get_register(19)));
      break;
        
      case 6:
        putdatamap(F("grid/volt/high/10min"), String((float)modbus_get_register(0) / 10, 1));
        putdatamap(F("status"), F("ready"));
        growattModbus_errorCounter = 0;
        break;
    }
#endif

    modbus_clear_buffer();
    return growattModbus_itteration;
  }
  return -1;
}

void growattModbus_handle()
{
  static long long timeout = GROWATTMODBUS_WAIT_AFTER_STARTUP_TIMEOUT;

  // When receiving has finished request next packet if it's not the last packet of sequence
  // and set timeout for next request round
  if (growattModbus_RxReady)
  {
    growattModbus_RxReady = false;
    if (growattModbus_itteration < 255) 
    {
      growattModbus_request();
    }
    else 
    {
      modbus_clear_buffer();
    }
    timeout = millis() + (GROWATTMODBUS_TIMEOUT * 1000);
  }

  // If timeout has reached errorcounter is increased and a retry to get modbus data
  // is performed
  if ((millis() > timeout))
  {
      if (growattModbus_errorCounter > 4)
      {
        putdatamap(F("inverter/status"), F("offline"));
        putdatamap(F("inverter/status/value"), F("-"));
        putdatamap(F("pv/watt"), F("-"));
        putdatamap(F("pv/1/volt"), F("-"));
        putdatamap(F("pv/1/amp"), F("-"));
        putdatamap(F("pv/1/watt"), F("-"));
        putdatamap(F("pv/2/volt"), F("-"));
        putdatamap(F("pv/2/amp"), F("-"));
        putdatamap(F("pv/2/watt"), F("-"));
        putdatamap(F("grid/l1/volt"), F("-"));
        putdatamap(F("grid/l1/amp"), F("-"));
        putdatamap(F("grid/l1/watt"), F("-"));
        putdatamap(F("grid/l2/volt"), F("-"));
        putdatamap(F("grid/l2/amp"), F("-"));
        putdatamap(F("grid/l2/watt"), F("-"));
        putdatamap(F("grid/l3/volt"), F("-"));
        putdatamap(F("grid/l3/amp"), F("-"));
        putdatamap(F("grid/l3/watt"), F("-"));
        putdatamap(F("grid/watt"), F("-"));
        putdatamap(F("grid/frequency"), F("-"));
        putdatamap(F("inverter/temperature"), F("-"));
        putdatamap(F("status"), F("commerror"));
      }
      else 
      {
        if (Debug.isActive(Debug.ERROR)) Debug.printf(cF("Modbus Receive Error!\n"));
        growattModbus_errorCounter++;
      }
      growattModbus_itteration = 0;
      growattModbus_RxReady = true;
  }

  modbus_handle();
  growattModbus_read();
}
