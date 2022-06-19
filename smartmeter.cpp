#include "espMQTT.h"
#include "smartmeter.h"

void(*_smartmeter_callback)(void);
Smartmeter_DataMap_Struct smartmeter_DataMap;

void smartmeter_init(void(*callback)(void))
{
  _smartmeter_callback = callback;
  
  Serial.setRxBufferSize(100); 
  Serial.begin(115200);  //Init serial 115200 baud
  Serial.setDebugOutput(false);

  U0C0 = BIT(UCRXI) | BIT(UCBN) | BIT(UCBN + 1) | BIT(UCSBN); // Inverse RX
}

int8_t smartmeter_handle()
{
  static int32_t wh = 0;
  int8_t returnvalue = 0;
  int day, month, year, hour, minute, second;
  char summerwinter;
  uint32_t uintvalue = 0; 
  uint32_t uintdecimals = 0;
  static char buffer[100];
  static uint8_t bufpos = 0;
  static int16_t watt = 0;


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
        DEBUG("RECEIVED FROM SERIAL:%s\n", buffer);
//      }
      if (buffer[0] == '/')
      {
        wh = 0;
        smartmeter_DataMap.status = receiving;
      }

      if (buffer[0] == '!')
      {
        if (watt != smartmeter_DataMap.electricity.watt)
        {
          smartmeter_DataMap.electricity.watt = watt;
          smartmeter_DataMap.electricity.changed.watt = 1;
        }  
        if (wh != smartmeter_DataMap.electricity.wh)
        {
          smartmeter_DataMap.electricity.wh = wh;
          smartmeter_DataMap.electricity.changed.wh = wh;
        }
        smartmeter_DataMap.status = ready;
        _smartmeter_callback();
        watt = 0;
      }


      // 1-0:1.8.1 = Electricity low tarif used
      if (sscanf(buffer, "1-0:1.8.1(%u.%u" , &uintvalue, &uintdecimals) == 2)
      {
        uintvalue = (uintvalue * 1000) + uintdecimals;
        wh += uintvalue;
        if (smartmeter_DataMap.electricity.wh_used1 != uintvalue)
        {
          smartmeter_DataMap.electricity.wh_used1 = uintvalue;
          smartmeter_DataMap.electricity.changed.wh_used1 = 1;
        }
        returnvalue++;
      }

      // 1-0:1.8.2 = Electricity high tarif used (DSMR v4.0)
      if (sscanf(buffer, "1-0:1.8.2(%u.%u" , &uintvalue, &uintdecimals) == 2)
      {
        uintvalue = (uintvalue * 1000) + uintdecimals;
        DEBUG ("Received wh_used2=%d\n", uintvalue);
        wh += uintvalue;
        if (smartmeter_DataMap.electricity.wh_used2 != uintvalue)
        {
          smartmeter_DataMap.electricity.wh_used2 = uintvalue;
          smartmeter_DataMap.electricity.changed.wh_used2 = 1;
        }
        returnvalue++;
      }

      // 1-0:2.8.1 = Electricity low tarif provided
      if (sscanf(buffer, "1-0:2.8.1(%u.%u" , &uintvalue, &uintdecimals) == 2)
      {
        uintvalue = (uintvalue * 1000) + uintdecimals;
        wh -= uintvalue;
        if (smartmeter_DataMap.electricity.wh_provided1 != uintvalue)
        {
          smartmeter_DataMap.electricity.wh_provided1 = uintvalue;
          smartmeter_DataMap.electricity.changed.wh_provided1 = 1;
        }
        returnvalue++;
      }

      // 1-0:2.8.2 = Electricity high tarif provided (DSMR v4.0)
      if (sscanf(buffer, "1-0:2.8.2(%u.%u" , &uintvalue, &uintdecimals) == 2)
      {
        uintvalue = uintvalue * 1000 + uintdecimals;
        wh -= uintvalue;
        if (smartmeter_DataMap.electricity.wh_provided2 != uintvalue)
        {
          smartmeter_DataMap.electricity.wh_provided2 = uintvalue;
          smartmeter_DataMap.electricity.changed.wh_provided2 = 1;
        }
        returnvalue++;
      }

      // 1-0:1.7.0 = Electricity actual usage (DSMR v4.0)
      if (sscanf(buffer, "1-0:1.7.0(%u.%u" , &uintvalue, &uintdecimals) == 2)
      {
        uintvalue = uintvalue * 1000 + uintdecimals;
        watt = uintvalue;
        if (smartmeter_DataMap.electricity.watt_using != uintvalue)
        {
          smartmeter_DataMap.electricity.watt_using = uintvalue;
          smartmeter_DataMap.electricity.changed.watt_using = 1;
        }
        returnvalue++;
      }

      // 1-0:2.7.0 = Electricity actual providing (DSMR v4.0)
      if (sscanf(buffer, "1-0:2.7.0(%u.%u" , &uintvalue, &uintdecimals) == 2)
      {
        DEBUG("Providing: %u.%u\n", uintvalue, uintdecimals);
        uintvalue = uintvalue * 1000 + uintdecimals;
        watt -= uintvalue;
        if ((smartmeter_DataMap.electricity.watt != watt))
        {
          smartmeter_DataMap.electricity.watt = watt;
          smartmeter_DataMap.electricity.changed.watt = 1;
        }
        if (smartmeter_DataMap.electricity.watt_providing != uintvalue)
        {
          smartmeter_DataMap.electricity.watt_providing = uintvalue;
          smartmeter_DataMap.electricity.changed.watt_using = 1;
        }
        returnvalue++;
      }

      // 0-1:24.2.1 = Gas (DSMR v4.0)
      if (sscanf(buffer, "0-1:24.2.1(%2d%2d%2d%2d%2d%2d%c)(%d.%d", &day, &month, &year, &hour, &minute, &second, &summerwinter, &uintvalue, &uintdecimals) == 9)
      {
        uintvalue = uintvalue * 1000 + uintdecimals;
        if (smartmeter_DataMap.gas.liter != uintvalue)
        {
          smartmeter_DataMap.gas.liter = uintvalue;
          smartmeter_DataMap.gas.changed.liter = 1;
        }

        char datetime[20];
        sprintf(datetime, "%02d-%02d-%02d %d:%02d:%02d", day, month, year, hour, minute, second);
        if (strcmp (datetime, smartmeter_DataMap.gas.datetime) != 0)
        {
          strcpy (smartmeter_DataMap.gas.datetime, datetime);
          smartmeter_DataMap.gas.changed.datetime = 1;
        }
        returnvalue += 2;

        static uint8_t oldhour = 255;
        static uint32_t oldgas = 0;
        if (oldhour != hour)
        {
            uintvalue = (uintvalue - oldgas);
            if (uintvalue != smartmeter_DataMap.gas.lh)
            {
              smartmeter_DataMap.gas.lh = uintvalue;
              smartmeter_DataMap.gas.changed.lh = 1;
            }
            oldhour = hour;
            oldgas = uintvalue;
        }
      }

      buffer[0] = 0;
      bufpos = 0;
    }
    yield();
  }
  return returnvalue;
}
