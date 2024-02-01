#include "espMQTT.h"
#include "smartmeter.h"

void(*_smartmeter_callback)(const char*,const String&);

void smartmeter_init(void(*callback)(const char *, const String&))
{
  _smartmeter_callback = callback;
  

  Serial.setRxBufferSize(100); 
  Serial.begin(115200);  //Init serial 115200 baud
  Serial.setDebugOutput(false);

  U0C0 = BIT(UCRXI) | BIT(UCBN) | BIT(UCBN + 1) | BIT(UCSBN); // Inverse RX

}

int8_t smartmeter_handle()
{
  float value = 0;
  static float kwh = 0;
  int8_t returnvalue = 0;
  int day, month, year, hour, minute, second;
  char summerwinter;
  static char buffer[100];
  static uint8_t bufpos = 0;
  static int watt = 0;

  while (Serial.available()) {
    char input = Serial.read() & 127;
    // Fill buffer up to and including a new line (\n)
    if (bufpos < 100)
    {
      buffer[bufpos] = input;
      bufpos++;
    }
    buffer[bufpos] = 0;

    if (input == '\n')
    { // We received a new line (data up to \n)
      buffer[bufpos - 1] = 0; // Remove newline character
//      if (Debug.isActive(Debug.VERBOSE)) {
//        DEBUG("RECEIVED FROM SERIAL:%s\n", buffer);
//      }
      if (buffer[0] == '/')
      {
        kwh = 0;
        _smartmeter_callback(cF("status"), sF("receiving"));
      }

      if (buffer[0] == '!')
      {
        _smartmeter_callback(cF("electricity/watt"), String(watt));
        _smartmeter_callback(cF("electricity/kwh_total"), String(kwh,3));
        watt = 0;
        _smartmeter_callback(cF("status"), sF("ready"));
      }


      // 1-0:1.8.1 = Electricity low tarif used
      if (sscanf(buffer, cF("1-0:1.8.1(%f") , &value) == 1)
      {
        kwh += value;
        _smartmeter_callback(cF("electricity/kwh_used1"), String(value, 3));
        returnvalue++;
      }

      // 1-0:1.8.2 = Electricity high tarif used (DSMR v4.0)
      if (sscanf(buffer, cF("1-0:1.8.2(%f") , &value) == 1)
      {
        kwh += value;
        _smartmeter_callback(cF("electricity/kwh_used2"), String(value, 3));
        returnvalue++;
      }

      // 1-0:2.8.1 = Electricity low tarif provided
      if (sscanf(buffer, cF("1-0:2.8.1(%f") , &value) == 1)
      {
        kwh -= value;
        _smartmeter_callback(cF("electricity/kwh_provided1"), String(value, 3));
        returnvalue++;
      }

      // 1-0:2.8.2 = Electricity high tarif provided (DSMR v4.0)
      if (sscanf(buffer, cF("1-0:2.8.2(%f") , &value) == 1)
      {
        kwh -= value;
        _smartmeter_callback(cF("electricity/kwh_provided2"), String(value, 3));
        returnvalue++;
      }

      // 1-0:1.7.0 = Electricity actual usage (DSMR v4.0)
      if (sscanf(buffer, cF("1-0:1.7.0(%f") , &value) == 1)
      {
        _smartmeter_callback(cF("electricity/kw_using"), String(value, 3));
        watt += value * 1000;
        returnvalue++;
      }

      // 1-0:2.7.0 = Electricity actual providing (DSMR v4.0)
      if (sscanf(buffer, cF("1-0:2.7.0(%f") , &value) == 1)
      {
        _smartmeter_callback(cF("electricity/kw_providing"), String(value, 3));
        watt -= value * 1000;
        returnvalue++;
      }

      // 0-1:24.2.1 = Gas (DSMR v4.0)
      if (sscanf(buffer, cF("0-1:24.2.1(%2d%2d%2d%2d%2d%2d%c)(%f"), &day, &month, &year, &hour, &minute, &second, &summerwinter, &value) == 8)
      {
        _smartmeter_callback(cF("gas/m3"), String(value, 3));
        char gasdatetime[20];
        sprintf(gasdatetime, cF("%02d-%02d-%02d %d:%02d:%02d"), day, month, year, hour, minute, second);
        _smartmeter_callback(cF("gas/datetime"), String(gasdatetime));
        returnvalue += 2;

        static uint8_t oldhour = 255;
        static float oldgas = 0;
        if (oldhour != hour)
        {
            oldhour = hour;
            float gasm3h = value - oldgas;
            if (oldgas == 0) _smartmeter_callback(cF("gas/m3h"), "-");
            else _smartmeter_callback(cF("gas/m3h"), String(gasm3h, 3));
            oldgas = value;
        }
      }

      buffer[0] = 0;
      bufpos = 0;
    }
    yield();
  }
  return returnvalue;
}
