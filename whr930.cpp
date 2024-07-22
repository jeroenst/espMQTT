#include "espMQTT.h"
#include "whr930.h"
#include <iostream>
#include <algorithm>

ZEHNDER_WHR930 zehnder_whr930;

using namespace std;

void ZEHNDER_WHR930::requestData(bool startSequence)
{  
  if (startSequence) 
  {
    sendCounter = 1;
    sendCleared = true;
  }

  while (sendCleared && (sendCounter > 0))
  {
    sendCleared = false;
    switch (sendCounter)
    {
      case 1:
        sendPacket(0x00CD);
      break;
      case 2:
        sendPacket(0x000B);
      break;
      case 3:
        sendPacket(0x00D1);
      break;
      case 4:
        sendPacket(0x00D9);
      break;
      case 5: 
        sendPacket(0x00DF);
      break;
      case 6: 
        sendPacket(0x00E1);
      break;
      case 7:
        if (fanlevel.send)
        {
          uint8_t sendData[1] = {fanlevel.level};
          if (fanlevel.level < 5) sendPacket(0x0099, sendData, 1);
          fanlevel.send = false;
        }
        else sendCleared = true;
      break;        
      case 8:
        if (comfort.send)
        {
          uint8_t sendData[1] = {comfort.calculated_temperature};
          sendPacket(0x00D3, sendData, 1);
          comfort.send = false;
        }
        else sendCleared = true;
      break;        
      default: 
        sendCounter = 0;
      break;
    }
    if (sendCounter) sendCounter++;
  }
}

void ZEHNDER_WHR930::sendPacket(uint16_t command, uint8_t *data, uint8_t length)
{
    /*
    Create a packet.
    Data length and checksum are automatically calculated and added to the packet.
    Start and end bits are added as well.

    A packet is build up as follow:

        Start                : 2 bytes (0x07 0xF0)
        Command              : 2 bytes
        Number of data bytes : 1 byte
        Data bytes           : 0-n bytes
        Checksum             : 1 byte
        End                  : 2 bytes (0x07 0x0F)
    */

  uint8_t *packet = (uint8_t *)malloc(length+8);
  uint8_t packetpointer = 0;
  uint8_t crc = 173;
  bool found07 = false;

  packet[packetpointer++] = 0x07;  // default start bit
  packet[packetpointer++] = 0xF0;  // default start bit
  crc += command >> 8;
  packet[packetpointer++] = command >> 8;
  crc += command & 0xFF;
  packet[packetpointer++] = command & 0xFF;
  packet[packetpointer++] = length;
  crc += length & 0xFF;

  for (uint8_t index = 0; index < length; index++)
  {
    packet[packetpointer++] = data[index];
    if (((data[index] == 0x07) && !found07) || (data[index] != 0x07))
    {
      crc += data[index];
    }

    if (data[index] == 0x07) found07 = true;
  }
  packet[packetpointer++] = crc;
  packet[packetpointer++] = 0x07;
  packet[packetpointer++] = 0x0F;

  String debugstring = "";
  for (uint8_t i = 0; i < packetpointer; i++) debugstring += " 0x" + String(packet[i], HEX);
  if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("Writing to serial:%s\n"), debugstring.c_str());

  Serial.write(packet, packetpointer);
  free(packet);
}

void ZEHNDER_WHR930::setfanlevel(uint8_t level)
{
      if (level <= 4)
      {
        if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("setfanlevel(%d)\n"), level);
        fanlevel.level = level;
        fanlevel.send = true;
      }
}

void ZEHNDER_WHR930::setcomforttemperature(float temperature)
{
      if (temperature >= 15 && temperature <= 27)
      {
        if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("setcomforttemperature(%f)\n"), temperature);
        comfort.calculated_temperature = (float)((temperature + 20) * 2);
        comfort.send = true;
      }
}

void ZEHNDER_WHR930::setup()
{
  Serial.begin(9600);
  Serial.swap();
}


