#include <ESP8266WiFi.h>
#include "ducobox.h"
WiFiServer ducoserver(2233);
static uint16_t _ducobox_minfanspeed = 0;
static uint8_t _ducobox_relay0 = 0;
static uint8_t _ducobox_relay1 = 0;
static uint8_t _ducobox_refreshtime = 30;
static uint16_t _ducobox_co2 = 0;
static String _serialsendqueue = "";
static unsigned long _nextupdatetime = 0;

void(*_ducobox_callback)(String, String);

void _ducobox_writeserialqueue()
// The ducobox can't handle serial data at full speed, so send char followed by a pause before next char
{
  static unsigned long nextsend = 0;
  if (nextsend < millis())
  {
    if (_serialsendqueue.length() > 0)
    {
      Serial.print (_serialsendqueue[0]);
      _serialsendqueue.remove(0,1); 
    }
    nextsend = millis() + 50;
  }
}

void ducobox_writeserial(String message)
{
  DEBUG_V ("ducobox_writeserial: %s\n", message.c_str());
  _serialsendqueue += message + "\r";
}

void ducobox_setfan_internal(uint8_t id, uint8_t value)
{
  DEBUG_V ("ducobox_setfan_internal: id=%d, value=%d\n", id, value);
#define ducobox_setfan_internal_count 2
  static uint8_t fanspeed[ducobox_setfan_internal_count] = {0, 0};
  if (id < ducobox_setfan_internal_count)
  {
    fanspeed[id] = value;
    uint8_t newfanspeed = 0;

    for (uint8_t i = 0; i < ducobox_setfan_internal_count; i++)
    {
      if (fanspeed[i] > newfanspeed) newfanspeed = fanspeed[i];
    }

    digitalWrite(_ducobox_relay0, newfanspeed == 1 ? 1 : 0);
    digitalWrite(_ducobox_relay1, newfanspeed > 1 ? 1 : 0);
  }
}

void ducobox_setfan(uint8_t value)
{
  DEBUG_V ("ducobox_setfan: value=%d\n", value);
  ducobox_setfan_internal(1, value);
}


void _ducobox_handleserial(String ducomessage)
{
  static uint8_t ducocmd = 0;
  String topic = "";
  String ducovalue = "";

  DEBUG_V ("ducomessage='%s'\n", ducomessage.c_str());

  if (ducomessage.indexOf("  FanSpeed:") == 0)
  {
    // Read fanspeed
    ducovalue = ducomessage.substring(19);
    ducovalue = ducovalue.substring(0, ducovalue.indexOf(' '));

    topic = "1/fanspeed";
    _ducobox_callback(topic, ducovalue);
    ducocmd = 1;
  }

  if (ducomessage.indexOf("   2. MIN FAN SPEED") == 0)
  {
    ducovalue = ducomessage.substring(ducomessage.indexOf(":") + 2);
    ducovalue = ducovalue.substring(0, ducovalue.indexOf(" ["));
    topic = "1/minfanspeed";
    _ducobox_callback(topic, ducovalue);
    ducocmd = 2;
  }

  if ((ducomessage.indexOf("  -->") == 0) || (ducomessage.indexOf("  Failed") == 0))
  {
    if (ducomessage.indexOf("  Failed") == 0)
    {
      ducomessage = "";
    }
    switch (ducocmd)
    {
      case 2:
        // Read node 2 temperature
        static uint8_t tempretry = 0;
        if (ducomessage != "")
        {
          ducovalue = String(float(ducomessage.substring(6).toInt()) / 10, 1);
          _ducobox_callback("2/temperature", ducovalue);
          tempretry = 0;
        }
        else
        {
          tempretry++;
          if (tempretry == 0) tempretry--;
        }
        if (tempretry == 10) _ducobox_callback("2/temperature", "-");
        _ducobox_callback("2/temperature/retries", String(tempretry));

        ducocmd = 3;
        break;
      case 3:
        // Read node 2 co2
        static uint8_t co2retry = 0;
        if (ducomessage != "")
        {
          ducovalue = ducomessage.substring(6);
          _ducobox_co2 = ducovalue.toInt();
          _ducobox_callback("2/co2", ducovalue);
          co2retry = 0;
        }
        else
        {
          co2retry++;
          if (co2retry == 0) co2retry--;
        }
        if (co2retry == 10)
        {
          _ducobox_callback("2/co2", "-");
          _ducobox_co2 = 0;
        }
        _ducobox_callback("2/co2/retries", String(co2retry));

        ducocmd = 4;
        break;
    }
  }

  if (ducocmd == 4)
  {
    ducocmd = 5;
    _ducobox_callback("status", "ready");
    static uint8_t fanspeed = 0;
    switch (fanspeed)
    {
      case 0:
        if (_ducobox_co2 >= 900) fanspeed = 1;
        break;
      case 1:
        if (_ducobox_co2 < 800) fanspeed = 0;
        if (_ducobox_co2 >= 1000) fanspeed = 2;
        break;
      case 2:
        if (_ducobox_co2 < 900) fanspeed = 1;
        break;
    }
    if (_ducobox_co2 == 0) fanspeed = 1; // When no co2 reading do some extra ventilation to prevent high co2...

    ducobox_setfan_internal(0, fanspeed);

    /* Setting fanspeed by serial command is disabled because it can break the onboard flash of the ducobox when used to many times (>10.000)
      oldminfanspeed = setminfanspeed;
      String cmd = "fanparaset 2 " + String(setminfanspeed) + "\r\n";
      ducobox_writeserial(cmd.c_str());
      nextupdatetime = millis() + 2000; // Next update over 2 seconds
    */
  }

  if (ducomessage == "> ")
  {
    _nextupdatetime = millis() + (_ducobox_refreshtime * 1000); // Next update over _duco_refreshtime seconds
    switch (ducocmd)
    {
      case 1:
        ducobox_writeserial("fanparaget");             // Get fan parameters
        break;
      case 2:
        ducobox_writeserial("nodeparaget 2 73");       // Request TEMPERATURE of node 2
        break;
      case 3:
        ducobox_writeserial("nodeparaget 2 74");       // Request CO2 of sensor 2
        break;
    }
  }
}

