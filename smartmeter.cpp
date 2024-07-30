#include "espMQTT.h"
#include "smartmeter.h"

#define SMARTMETER_BUFFER_SIZE 400

/*
 * CRC Calculation inherited from: https://github.com/jantenhove/P1-Meter-ESP8266/blob/master/P1Meter.ino
 * 
 */
unsigned int currentCRC=0;

static struct 
{
      struct 
      {
        struct
        {
          int32_t total = UINT32_MAX;
          struct
          {
            uint32_t t1 = UINT32_MAX;
            uint32_t t2 = UINT32_MAX;
          } fromgrid;
          struct
          {
            uint32_t t1 = UINT32_MAX;
            uint32_t t2 = UINT32_MAX;            
          } togrid;
        } kWh;
        struct
        {
          int16_t total = UINT16_MAX; // Watt
          struct
          {
            uint16_t total = UINT16_MAX; // Watt
            uint16_t l1 = UINT16_MAX; // Watt
            uint16_t l2 = UINT16_MAX; // Watt
            uint16_t l3 = UINT16_MAX; // Watt
          } fromgrid;
          struct
          {
            uint16_t total = UINT16_MAX; // Watt
            uint16_t l1 = UINT16_MAX; // Watt
            uint16_t l2 = UINT16_MAX; // Watt
            uint16_t l3 = UINT16_MAX; // Watt
          } togrid;
        } kW;
        struct
        {
          uint16_t l1 = UINT16_MAX; // deciVolt
          uint16_t l2 = UINT16_MAX; // deciVolt
          uint16_t l3 = UINT16_MAX; // deciVolt
        } V;
        struct
        {
          uint16_t l1 = UINT16_MAX; // centiAmps
          uint16_t l2 = UINT16_MAX; // centiAmps
          uint16_t l3 = UINT16_MAX; // centiAmps
        } A;
      } electricity;
      struct {
        uint32_t m3 = UINT32_MAX; // liter
        uint32_t oldm3 = UINT32_MAX; // liter
        uint16_t m3h = UINT16_MAX; // liter/hour
        char datetime[20] = ""; // date time
      } gas;
} smartmetervalues;


void(*_smartmeter_callback)(const char*,const String&);

String valuetodecimalstring(uint64_t value, uint8_t decimals)
{
  if (UINT64_MAX == value) return "-";
  return String(double(value) / pow(10,decimals), decimals);
}

String valuetodecimalstring(uint32_t value, uint8_t decimals)
{
  if (UINT32_MAX == value) return "-";
  return String(double(value) / pow(10,decimals), decimals);
}

String valuetodecimalstring(uint16_t value, uint8_t decimals)
{
  if (UINT16_MAX == value) return "-";
  return String(double(value) / pow(10,decimals), decimals);
}


String valuetodecimalstring(uint8_t value, uint8_t decimals)
{
  if (UINT8_MAX == value) return "-";
  return String(double(value) / pow(10,decimals), decimals);
}

String valuetodecimalstring(int64_t value, uint8_t decimals)
{
  if (INT64_MAX == value) return "-";
  return String(double(value) / pow(10,decimals), decimals);
}

String valuetodecimalstring(int32_t value, uint8_t decimals)
{
  if (INT32_MAX == value) return "-";
  return String(double(value) / pow(10,decimals), decimals);
}

String valuetodecimalstring(int16_t value, uint8_t decimals)
{
  if (INT16_MAX == value) return "-";
  return String(double(value) / pow(10,decimals), decimals);
}


String valuetodecimalstring(int8_t value, uint8_t decimals)
{
  if (INT8_MAX == value) return "-";
  return String(double(value) / pow(10,decimals), decimals);
}


