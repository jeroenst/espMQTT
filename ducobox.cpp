#include <ESP8266WiFi.h>
WiFiServer ducoserver(2233);
static uint16_t _ducobox_minfanspeed = 0;
static uint8_t _ducobox_relay0 = 0;
static uint8_t _ducobox_relay1 = 0;
static uint8_t _ducobox_refreshtime = 30;
static uint16_t _ducobox_co2 = 0;

void(*_ducobox_callback)(String,String);

void ducobox_writeserial(const char *message)
{
  for (int i = 0; i < strlen(message); i++)
  {
    Serial.print (message[i]);
    yield();
    delay(10);
  }
}

void ducobox_setfan(uint8_t value)
{
  digitalWrite(_ducobox_relay0, value == 1 ? 1 : 0);
  digitalWrite(_ducobox_relay1, value > 1 ? 1 : 0);
}

// This function reads the ducobox fanspeed, node 2 co2 and node 2 temperature.
// It also can write the minimum fanspeed to the ducobox
// minfanspeed : this value will be written to the ducobox upon change
// return: 0 if no new data was received, 1 if new data is available
int ducobox_handle()
{
  static String serialmessage = "";
  static uint8_t ducocmd = 0;
  static unsigned long nextupdatetime = millis();
  int returnvalue = 0;
  String topic = "";
  String ducovalue = "";
  static bool firstrun = 1;

  if (firstrun)
  {
    _ducobox_callback("1/fanspeed", "-");
    _ducobox_callback("1/minfanspeed", "-");
    _ducobox_callback("2/co2", "-");
    _ducobox_callback("2/co2/retries", "-");
    _ducobox_callback("2/temperature", "-");
    _ducobox_callback("2/temperature/retries", "-");
    firstrun = 0;
  }

  static WiFiClient ducoclient;
  if (!ducoclient || !ducoclient.connected()) ducoclient = ducoserver.available();
  else
  {
    while (ducoclient.available())
    {
      yield();
      delay(10);
      Serial.print((char)ducoclient.read());
    }
  }

  while (Serial.available() > 0) {
    char ducochar = char(Serial.read());
    serialmessage += ducochar;
  }
  yield();

  if (ducoclient && ducoclient.connected())
  {
    for (int i = 0; i < serialmessage.length(); i++)
    {
      yield();
      char ducochar = serialmessage.charAt(i);
      ducoclient.print(ducochar);
      if (ducochar == '\r') ducoclient.print('\n');
    }
    serialmessage = "";
  }
  else
  {
    if (millis() > nextupdatetime)
    {
      nextupdatetime = millis() + (_ducobox_refreshtime * 1000); // Next update over _duco_refreshtime seconds
      if (!ducoclient.connected())
      {
        ducobox_writeserial("\r\nfanspeed\r\n");                // Request fanspeed
        ducocmd = 0;
      }
    }


    while (serialmessage.indexOf('\r') >= 0)
    {
      yield();
      int eolchar = serialmessage.indexOf('\r');
      String ducomessage = serialmessage.substring(0, eolchar);
      serialmessage = serialmessage.substring(eolchar + 1);


      if (ducomessage.indexOf("  FanSpeed:") == 0)
      {
        // Read fanspeed
        ducovalue = ducomessage.substring(19);
        ducovalue = ducovalue.substring(0, ducovalue.indexOf(' '));

        topic = "1/fanspeed";
        _ducobox_callback(topic, ducovalue);
        returnvalue = 1;
        ducobox_writeserial("fanparaget\r\n");       // Request TEMPERATURE of node 2
        ducocmd  = 1;
      }

      if (ducomessage.indexOf("   2. MIN FAN SPEED") == 0)
      {
        ducovalue = ducomessage.substring(ducomessage.indexOf(":") + 2);
        ducovalue = ducovalue.substring(0, ducovalue.indexOf(" ["));
        topic = "1/minfanspeed";
        _ducobox_callback(topic, ducovalue);
      }

      if ((ducocmd == 1) && (ducomessage.indexOf("> ") == 0))
      {
        ducobox_writeserial("nodeparaget 2 73\r\n");       // Request TEMPERATURE of node 2
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
            returnvalue = 1;

            ducobox_writeserial("nodeparaget 2 74\r\n");       // Request CO2 of sensor 2
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
            returnvalue = 1;

            ducocmd = 4;
            break;
        }
      }

      if (ducocmd == 4)
      {
        static uint8_t fanspeed = 0;
        switch (fanspeed)
        {
          case 0:
            if (_ducobox_co2 >= 1000) fanspeed = 1;
            break;
          case 1:
            if (_ducobox_co2 < 800) fanspeed = 0;
            if (_ducobox_co2 >= 1200) fanspeed = 2;
            break;
          case 2:
            if (_ducobox_co2 < 1000) fanspeed = 1;
            break;
        }
        if (_ducobox_co2 == 0) fanspeed = 1; // When no co2 reading do some extra ventilation to prevent high co2...

        ducobox_setfan(fanspeed);

        /* Setting fanspeed by serial command is disabled because it can break the onboard flash of the ducobox when used to many times (>10.000)
          oldminfanspeed = setminfanspeed;
          String cmd = "fanparaset 2 " + String(setminfanspeed) + "\r\n";
          ducobox_writeserial(cmd.c_str());
          nextupdatetime = millis() + 2000; // Next update over 2 seconds
        */
        nextupdatetime = millis() + (_ducobox_refreshtime * 1000); // Next update over _duco_refreshtime seconds
      }
    }
  }
  return returnvalue;
}

void ducobox_init(uint8_t ducobox_relay0, uint8_t ducobox_relay1, uint8_t ducobox_refreshtime, void(*callback)(String,String))
{
    _ducobox_callback = callback;
    _ducobox_relay0 = ducobox_relay0;
    _ducobox_relay1 = ducobox_relay1;
    _ducobox_refreshtime = ducobox_refreshtime;
    Serial.setRxBufferSize(2048); 
    Serial.begin(115200); //Init serial 115200 baud
    ducoserver.begin();
}
