/*
   Based on: https://github.com/timjong93/growatt_mqtt
   Depends on: https://github.com/tekk/modbusmaster-esp8266

*/

#include "espMQTT.h"
#include "growattmodbus.h"
#include <ModbusMaster.h>

ModbusMaster node;

void(*_growattModbus_callback)(char *, String);

void growattModbus_init(void(*callback)(char *, String), int fanpin)
{
  _growattModbus_callback = callback;
  Serial.setRxBufferSize(10);
  Serial.begin(9600);  //Init serial 9600 baud
  Serial.setDebugOutput(false);
  // instantiate modbusmaster with slave id 1 (growatt)
  node.begin(1, Serial);

  _growattModbus_callback("grid/today/kwh", "-");
  _growattModbus_callback("grid/total/kwh", "-");
  _growattModbus_callback("inverter/hours", "-");
}



double glue(unsigned int d1, unsigned int d0) {
  double t;
  t = d1 << 16;
  t += d0;
  return t;
}

bool update_growatt() {
  uint8_t result;
  static uint8_t itteration = 0;


  if (itteration > 3) itteration = 0;

  switch (itteration)
  {
    case 0:
      _growattModbus_callback("status", "querying");
      result = node.readInputRegisters(0, 11);
      break;
    case 1:
      result = node.readInputRegisters(35, 5);
      break;
    case 2:
      result = node.readInputRegisters(53, 6);
      break;
    case 3:
      result = node.readInputRegisters(93, 1);
      break;
  }

  // do something with data if read is successful
  if (result == node.ku8MBSuccess) {
    switch (itteration)
    {
      case 0:
        switch (node.getResponseBuffer(0))
        {
          case 0: _growattModbus_callback("inverter/status", "waiting");
            break;
          case 1: _growattModbus_callback("inverter/status", "normal");
            break;
          case 3: _growattModbus_callback("inverter/status", "fault");
            break;
        }


        _growattModbus_callback("inverter/status/value", String((int)node.getResponseBuffer(0)));

        _growattModbus_callback("pv/watt", String(glue(node.getResponseBuffer(1), node.getResponseBuffer(2)) / 10, 1));

        _growattModbus_callback("pv/1/volt", String((float)node.getResponseBuffer(3) / 10, 1));
        _growattModbus_callback("pv/1/amp",  String((float)node.getResponseBuffer(4) / 10, 1));
        _growattModbus_callback("pv/1/watt", String(glue(node.getResponseBuffer(5), node.getResponseBuffer(6)) / 10, 1));

        _growattModbus_callback("pv/2/volt", String((float)node.getResponseBuffer(7) / 10, 1));
        _growattModbus_callback("pv/2/amp",  String((float)node.getResponseBuffer(8) / 10, 1));
        _growattModbus_callback("pv/2/watt", String(glue(node.getResponseBuffer(9), node.getResponseBuffer(10)) / 10, 1));
        break;

      case 1:
        _growattModbus_callback("grid/watt", String(glue(node.getResponseBuffer(0), node.getResponseBuffer(1)) / 10, 1));
        _growattModbus_callback("grid/frequency", String((float)node.getResponseBuffer(2) / 100, 1));
        _growattModbus_callback("grid/volt", String((float)node.getResponseBuffer(3) / 10, 1));
        _growattModbus_callback("grid/amp", String((float)node.getResponseBuffer(4) / 10, 1));
        break;

      case 2:
        _growattModbus_callback("grid/today/kwh", String(glue(node.getResponseBuffer(0), node.getResponseBuffer(1)) / 10, 1));
        _growattModbus_callback("grid/total/kwh", String(glue(node.getResponseBuffer(2), node.getResponseBuffer(3)) / 10, 1));
        _growattModbus_callback("inverter/seconds", String(glue(node.getResponseBuffer(4), node.getResponseBuffer(5)) / 5, 1));
        break;

      case 3:
        _growattModbus_callback("inverter/temperature", String((float)node.getResponseBuffer(0) / 10, 1));
        _growattModbus_callback("status", "ready");
        break;
    }
    node.clearResponseBuffer();
    itteration++;
    return 0;
  } else {
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
    node.clearResponseBuffer();
    itteration++;
    return 1;
  }
}

void growattModbus_handle()
{
  static long long nextupdatetime = 0;

  if (millis() > nextupdatetime)
  {
    if (update_growatt())
    {
      nextupdatetime = millis() + (GROWATTMODBUS_POLL_TIMER_ERROR * 1000);
    }
    else
    {
      nextupdatetime = millis() + (GROWATTMODBUS_POLL_TIMER * 1000);
    }
  }
}
