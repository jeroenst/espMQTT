#include "espMQTT.h"
#include "smartmeter.h"

#define SMARTMETER_BUFFER_SIZE 100

void(*_smartmeter_callback)(const char*,const String&);

void smartmeter_init(void(*callback)(const char *, const String&))
{
  _smartmeter_callback = callback;
  

  Serial.setRxBufferSize(1000);
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
  static char buffer[SMARTMETER_BUFFER_SIZE];
  static uint8_t bufpos = 0;
  static int watt = 0;

  while (Serial.available()) {
    char input = Serial.read() & 127;
    // Fill buffer up to and including a new line (\n)
    if (bufpos < SMARTMETER_BUFFER_SIZE)
    {
      if (input != '\r')
      {
        buffer[bufpos] = input;
        bufpos++;    
      }
    }
    else
    {
      if (Debug.isActive(Debug.ERROR)) Debug.printf("SERIAL BUFFER OVERFLOW\n"); 
      bufpos = 0;
    }
    buffer[bufpos] = 0;

    if ((input == '\n') && (bufpos > 0))
    { // We received a new line (data up to \n)
      buffer[--bufpos] = 0; // Remove newline character

      if (Debug.isActive(Debug.VERBOSE)) Debug.printf("RECEIVED FROM SERIAL:'%s'\n", buffer);

      if (buffer[0] == '/')
      {
        kwh = 0;
        watt = 0;
        _smartmeter_callback(cF("status"), sF("receiving"));
      }
      
      else if (buffer[0] == '!')
      {
        _smartmeter_callback(cF("electricity/watt"), String(watt));
        _smartmeter_callback(cF("electricity/kwh_total"), String(kwh,3));
        _smartmeter_callback(cF("status"), sF("ready"));
      }
      
      else if ((buffer[1] != '-') && (buffer[1] != 0)) 
      { 
        if (Debug.isActive(Debug.ERROR)) Debug.printf("SERIAL RECEIVE ERROR\n"); 
      }

      // 1-0:1.8.1 = Electricity low tarif used
      else if (sscanf(buffer, "1-0:1.8.1(%f" , &value) == 1)
      {
        kwh += value;
        _smartmeter_callback(cF("electricity/kwh_used1"), String(value, 3));
        returnvalue++;
      }

      // 1-0:1.8.2 = Electricity high tarif used (DSMR v4.0)
      else if (sscanf(buffer, "1-0:1.8.2(%f" , &value) == 1)
      {
        kwh += value;
        _smartmeter_callback(cF("electricity/kwh_used2"), String(value, 3));
        returnvalue++;
      }

      // 1-0:2.8.1 = Electricity low tarif provided
      else if (sscanf(buffer, "1-0:2.8.1(%f" , &value) == 1)
      {
        kwh -= value;
        _smartmeter_callback(cF("electricity/kwh_provided1"), String(value, 3));
        returnvalue++;
      }

      // 1-0:2.8.2 = Electricity high tarif provided (DSMR v4.0)
      else if (sscanf(buffer, "1-0:2.8.2(%f" , &value) == 1)
      {
        kwh -= value;
        _smartmeter_callback(cF("electricity/kwh_provided2"), String(value, 3));
        returnvalue++;
      }

      // 1-0:1.7.0 = Electricity actual usage (DSMR v4.0)
      else if (sscanf(buffer, "1-0:1.7.0(%f" , &value) == 1)
      {
        _smartmeter_callback(cF("electricity/kw_using"), String(value, 3));
        watt += value * 1000;
        returnvalue++;
      }

      // 1-0:2.7.0 = Electricity actual providing (DSMR v4.0)
      else if (sscanf(buffer, "1-0:2.7.0(%f" , &value) == 1)
      {
        _smartmeter_callback(cF("electricity/kw_providing"), String(value, 3));
        watt -= value * 1000;
        returnvalue++;
      }

      // 1-0:32.7.0 = Electricity voltage l1 (DSMR v5.0)
      else if (sscanf(buffer, "1-0:32.7.0(%f" , &value) == 1)
      {
        _smartmeter_callback(cF("electricity/voltage/l1"), String(value, 1));
        returnvalue++;
      }

      // 1-0:52.7.0 = Electricity voltage l2 (DSMR v5.0)
      else if (sscanf(buffer, "1-0:52.7.0(%f" , &value) == 1)
      {
        _smartmeter_callback(cF("electricity/voltage/l2"), String(value, 1));
        returnvalue++;
      }

      // 1-0:72.7.0 = Electricity voltage l3 (DSMR v5.0)
      else if (sscanf(buffer, "1-0:72.7.0(%f" , &value) == 1)
      {
        _smartmeter_callback(cF("electricity/voltage/l3"), String(value, 1));
        returnvalue++;
      }


      // 1-0:31.7.0 = Electricity current l1 (DSMR v5.0)
      else if (sscanf(buffer, "1-0:31.7.0(%f" , &value) == 1)
      {
        _smartmeter_callback(cF("electricity/current/l1"), String(value, 1));
        returnvalue++;
      }

      // 1-0:51.7.0 = Electricity current l2 (DSMR v5.0)
      else if (sscanf(buffer, "1-0:51.7.0(%f" , &value) == 1)
      {
        _smartmeter_callback(cF("electricity/current/l2"), String(value, 1));
        returnvalue++;
      }

      // 1-0:71.7.0 = Electricity current l3 (DSMR v5.0)
      else if (sscanf(buffer, "1-0:71.7.0(%f" , &value) == 1)
      {
        _smartmeter_callback(cF("electricity/current/l3"), String(value, 1));
        returnvalue++;
      }

      // 0-1:24.2.1 = Gas (DSMR v4.0)
      else if (sscanf(buffer, "0-1:24.2.1(%2d%2d%2d%2d%2d%2d%c)(%f", &day, &month, &year, &hour, &minute, &second, &summerwinter, &value) == 8)
      {
        _smartmeter_callback(cF("gas/m3"), String(value, 3));
        char gasdatetime[20];
        sprintf(gasdatetime, "%02d-%02d-%02d %d:%02d:%02d", day, month, year, hour, minute, second);
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
