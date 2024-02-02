#include "espMQTT.h"
#include "opentherm.h"
WiFiServer otserver(25238);

#ifndef D5
#define D5 14
#endif

uint32_t wdresettimeout = 0;
uint8_t resetstate = 1;


void opentherm_serialprint(String serialmessage)
{
  DEBUG_V("%sSERIAL TX:%s %s\n", COLOR_RED, COLOR_RESET, serialmessage.c_str());
  serialmessage += "\r\n";
  Serial.print(serialmessage);
}

uint16_t opentherm_message_touint(String otmessage)
{
  return (strtol(otmessage.substring(5, 7).c_str(), 0, 16) << 8) + (strtol(otmessage.substring(7, 9).c_str(), 0, 16));
}

int16_t opentherm_message_toint(String otmessage)
{
  return ((strtol(otmessage.substring(5, 7).c_str(), 0, 16) & 0x127) << 8) + (strtol(otmessage.substring(7, 9).c_str(), 0, 16)) * ((strtol(otmessage.substring(5, 7).c_str(), 0, 16) & 0x128) ? -1 : 1);
}

float opentherm_twobytestofloat(uint16_t decimal, uint16_t fractional)
{
  return ((decimal & 127)  +
          ((fractional & 128) ? float(1) / 2 : 0) +
          ((fractional & 64) ? float(1) / 4 : 0) +
          ((fractional & 32) ? float(1) / 8 : 0) +
          ((fractional & 16) ? float(1) / 16 : 0) +
          ((fractional & 8) ? float(1) / 32 : 0) +
          ((fractional & 4) ? float(1) / 64 : 0) +
          ((fractional & 2) ? float(1) / 128 : 0) +
          ((fractional & 1) ? float(1) / 256 : 0)) *
         ((decimal & 128) ? -1 : 1);
}

float opentherm_message_tofloat(String otmessage)
{
  return opentherm_twobytestofloat(strtol(otmessage.substring(5, 7).c_str(), 0, 16), strtol(otmessage.substring(7, 9).c_str(), 0, 16));
}


void opentherm_reset()
{
  resetstate = 1;
}

void opentherm_init()
{
  Serial.setDebugOutput(false);
  Serial.setRxBufferSize(100);
  Serial.begin(9600);  //Init serial 9600 baud
  Serial.print("\r\n");

  otserver.begin();
}

