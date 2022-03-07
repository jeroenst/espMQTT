#include "espMQTT.h"
#include "ducobox.h"
#define ducobox_setfan_internal_count 2

WiFiServer ducoserver(2233);
static uint8_t _ducobox_fanspeedoverride = 255;
static uint8_t _ducobox_relay0 = 0;
static uint8_t _ducobox_relay1 = 0;
static uint8_t _ducobox_refreshdelay = 5;
static uint8_t _ducobox_refreshtimeout = 30;
static uint16_t _ducobox_co2 = 0;
static uint8_t _ducobox_rh = 0;
static float _ducobox_temp = 0;

static String _serialsendqueue = "";
static unsigned long _nextupdatetime = 0;
static bool recvok = 1;

void(*_ducobox_callback)(const char *, String);

void _ducobox_writeserialqueue()
// The ducobox can't handle serial data at full speed, so send char followed by a pause before next char
{
  static unsigned long nextsend = 0;
  if (nextsend < millis())
  {
    if (_serialsendqueue.length() > 0)
    {
      Serial.print (_serialsendqueue[0]);
      _serialsendqueue.remove(0, 1);
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
  if ((value != 255) && (value > 100)) value = 100;

  DEBUG_V ("ducobox_setfan_internal: id=%d, value=%d\n", id, value);
  static uint8_t newfanspeed = 255;
  static uint8_t fanspeed[ducobox_setfan_internal_count] = {255, 255};
  if (id < ducobox_setfan_internal_count)
  {
    fanspeed[id] = value;
    _ducobox_fanspeedoverride = 0;

    for (uint8_t i = 0; i < ducobox_setfan_internal_count; i++)
    {
      if (fanspeed[i] > _ducobox_fanspeedoverride) _ducobox_fanspeedoverride = fanspeed[i];
      if (newfanspeed == 255) _ducobox_fanspeedoverride = fanspeed[i];
    }
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

  if (ducocmd == 0)
  {
    int stringpos = ducomessage.indexOf("  FanSpeed:");
    if (stringpos == 0)
    {
      _ducobox_callback("status", "querying");
      recvok = 1;

      // Read fanspeed
      ducovalue = ducomessage.substring(19);
      ducovalue = ducovalue.substring(0, ducovalue.indexOf(' '));

      _ducobox_callback("1/fanspeed", ducovalue);
      //ducocmd = 1;
    }
  }

  if (ducocmd == 1)
  {
    int stringpos = ducomessage.indexOf("   2. MIN FAN SPEED");
    if (stringpos == 0)
    {
      ducovalue = ducomessage.substring(ducomessage.indexOf(":") + 2);
      ducovalue = ducovalue.substring(0, ducovalue.indexOf(" ["));
      _ducobox_callback("1/minfanspeed", ducovalue);
      //ducocmd = 2;
    }
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
        if (tempretry == 30) _ducobox_callback("2/temperature", "-");
        _ducobox_callback("2/temperature/retries", String(tempretry));

        //      ducocmd = 3;
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
        if (co2retry == 30)
        {
          _ducobox_callback("2/co2", "-");
          _ducobox_co2 = 0;
        }
        _ducobox_callback("2/co2/retries", String(co2retry));

        //        ducocmd = 4;
        break;
    }
  }

  if (ducocmd == 4)
  {
    int stringpos = ducomessage.indexOf("RH : ");
    if (stringpos > 0)
    {
      _ducobox_rh = ducomessage.substring(stringpos + 5, stringpos + 5 + 4).toInt() / 100;
      _ducobox_callback ("26/humidity", String(_ducobox_rh));
    }

    stringpos = ducomessage.indexOf("TEMP : ");
    if (stringpos > 0)
    {
      _ducobox_temp = float(ducomessage.substring(stringpos + 8, stringpos + 8 + 3).toInt()) / 10;
      _ducobox_callback ("26/temperature", String(_ducobox_temp, 1));
    }
  }

  if (ducomessage == "> ")
  {
    ducocmd++;
    switch (ducocmd)
    {
      case 1:
        ducobox_writeserial("fanparaget");             // Get fan parameters
        break;
      case 2:
        ducobox_writeserial("nodeparaget 4 73");       // Request TEMPERATURE of node 3
        break;
      case 3:
        ducobox_writeserial("nodeparaget 4 74");       // Request CO2 of sensor 3
        break;
      case 4:
        ducobox_writeserial("sensorinfo");       // Request internal sensors
        break;
      case 5:
        static uint8_t fanspeed = 0;
        if (_ducobox_co2 < 800) fanspeed = 255; // 255 = auto
        else if (_ducobox_co2 > 1200) fanspeed = 100;
        else fanspeed = ((_ducobox_co2 - 800) / 40) * 10;
        if (fanspeed > 100) fanspeed = 100;
        ducobox_setfan_internal(0, fanspeed);

        static uint8 _ducobox_fanspeedoverride_old = 255;
        if (_ducobox_fanspeedoverride != _ducobox_fanspeedoverride_old)
        {
          _ducobox_fanspeedoverride_old = _ducobox_fanspeedoverride;
          String cmd = "nodesetoverrule 1 " + String(_ducobox_fanspeedoverride) + "\r\n";
          ducobox_writeserial(cmd.c_str());
        }
        else
        {
          _ducobox_callback("status", "ready");
          ducocmd = 0;
          _nextupdatetime = millis() + (_ducobox_refreshdelay * 1000); // Next update over _duco_refreshtime seconds
        }
        break;
      case 6:
        _ducobox_callback("status", "ready");
        ducocmd = 0;
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
      if (ducochar == 13) ducoclient.print(char(10));
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
      if (recvok == 0)   _ducobox_callback("status", "commerror");
      _nextupdatetime = millis() + (_ducobox_refreshtimeout * 1000); // Next update over _duco_refreshtime seconds
      ducobox_writeserial("fanspeed");                // Request fanspeed
      recvok = 0;
    }
    _ducobox_writeserialqueue();  // Send queued serial data
  }
}

void ducobox_init(uint8_t ducobox_relay0, uint8_t ducobox_relay1, uint8_t ducobox_refreshtimeout, void(*callback)(const char *, String))
{
  _ducobox_callback = callback;
  _ducobox_relay0 = ducobox_relay0;
  _ducobox_relay1 = ducobox_relay1;
  _ducobox_refreshtimeout = ducobox_refreshtimeout;
  Serial.setRxBufferSize(1000);
  Serial.setDebugOutput(false);
  Serial.begin(115200); //Init serial 115200 baud
  ducoserver.begin();
  ducobox_writeserial(""); // Clear bogus
  _ducobox_callback("1/fanspeed", "-");
  _ducobox_callback("1/minfanspeed", "-");
  _ducobox_callback("26/humidity", "-");
  _ducobox_callback("26/temperature", "-");
  _ducobox_callback("2/co2", "-");
  _ducobox_callback("2/co2/retries", "-");
  _ducobox_callback("2/temperature", "-");
  _ducobox_callback("2/temperature/retries", "-");
}
