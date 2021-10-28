/*
 * Based on: https://github.com/timjong93/growatt_mqtt
 * Depends on: https://github.com/tekk/modbusmaster-esp8266
 * 
 */

#include "espMQTT.h"
#include "growattmodbus.h"
#include <ModbusMaster.h>

ModbusMaster node;

void(*_growattModbus_callback)(String,String);

void growattModbus_init(void(*callback)(String,String), int fanpin)
{
  _growattModbus_callback = callback;
  Serial.setRxBufferSize(100); 
  Serial.begin(9600);  //Init serial 9600 baud
  Serial.setDebugOutput(false);
  // instantiate modbusmaster with slave id 1 (growatt)
  node.begin(1, Serial);

  _growattModbus_callback("grid/today/kwh", "-");
  _growattModbus_callback("grid/total/kwh", "-");
  _growattModbus_callback("inverter/hours", "-");
}



float glueFloat(unsigned int d1, unsigned int d0) {
  unsigned long t;
  t = d1 << 16;
  t += d0;

  float f;
  f = t;
  f = f / 10;
  return f;
}

bool update_growatt() {
  uint8_t result;  

  _growattModbus_callback("status", "querying");

  result = node.readInputRegisters(0, 33);
  // do something with data if read is successful
  if (result == node.ku8MBSuccess){

    switch (node.getResponseBuffer(0))
    {
      case 0: _growattModbus_callback("inverter/status", "waiting");
      break;
      case 1: _growattModbus_callback("inverter/status", "normal");
      break;
      case 3: _growattModbus_callback("inverter/status", "fault");
      break;
    }
    
    
    _growattModbus_callback("status", String(glueFloat(node.getResponseBuffer(1), node.getResponseBuffer(2)),1));    

    _growattModbus_callback("pv/watt", String(glueFloat(node.getResponseBuffer(1), node.getResponseBuffer(2)),1));    

    _growattModbus_callback("pv/1/volt", String(glueFloat(0, node.getResponseBuffer(3)),3));    
    _growattModbus_callback("pv/1/amp",  String(glueFloat(0, node.getResponseBuffer(4)),3));    
    _growattModbus_callback("pv/1/watt", String(glueFloat(node.getResponseBuffer(5), node.getResponseBuffer(6)),1));

    _growattModbus_callback("pv/2/volt", String(glueFloat(0, node.getResponseBuffer(7)),3));    
    _growattModbus_callback("pv/2/amp",  String(glueFloat(0, node.getResponseBuffer(8)),3));    
    _growattModbus_callback("pv/2/watt", String(glueFloat(node.getResponseBuffer(9), node.getResponseBuffer(10)),1));
    
    _growattModbus_callback("grid/watt", String(glueFloat(node.getResponseBuffer(11), node.getResponseBuffer(12)),1));
    
    _growattModbus_callback("grid/frequency", String(glueFloat(0, node.getResponseBuffer(13))/10,1));  
  

    _growattModbus_callback("grid/volt", String(glueFloat(0, node.getResponseBuffer(14)),1));
    //_growattModbus_callback("grid/1/volt", String(glueFloat(0, node.getResponseBuffer(14)),1));
    _growattModbus_callback("grid/amp", String(glueFloat(0, node.getResponseBuffer(15)),1));
    //_growattModbus_callback("grid/1/amp", String(glueFloat(0, node.getResponseBuffer(15)),1));
    //_growattModbus_callback("grid/1/va", String(glueFloat(node.getResponseBuffer(16), node.getResponseBuffer(17)),1));

    //_growattModbus_callback("grid/2/volt", String(glueFloat(0, node.getResponseBuffer(18)),1));
    //_growattModbus_callback("grid/2/amp", String(glueFloat(0, node.getResponseBuffer(19)),1));
    //_growattModbus_callback("grid/2/va", String(glueFloat(node.getResponseBuffer(20), node.getResponseBuffer(21)),1));

    //_growattModbus_callback("grid/3/volt", String(glueFloat(0, node.getResponseBuffer(22)),1));
    //_growattModbus_callback("grid/3/amp", String(glueFloat(0, node.getResponseBuffer(23)),1));
    //_growattModbus_callback("grid/3/va", String(glueFloat(node.getResponseBuffer(24), node.getResponseBuffer(25)),1));

    _growattModbus_callback("grid/today/kwh", String(glueFloat(node.getResponseBuffer(26), node.getResponseBuffer(27)),1));
    _growattModbus_callback("grid/total/kwh", String(glueFloat(node.getResponseBuffer(28), node.getResponseBuffer(29)),1));
    _growattModbus_callback("inverter/hours", String(glueFloat(node.getResponseBuffer(30), node.getResponseBuffer(31)),1));
    _growattModbus_callback("temperature", String(glueFloat(0, node.getResponseBuffer(32)),1));
    _growattModbus_callback("status", "ready");
    return 0;
  } else {
      _growattModbus_callback("inverter/status", "offline");
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


      //_growattModbus_callback("grid/1/volt", "-");
      //_growattModbus_callback("grid/1/amp", "-");
      //_growattModbus_callback("grid/1/va", "-");

      //_growattModbus_callback("grid/2/volt", "-");
      //_growattModbus_callback("grid/2/amp", "-");
      //_growattModbus_callback("grid/2/va", "-");

      
      //_growattModbus_callback("grid/3/volt", "-");
      //_growattModbus_callback("grid/3/amp", "-");
      //_growattModbus_callback("grid/3/va", "-");


      //_growattModbus_callback("fault/temperature", "-");
      //_growattModbus_callback("fault/type", "-");
      //_growattModbus_callback("temperature", "-");
      //_growattModbus_callback("status", "commerror");
      return 1;
  }
  node.clearResponseBuffer();
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