int opentherm_handle()
{
  static WiFiClient otclient;
  static String serialbuffer = "";
  int returnvalue = 0;
  static int8_t otgwcommstate = 0;
  if (!otclient || !otclient.connected()) otclient = otserver.available();
  else
  {
    if (otclient.available())
    {
      yield();
      Serial.print((char)otclient.read());
    }
  }

  yield();

  while (Serial.available() > 0) {
    yield();
    char otchar = char(Serial.read());
    serialbuffer += otchar;
    if (otclient && otclient.connected())
    {
      otclient.print(otchar);
    }
  }

  yield();

  if (serialbuffer.indexOf(10) >= 0)
  {
    int eolchar = serialbuffer.indexOf(10);
    String otmessage = serialbuffer.substring(0, eolchar - 1);
    serialbuffer = serialbuffer.substring(eolchar + 1);
    yield();
    DEBUG_V("%sSERIAL RX:%s %s\n", COLOR_GREEN, COLOR_RESET, otmessage.c_str());
    yield();
    String otvalue = "";
    String topic = "";
    uint16_t otintvalue = 0;

    if (otmessage.substring(0, 4) == "TC: ")
    {
      topic = sF("thermostat/setpoint");
      otvalue = otmessage.substring(4);
      wdresettimeout = 0;
      otgwcommstate = 2;
    }

    else if (otmessage.substring(0, 4) == "TT: ")
    {
      topic = sF("thermostat/setpoint");
      otvalue = otmessage.substring(4);
      wdresettimeout = 0;
      otgwcommstate = 2;
    }

    else if (otmessage.substring(0, 4) == "TO: ")
    {
      topic = sF("outside/temperature");
      otvalue = otmessage.substring(4);
      wdresettimeout = 0;
      otgwcommstate = 2;
    }

    else if (otmessage.substring(0, 4) == "CS: ")
    {
      topic = sF("otgw/ch/water/setpoint");
      otvalue = otmessage.substring(4);
      wdresettimeout = 0;
      otgwcommstate = 2;
    }

    else if (otmessage.substring(0, 4) == "MM: ")
    {
      topic = sF("otgw/maxmodulationlevel");
      otvalue = otmessage.substring(4);
      wdresettimeout = 0;
      otgwcommstate = 2;
    }

    else if (otmessage.substring(0, 4) == "OT: ")
    {
      topic = sF("otgw/outsidetemperature");
      otvalue = otmessage.substring(4);
      wdresettimeout = 0;
      otgwcommstate = 2;
    }

    else if (otmessage[0] == 'B')
    {
      if (otgwcommstate < 1) otgwcommstate = 1;
      long otmsgtype = (strtol(otmessage.substring(1, 3).c_str(), 0, 16) >> 4) & 7;
      if (otmsgtype == 4) // Check if messagetype is read ack
      {
        if (otgwcommstate < 1) otgwcommstate = 1;
        long otcommand = strtol(otmessage.substring(3, 5).c_str(), 0, 16);
        switch (otcommand)
        {
          case 0:
            otintvalue = opentherm_message_touint(otmessage);
            putdatamap(cF("ch/active"), (otintvalue & 0x0002) ? "1" : "0");
            putdatamap(cF("dhw/active"), (otintvalue & 0x0004) ? "1" : "0");
            putdatamap(cF("burner/active"), (otintvalue & 0x0008) ? "1" : "0");
            break;
          case 17:
            otvalue = String(opentherm_message_tofloat(otmessage), 1);
            topic = sF("burner/modulation/level");
            break;
          case 116:
            otvalue = String(opentherm_message_touint(otmessage));
            topic = sF("burner/starts");
            break;
          case 120:
            otvalue = String(opentherm_message_touint(otmessage));
            topic = sF("burner/hours");
            break;
          case 19:
            otvalue = String(opentherm_message_tofloat(otmessage), 1);
            topic = sF("dhw/flowrate");
            break;
          case 26:
            otvalue = String(opentherm_message_tofloat(otmessage), 1);
            topic = sF("dhw/temperature");
            break;
          case 118:
            otvalue = String(opentherm_message_touint(otmessage));
            topic = sF("dhw/pump/starts");
            break;
          case 122:
            otvalue = String(opentherm_message_touint(otmessage));
            topic = sF("dhw/pump/hours");
            break;
          case 119:
            otvalue = String(opentherm_message_touint(otmessage));
            topic = sF("dhw/burner/starts");
            break;
          case 123:
            otvalue = String(opentherm_message_touint(otmessage));
            topic = sF("dhw/burner/hours");
            break;
          case 25:
            otvalue = String(opentherm_message_tofloat(otmessage), 1);
            topic = sF("boiler/temperature");
            break;
          case 18:
            otvalue = String(opentherm_message_tofloat(otmessage), 1);
            topic = sF("ch/water/pressure");
            break;
          case 117:
            otvalue = String(opentherm_message_touint(otmessage));
            topic = sF("ch/pump/starts");
            break;
          case 121:
            otvalue = String(opentherm_message_touint(otmessage));
            topic = sF("ch/pump/hours");
            break;
          case 56:
            otvalue = String(opentherm_message_tofloat(otmessage), 1);
            topic = sF("dhw/setpoint");
            break;
          case 57:
            otvalue = String(opentherm_message_tofloat(otmessage), 1);
            topic = sF("dhw/maxsetpoint");
            break;
          case 28:
            otvalue = String(opentherm_message_tofloat(otmessage), 1);
            topic = sF("ch/returnwatertemperature");
            break;
          case 27:
            otvalue = String(opentherm_message_tofloat(otmessage), 1);
            topic = sF("outside/temperature");
            break;
          case 33:
            otvalue = String(opentherm_message_toint(otmessage));
            topic = sF("exhausttemperature");
            break;
        }
      }
    }
    else if (otmessage[0] == 'T')
    {
      long otmsgtype = (strtol(otmessage.substring(1, 3).c_str(), 0, 16) >> 4) & 7;
      if (otmsgtype == 1) // Check if messagetype is write data
      {
        if (otgwcommstate < 1) otgwcommstate = 1;
        long otcommand = strtol(otmessage.substring(3, 5).c_str(), 0, 16);
        switch (otcommand)
        {
          case 1:
            otvalue = String(opentherm_message_tofloat(otmessage), 1);
            topic = sF("thermostat/ch/water/setpoint");
            break;
          case 16:
            otvalue = String(opentherm_message_tofloat(otmessage), 1);
            topic = sF("thermostat/setpoint");
            break;
          case 24:
            otvalue = String(opentherm_message_tofloat(otmessage), 1);
            topic = sF("thermostat/temperature");
            break;
        }
      }
    }

    if (topic != "")
    {
      putdatamap(topic, otvalue);
      returnvalue++;
    }
  }

  yield();

  // If otgw chip doesn't respond to commands reset system
  if ((wdresettimeout > 0) && (millis() > wdresettimeout))
  {
    wdresettimeout = 0;
    resetstate = 1;
    otgwcommstate = 0;
  }

  static uint8_t oldotgwcommstate = -1;
  if (otgwcommstate != oldotgwcommstate)
  {
    oldotgwcommstate = otgwcommstate;
    putdatamap(cF("otgw/commstate"), String(otgwcommstate));
  }

  static uint32_t resettimer = 0;
  switch (resetstate)
  {
    case 1:
      digitalWrite(D5, 1);
      pinMode(D5, OUTPUT);
      resettimer = millis() + 100;
      resetstate++;
    break;
    case 2:
      if (resettimer < millis())
      {
        digitalWrite(D5, 0);
        resettimer = millis() + 100;        
        resetstate++;
      }
    break;
    case 3:
      if (resettimer < millis())
      {
        digitalWrite(D5, 1);
        resettimer = millis() + 2000;        
        resetstate++;
      }
    break;
    case 4:
      if (resettimer < millis())
      {
        opentherm_setchwatertemperature(-1);
        resettimer = millis() + 1000;        
        resetstate++;
      }
    break;
    case 5:
      if (resettimer < millis())
      {
        opentherm_setmaxmodulationlevel(-1);
        resettimer = millis() + 1000;        
        resetstate++;
      }
    break;
    case 6:
      if (resettimer < millis())
      {
        opentherm_setoutsidetemperature(-100);
        resettimer = millis() + 1000;        
        resetstate++;
      }
    break;
    default:
    break;
  }
  
  yield();
  
  return returnvalue;
}