// This function reads the ducobox fanspeed, node 2 co2 and node 2 temperature.
// It also can write the minimum fanspeed to the ducobox
// minfanspeed : this value will be written to the ducobox upon change
// return: 0 if no new data was received, 1 if new data is available
void ducobox_handle()
{
  static WiFiClient ducoclient;
  static String serialmessage = "";

  if (!ducoclient || !ducoclient.connected()) ducoclient = ducoserver.available();
  else
  {
    if (ducoclient.available())
    {
      yield();
      delay(10);
      Serial.print((char)ducoclient.read());
    }
  }


  if (ducoclient && ducoclient.connected())
  {
    if (Serial.available() > 0)
    {
      char ducochar = char(Serial.read());
      ducoclient.print(ducochar);
    }
  }
  else
  {
    if (Serial.available() > 0) 
    {
      char ducochar = char(Serial.read());
      if (ducochar != '\r') serialmessage += ducochar;
      if ((ducochar == '\r') || (serialmessage == "> "))
      {
        _ducobox_handleserial(serialmessage);
        serialmessage = "";
      }
      if (serialmessage.length() > 200) serialmessage = ""; // Prevent memory fill upon garbage data
    }

    if (millis() > _nextupdatetime)
    {
      _nextupdatetime = millis() + (_ducobox_refreshtime * 1000); // Next update over _duco_refreshtime seconds
      _ducobox_callback("status", "queuering");
      ducobox_writeserial("fanspeed");                // Request fanspeed
    }
    _ducobox_writeserialqueue();  // Send queued serial data
  }
}

void ducobox_init(uint8_t ducobox_relay0, uint8_t ducobox_relay1, uint8_t ducobox_refreshtime, void(*callback)(String, String))
{
  _ducobox_callback = callback;
  _ducobox_relay0 = ducobox_relay0;
  _ducobox_relay1 = ducobox_relay1;
  _ducobox_refreshtime = ducobox_refreshtime;
  Serial.setRxBufferSize(2048);
  Serial.setDebugOutput(false);
  Serial.begin(115200); //Init serial 115200 baud
  ducoserver.begin();
  ducobox_writeserial(""); // Clear bogus
  _ducobox_callback("1/fanspeed", "-");
  _ducobox_callback("1/minfanspeed", "-");
  _ducobox_callback("2/co2", "-");
  _ducobox_callback("2/co2/retries", "-");
  _ducobox_callback("2/temperature", "-");
  _ducobox_callback("2/temperature/retries", "-");
}