void ZEHNDER_WHR930::loop()
{
  static uint8_t data[40];
  static uint8_t datalength = 0;


  if (Serial.available())
  {
    if (datalength == 40) datalength = 0;

    data[datalength++] = Serial.read();
    
    String debugstring = "";
    for (uint8_t i = 0; i < datalength; i++) debugstring += " 0x" + String(data[i], HEX);
    if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("Received from serial:%s\n"), debugstring.c_str());

    if ((datalength == 2) && (data[0] == 0x07) && (data[1] == 0xF3))
    {
      // ACK received
      datalength = 0;
    }

    if (data[0] != 0x07)
    {
      // Remove garbage
      datalength = 0;
    }

    if ((datalength == 2) && (data[0] != 0x07) && (data[1] != 0xF0))
    {
      // Remove garbage
      datalength = 0;
    }

    if ((datalength == 4) && (data[0] == 0x07) && (data[1] == 0x0F) && (data[2] == 0x07) && (data[3] == 0xF3) )
    {
      // ACK
      datalength = 0;
    }
    if (datalength > 10)
    {
       uint8_t messagelength = data[4] + 8;
      // Check if complete packet is received
      if ((data[0] == 0x7) && (data[1] == 0xF0) && (data[datalength-2] == 0x7) && (data[datalength-1] == 0xF))
      {
        sendCleared = true;
        if (datalength == messagelength)
        {
          uint16_t command = (data[2] << 8) + data[3];
          if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("Command received:0x%x\n"), command);
          switch (command)
          {
            case 0x00CE:
                if (datalength >= 16+3)
              {
                putdatamap(cF("fanlevel"), data[13]);
                putdatamap(cF("exhaust/fan/current"), data[11]);
                putdatamap(cF("exhaust/fan/absent"), data[5]);
                putdatamap(cF("exhaust/fan/low"), data[6]);
                putdatamap(cF("exhaust/fan/medium"), data[7]);
                putdatamap(cF("exhaust/fan/high"), data[15]);
                putdatamap(cF("intake/fan/active"), data[14]);
                putdatamap(cF("intake/fan/current"), data[12]);
                putdatamap(cF("intake/fan/absent"), data[8]);
                putdatamap(cF("intake/fan/low"), data[9]);
                putdatamap(cF("intake/fan/medium"), data[10]);
                putdatamap(cF("intake/fan/high"), data[16]);
              }
            break;
            case 0x000C:
              if (datalength >= 10+3)
              {
                putdatamap(cF("intake/fan/rpm"),  1875000 / ((data[7] << 8) + data[8]));
                putdatamap(cF("exhaust/fan/rpm"), 1875000 / ((data[9] << 8) + data[10]));
              }
            break;
            case 0x00D2:
              if (datalength >= 9+3)
              {
                char floatvalue[5];
                snprintf(floatvalue, 5, "%.1f", float(data[5])/ 2.0 - 20);
                putdatamap(cF("comfort/temperature"), floatvalue);
                snprintf(floatvalue, 5, "%.1f", float(data[6])/ 2.0 - 20);
                putdatamap(cF("intake/temperature"), floatvalue);
                snprintf(floatvalue, 5, "%.1f", float(data[7])/ 2.0 - 20);
                putdatamap(cF("supply/temperature"), floatvalue);
                snprintf(floatvalue, 5, "%.1f", float(data[8])/ 2.0 - 20);
                putdatamap(cF("extract/temperature"), floatvalue);
                snprintf(floatvalue, 5, "%.1f", float(data[9])/ 2.0 - 20);
                putdatamap(cF("exhaust/temperature"), floatvalue);
              }
            break;
            case 0x00DA:
              if (datalength >= 13+3)
              {
                putdatamap(cF("filterfull"), data[13]);
              }
            break;
            case 0x00E0:
              if (datalength >= 11+3)
              {
                putdatamap(cF("bypass/factor"), data[7]);
                putdatamap(cF("bypass/level"), data[8]);
                putdatamap(cF("bypass/correction"), data[9]);
                putdatamap(cF("bypass/summermode"), data[11]);
              }
            break;
            case 0x00E2:
              if (datalength >= 10+3)
              {
                putdatamap(cF("bypass/status"), data[5]);
                putdatamap(cF("frostprotection/heater"), data[7]);
                putdatamap(cF("frostprotection/status"), data[6]);
                putdatamap(cF("frostprotection/minutes"), (data[8] << 8) + data[9]);
                putdatamap(cF("frostprotection/level"), data[10]);
              }
            break;
          }
          datalength = 0;
        }
      }
    }
  }
  else
  {
  }
  zehnder_whr930.requestData(false);
}

void ZEHNDER_WHR930::secondTick(uint16_t uptime)
{
  if ((uptime % 10 == 0) && (uptime > 0))
  {
    if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("Requesting data from WHR-930\n"));
    zehnder_whr930.requestData(true);
  }
}