unsigned int CRC16(unsigned int crc, unsigned char *buf, int len)
{
  for (int pos = 0; pos < len; pos++)
  {
    crc ^= (unsigned int)buf[pos];    // XOR byte into least sig. byte of crc

    for (int i = 8; i != 0; i--) {    // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }

  return crc;
}

void smartmeter_init(void(*callback)(const char *, const String&))
{
  _smartmeter_callback = callback;
  
  Serial.setRxBufferSize(400);
  Serial.begin(115200, SERIAL_8N1);

  U0C0 = BIT(UCRXI) | BIT(UCBN) | BIT(UCBN + 1) | BIT(UCSBN); // Inverse RX
}

int8_t smartmeter_handle()
/*
 * Return value:
 *  0 = Complete packet received ok, data is complete and ready to be used
 *  1 = Line received ok, packet not completed yet
 *  2 = CRC Error
 *  3 = Wrong serial characters received
 *  4 = Line not recognized
 *  5 = Buffer overflow
 */
{
  float value = 0;
  int8_t returnvalue = 0;
  static char buffer[SMARTMETER_BUFFER_SIZE];
  static uint16_t bufpos = 0;
  
  while (Serial.available()) 
  {
    char input = Serial.read() & 127;
    
    // Fill buffer up to and including a new line (\n)
    if (bufpos < SMARTMETER_BUFFER_SIZE - 1)
    {
        buffer[bufpos++] = input;
    }
    else
    {
      if (Debug.isActive(Debug.ERROR)) Debug.printf("SERIAL BUFFER OVERFLOW\n"); 
      bufpos = 0;
      buffer[bufpos] = 0;
      return 5;
    }
    buffer[bufpos] = 0;

    if ((input == '\n') && (bufpos > 1))
    { // We received a new line (data up to \n)
      bool validCRCFound = false;

      uint8_t nroflinefeeds = 0;

      if (strchr(buffer,'\r')) nroflinefeeds++;
      if (strchr(buffer,'\n')) nroflinefeeds++;
      
      if (Debug.isActive(Debug.VERBOSE)) Debug.printf(cF("RECEIVED FROM SERIAL:'%.*s'\n"), strlen(buffer) - nroflinefeeds, buffer);

      if(buffer[0] == '/')
      {
        //start found. Reset CRC calculation
        currentCRC=CRC16(0x0000,(unsigned char *) buffer, bufpos);
        _smartmeter_callback(cF("status"), sF("receiving"));
        buffer[0] = 0;
        bufpos = 0;
        return 1;
      }
      else if(buffer[0] == '!')
      {
        //add to crc calc 
        currentCRC=CRC16(currentCRC,(unsigned char*)buffer, 1);
        char messageCRC[5];
        strncpy(messageCRC, buffer + 1, 4);
        messageCRC[4]=0; //thanks to HarmOtten (issue 5)
        validCRCFound = (strtoul(messageCRC, NULL, 16) == currentCRC);
        bufpos = 0;
        buffer[bufpos] = 0;
        currentCRC = 0;
        if (validCRCFound)
        {
          if (Debug.isActive(Debug.VERBOSE)) Debug.printf(cF("SMARTMETER CRC RECEIVE OK\n"));

          smartmetervalues.electricity.kWh.total = (smartmetervalues.electricity.kWh.fromgrid.t1 != UINT16_MAX ? smartmetervalues.electricity.kWh.fromgrid.t1 : 0) +
                                                   (smartmetervalues.electricity.kWh.fromgrid.t2 != UINT16_MAX ? smartmetervalues.electricity.kWh.fromgrid.t2 : 0) -
                                                   (smartmetervalues.electricity.kWh.togrid.t1   != UINT16_MAX ? smartmetervalues.electricity.kWh.togrid.t1   : 0) -
                                                   (smartmetervalues.electricity.kWh.togrid.t2   != UINT16_MAX ? smartmetervalues.electricity.kWh.togrid.t2   : 0);

          
          smartmetervalues.electricity.kW.total = (smartmetervalues.electricity.kW.fromgrid.total != UINT16_MAX ? smartmetervalues.electricity.kW.fromgrid.total : 0) -
                                                     (smartmetervalues.electricity.kW.togrid.total != UINT16_MAX ? smartmetervalues.electricity.kW.togrid.total : 0);

          _smartmeter_callback(cF("electricity/kWh/total"), valuetodecimalstring(smartmetervalues.electricity.kWh.total, 3));

          _smartmeter_callback(cF("electricity/kWh/import/t1"), valuetodecimalstring(smartmetervalues.electricity.kWh.fromgrid.t1, 3));
          _smartmeter_callback(cF("electricity/kWh/import/t2"), valuetodecimalstring(smartmetervalues.electricity.kWh.fromgrid.t2, 3));
          _smartmeter_callback(cF("electricity/kWh/export/t1"), valuetodecimalstring(smartmetervalues.electricity.kWh.togrid.t1, 3));
          _smartmeter_callback(cF("electricity/kWh/export/t2"), valuetodecimalstring(smartmetervalues.electricity.kWh.togrid.t2, 3));

          _smartmeter_callback(cF("electricity/W"), valuetodecimalstring(smartmetervalues.electricity.kW.total, 0));

          _smartmeter_callback(cF("electricity/kW/import"), valuetodecimalstring(smartmetervalues.electricity.kW.fromgrid.total, 3));
          _smartmeter_callback(cF("electricity/kW/import/l1"), valuetodecimalstring(smartmetervalues.electricity.kW.fromgrid.l1, 3));
          _smartmeter_callback(cF("electricity/kW/import/l2"), valuetodecimalstring(smartmetervalues.electricity.kW.fromgrid.l2, 3));
          _smartmeter_callback(cF("electricity/kW/import/l3"), valuetodecimalstring(smartmetervalues.electricity.kW.fromgrid.l3, 3));

          _smartmeter_callback(cF("electricity/kW/export"), valuetodecimalstring(smartmetervalues.electricity.kW.togrid.total, 3));
          _smartmeter_callback(cF("electricity/kW/export/l1"), valuetodecimalstring(smartmetervalues.electricity.kW.togrid.l1, 3));
          _smartmeter_callback(cF("electricity/kW/export/l2"), valuetodecimalstring(smartmetervalues.electricity.kW.togrid.l2, 3));
          _smartmeter_callback(cF("electricity/kW/export/l3"), valuetodecimalstring(smartmetervalues.electricity.kW.togrid.l3, 3));

          _smartmeter_callback(cF("electricity/V/l1"), valuetodecimalstring(smartmetervalues.electricity.V.l1, 1));
          _smartmeter_callback(cF("electricity/V/l2"), valuetodecimalstring(smartmetervalues.electricity.V.l2, 1));
          _smartmeter_callback(cF("electricity/V/l3"), valuetodecimalstring(smartmetervalues.electricity.V.l3, 1));

          _smartmeter_callback(cF("electricity/A/l1"), valuetodecimalstring(smartmetervalues.electricity.A.l1, 2));
          _smartmeter_callback(cF("electricity/A/l2"), valuetodecimalstring(smartmetervalues.electricity.A.l2, 2));
          _smartmeter_callback(cF("electricity/A/l3"), valuetodecimalstring(smartmetervalues.electricity.A.l3, 2));

          _smartmeter_callback(cF("gas/m3"), valuetodecimalstring(smartmetervalues.gas.m3, 3));
          _smartmeter_callback(cF("gas/m3h"), valuetodecimalstring(smartmetervalues.gas.m3h, 3));
          _smartmeter_callback(cF("gas/datetime"), smartmetervalues.gas.datetime);

          _smartmeter_callback(cF("status"), sF("ready"));
          return 0;
        }
        else
        {
          if (Debug.isActive(Debug.ERROR)) Debug.printf(cF("SMARTMETER CRC RECEIVE ERROR\n"));
          _smartmeter_callback(cF("status"), sF("crcerror"));
          return 2;
        }
      }
      else
      {
        int day, month, year, hour, minute, second;
        char summerwinter;

        currentCRC=CRC16(currentCRC, (unsigned char*)buffer, bufpos);
  
        // 1-0:1.8.1 = Electricity low tarif used
        if (sscanf(buffer, cF("1-0:1.8.1(%f") , &value) == 1)
        {
          smartmetervalues.electricity.kWh.fromgrid.t1 = value * 1000;
        }
  
        // 1-0:1.8.2 = Electricity high tarif used (DSMR v4.0)
        else if (sscanf(buffer, cF("1-0:1.8.2(%f") , &value) == 1)
        {
          smartmetervalues.electricity.kWh.fromgrid.t2 = value * 1000;
        }
  
        // 1-0:2.8.1 = Electricity low tarif provided
        else if (sscanf(buffer, cF("1-0:2.8.1(%f"), &value) == 1)
        {
          smartmetervalues.electricity.kWh.togrid.t1 = value * 1000;
        }
  
        // 1-0:2.8.2 = Electricity high tarif provided (DSMR v4.0)
        else if (sscanf(buffer, cF("1-0:2.8.2(%f"), &value) == 1)
        {
          smartmetervalues.electricity.kWh.togrid.t2 = value * 1000;
        }
  
        // 1-0:1.7.0 = Electricity actual usage from grid (DSMR v4.0)
        else if (sscanf(buffer, cF("1-0:1.7.0(%f"), &value) == 1)
        {
          smartmetervalues.electricity.kW.fromgrid.total = value * 1000;
        }
  
        // 1-0:21.7.0 = Electricity actual usage from grid l1(DSMR v4.0)
        else if (sscanf(buffer, cF("1-0:21.7.0(%f"), &value) == 1)
        {
          smartmetervalues.electricity.kW.fromgrid.l1 = value * 1000;
        }
  
        // 1-0:41.7.0 = Electricity actual usage from grid l2(DSMR v4.0)
        else if (sscanf(buffer, cF("1-0:41.7.0(%f"), &value) == 1)
        {
          smartmetervalues.electricity.kW.fromgrid.l2 = value * 1000;
        }
  
        // 1-0:61.7.0 = Electricity actual usage from grid l3(DSMR v4.0)
        else if (sscanf(buffer, cF("1-0:61.7.0(%f"), &value) == 1)
        {
          smartmetervalues.electricity.kW.fromgrid.l3 = value * 1000;
        }
  
        // 1-0:2.7.0 = Electricity actual providing to grid (DSMR v4.0)
        else if (sscanf(buffer, cF("1-0:2.7.0(%f"), &value) == 1)
        {
          smartmetervalues.electricity.kW.togrid.total = value * 1000;
        }
  
        // 1-0:22.7.0 = Electricity actual providing to grid l1 (DSMR v4.0)
        else if (sscanf(buffer, cF("1-0:22.7.0(%f"), &value) == 1)
        {
          smartmetervalues.electricity.kW.togrid.l1 = value * 1000;
        }
  
        // 1-0:42.7.0 = Electricity actual providing to grid l1 (DSMR v4.0)
        else if (sscanf(buffer, cF("1-0:42.7.0(%f"), &value) == 1)
        {
           smartmetervalues.electricity.kW.togrid.l2 = value * 1000;
        }
  
        // 1-0:62.7.0 = Electricity actual providing to grid l1 (DSMR v4.0)
        else if (sscanf(buffer, cF("1-0:62.7.0(%f"), &value) == 1)
        {
           smartmetervalues.electricity.kW.togrid.l3 = value * 1000;
        }
  
        // 1-0:32.7.0 = Electricity voltage l1 (DSMR v5.0)
        else if (sscanf(buffer, cF("1-0:32.7.0(%f"), &value) == 1)
        {
           smartmetervalues.electricity.V.l1 = value * 10;
        }
  
        // 1-0:52.7.0 = Electricity voltage l2 (DSMR v5.0)
        else if (sscanf(buffer, cF("1-0:52.7.0(%f"), &value) == 1)
        {
           smartmetervalues.electricity.V.l2 = value * 10;
        }
  
        // 1-0:72.7.0 = Electricity voltage l3 (DSMR v5.0)
        else if (sscanf(buffer, cF("1-0:72.7.0(%f"), &value) == 1)
        {
           smartmetervalues.electricity.V.l3 = value * 10;
        }
  
  
        // 1-0:31.7.0 = Electricity current l1 (DSMR v5.0)
        else if (sscanf(buffer, cF("1-0:31.7.0(%f"), &value) == 1)
        {
           smartmetervalues.electricity.A.l1 = value * 100;
        }
  
        // 1-0:51.7.0 = Electricity current l2 (DSMR v5.0)
        else if (sscanf(buffer, cF("1-0:51.7.0(%f"), &value) == 1)
        {
           smartmetervalues.electricity.A.l2 = value * 100;
        }
  
        // 1-0:71.7.0 = Electricity current l3 (DSMR v5.0)
        else if (sscanf(buffer, cF("1-0:71.7.0(%f"), &value) == 1)
        {
           smartmetervalues.electricity.A.l3 = value * 100;
        }
  
        // 0-1:24.2.1 = Gas (DSMR v4.0)
        else if (sscanf(buffer, cF("0-1:24.2.1(%2d%2d%2d%2d%2d%2d%c)(%f*m3)"), &day, &month, &year, &hour, &minute, &second, &summerwinter, &value) == 8)
        {
          smartmetervalues.gas.m3 = value * 1000;
          sprintf(smartmetervalues.gas.datetime, cF("%02d-%02d-%02d %d:%02d:%02d"), day, month, year, hour, minute, second);
  
          static uint8_t oldhour = 255;
          if (oldhour != hour)
          {
              oldhour = hour;
              if (smartmetervalues.gas.oldm3 != UINT32_MAX) smartmetervalues.gas.m3h = (smartmetervalues.gas.m3 - smartmetervalues.gas.oldm3);
              smartmetervalues.gas.oldm3 = smartmetervalues.gas.m3;
          }
        }
  
        else
        {
          buffer[0] = 0;
          bufpos = 0;
          return 4;         
        }
  
        buffer[0] = 0;
        bufpos = 0;
        return 1; 
      }
    }
    yield();
  }
  return returnvalue;
}
