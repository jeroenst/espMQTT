#include <ESP8266WiFi.h>
WiFiServer otserver(25238);

void(*_opentherm_callback)(String,String);


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

void opentherm_init(void(*callback)(String,String))
{
   _opentherm_callback = callback;
   otserver.begin();

  Serial.setRxBufferSize(2048); 
  Serial.begin(9600);  //Init serial 9600 baud
  Serial.setDebugOutput(false);
}

int opentherm_handle()
{
  static WiFiClient otclient;
  static String serialmessage = "";
  int returnvalue = 0;
  if (!otclient || !otclient.connected()) otclient = otserver.available();
  else
  {
    while (otclient.available())
    {
      yield();
      Serial.print((char)otclient.read());
    }
  }

  while (Serial.available() > 0) {
    yield();
    char otchar = char(Serial.read());
    serialmessage += otchar;
    if (otclient && otclient.connected())
    {
      otclient.print(otchar);
    }
  }


  while (serialmessage.indexOf(10) >= 0)
  {
    yield();
    int eolchar = serialmessage.indexOf(10);
    String otmessage = serialmessage.substring(0, eolchar);
    serialmessage = serialmessage.substring(eolchar + 1);
    String otvalue = "";
    String topic = "";

    if (otmessage.substring(0, 4) == "TC: ")
    {
      topic = "thermostat/setpoint";
      otvalue = String(otmessage.substring(4).toFloat(),1);
    }

    else if (otmessage.substring(0, 4) == "TT: ")
    {
      topic = "thermostat/setpoint";
      otvalue = String(otmessage.substring(4).toFloat(),1);
    }

    else if (otmessage.substring(0, 4) == "TO: ")
    {
      topic = "outside/temperature";
      otvalue = String(otmessage.substring(4).toFloat(),1);
    }

    else if (otmessage.substring(0, 4) == "CS: ")
    {
      topic = "otgw/ch/water/setpoint";
      otvalue = String(otmessage.substring(4).toFloat(),1);
    }

    else if (otmessage[0] == 'B')
    {
      long otcommand = strtol(otmessage.substring(3, 5).c_str(), 0, 16);
      switch (otcommand)
      {
        case 14:
          otvalue = String(opentherm_message_tofloat(otmessage), 1);
          topic = "burner/modulation/maxlevel";
          break;
        case 17:
          otvalue = String(opentherm_message_tofloat(otmessage), 1);
          topic = "burner/modulation/level";
          break;
        case 116:
          otvalue = String(opentherm_message_touint(otmessage));
          topic = "burner/starts";
          break;
        case 120:
          otvalue = String(opentherm_message_touint(otmessage));
          topic = "burner/hours";
          break;
        case 19:
          otvalue = String(opentherm_message_tofloat(otmessage), 1);
          topic = "dhw/flowrate";
          break;
        case 26:
          otvalue = String(opentherm_message_tofloat(otmessage), 1);
          topic = "dhw/temperature";
          break;
        case 118:
          otvalue = String(opentherm_message_touint(otmessage));
          topic = "dhw/pump/starts";
          break;
        case 122:
          otvalue = String(opentherm_message_touint(otmessage));
          topic = "dhw/pump/hours";
          break;
        case 119:
          otvalue = String(opentherm_message_touint(otmessage));
          topic = "dhw/burner/starts";
          break;
        case 123:
          otvalue = String(opentherm_message_touint(otmessage));
          topic = "dhw/burner/hours";
          break;
        case 25:
          otvalue = String(opentherm_message_tofloat(otmessage), 1);
          topic = "boiler/temperature";
          break;
        case 18:
          otvalue = String(opentherm_message_tofloat(otmessage), 1);
          topic = "ch/water/pressure";
          break;
        case 117:
          otvalue = String(opentherm_message_touint(otmessage));
          topic = "ch/pump/starts";
          break;
        case 121:
          otvalue = String(opentherm_message_touint(otmessage));
          topic = "ch/pump/hours";
          break;
        case 56:
          otvalue = String(opentherm_message_tofloat(otmessage), 1);
          topic = "dhw/setpoint";
          break;
        case 57:
          otvalue = String(opentherm_message_tofloat(otmessage), 1);
          topic = "dhw/maxsetpoint";
          break;
        case 28:
          otvalue = String(opentherm_message_tofloat(otmessage), 1);
          topic = "ch/returnwatertemperature";
          break;
        case 27:
          otvalue = String(opentherm_message_tofloat(otmessage), 1);
          topic = "outside/temperature";
          break;
        case 33:
          otvalue = String(opentherm_message_toint(otmessage));
          topic = "exhausttemperature";
          break;
      }
    }
    else if (otmessage[0] == 'T')
    {
      long otcommand = strtol(otmessage.substring(3, 5).c_str(), 0, 16);
      switch (otcommand)
      {
        case 1:
          otvalue = String(opentherm_message_tofloat(otmessage), 1);
          topic = "thermostat/ch/water/setpoint";
          break;
        case 16:
          otvalue = String(opentherm_message_tofloat(otmessage), 1);
          topic = "thermostat/setpoint";
          break;
        case 24:
          otvalue = String(opentherm_message_tofloat(otmessage), 1);
          topic = "thermostat/temperature";
          break;
      }
    }

    if (topic != "")
    {
      _opentherm_callback(topic, otvalue);
      returnvalue++;
    }
  }

  return returnvalue;
}

void opentherm_setthermosttattemporary(double value)
{
    Serial.print("TT=");
    Serial.print(String(value,1));
    Serial.print("\r\n");
}

void opentherm_setthermosttatcontinue(double value)
{
    Serial.print("TC=");
    Serial.print(String(value,1));
    Serial.print("\r\n");
}

void opentherm_setchwatertemperature(double value)
{
    Serial.print("CS=");
    Serial.print(String(value,1));
    Serial.print("\r\n");
}