void opentherm_setthermosttattemporary(double value)
{
  static double oldvalue = -1;
  if (oldvalue != value)
  {
    if (value < 0) value = oldvalue;
    if ((value > 10) && (value < 30))
    {
      oldvalue = value;
      opentherm_serialprint("TT=" + String(value, 1));
      wdresettimeout = millis() + 5000;
    }
  }
}

void opentherm_setthermosttatcontinue(double value)
{
  static double oldvalue = -1;
  if (oldvalue != value)
  {
    if (value < 0) value = oldvalue;
    if ((value > 10) && (value < 30))
    {
      oldvalue = value;
      opentherm_serialprint("TC=" + String(value, 1));
      wdresettimeout = millis() + 5000;
    }
  }
}

void opentherm_setchwatertemperature(int8_t value)
{
  static int8_t oldvalue = -1;
  if (oldvalue != value)
  {
    if (value < 0) value = oldvalue;
    if ((value >= 0) && (value <= 100))
    {
      oldvalue = value;
      opentherm_serialprint("CS=" + String(value));
      wdresettimeout = millis() + 5000;
    }
  }
}

void opentherm_setmaxmodulationlevel(int8_t value)
{
  
  static int8_t oldvalue = -1;
  if (oldvalue != value)
  {
    if (value < 0) value = oldvalue;
    if ((value > 0) && (value < 100)) opentherm_serialprint("MM=" + String(value));
    else opentherm_serialprint("MM=T");
    oldvalue = value;
    wdresettimeout = millis() + 5000;
  }
}

void opentherm_setoutsidetemperature(double value)
{
  static double oldvalue = -100;
  if (oldvalue != value)
  {
    if (value == -100) value = oldvalue;
    if ((value > -50) && (value < 60))
    {
      oldvalue = value;
      opentherm_serialprint("OT=" + String(value, 1));
      wdresettimeout = millis() + 5000;
    }
  }
}
