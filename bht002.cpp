// Created with information from: https://github.com/klausahrenberg/WThermostatBeca/
// https://github.com/fashberg/WThermostatBeca/blob/master/WThermostat/WBecaDevice.h

#define receivedCommandLength 140


#include "espMQTT.h"
#include "bht002.h"
int8_t bht002_temperature = 128;
uint8_t bht002_setpoint = 18;
bool bht002_heating = false;
bool bht002_deviceon = false;
bool bht002_deviceoninitialized = false;
bool bht002_preventdeviceoff = true;
bool bht002_wificonnected = false;
bool bht002_initready = false;

void(*_bht002_callback)(const char *, String);

void bht002_commandCharsToSerial(unsigned int length, unsigned char* command)
{
  int chkSum = 0;
  unsigned int i;
  String commandStr = "";

  if (length > 2) {
    for (i = 0; i < length; i++) {
      unsigned char chValue = command[i];
      chkSum += chValue;
      Serial.print((char) chValue);
      String hexvalue = String(chValue, HEX);
      if (hexvalue.length() == 1) hexvalue = "0" + hexvalue;
      commandStr += "0x" + hexvalue + " ";
    }
    unsigned char chValue = chkSum % 0x100;
    Serial.print((char) chValue);
    String hexvalue = String(chValue, HEX);
    if (hexvalue.length() == 1) hexvalue = "0" + hexvalue;
    commandStr += "0x" + hexvalue + " ";
    DEBUG ("SEND SERIALDATA=%s\n", commandStr.c_str());
  }
}

void bht002_senddevicestate(bool state)
{
  DEBUG ("Sending On\n");
  unsigned char bht002Command[] = { 0x55, 0xAA, 0x00, 0x06, 0x00, 0x05, 0x01, 0x01, 0x00, 0x01, state ? (unsigned char)0x01 : (unsigned char)0x00 };
  bht002_commandCharsToSerial(11, bht002Command);
}

void bht002_sendtime()
{
  time_t now;
  char strftime_buf[64];
  struct tm timeinfo;

  time(&now);

  localtime_r(&now, &timeinfo);
  strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);

  DEBUG ("Sending Datetime: %s\n", strftime_buf);

  unsigned char cancelConfigCommand[] = { 0x55, 0xaa, 0x00, 0x1c, 0x00, 0x08,
                                          0x01, (unsigned char)timeinfo.tm_year, (unsigned char)timeinfo.tm_mon, (unsigned char)timeinfo.tm_mday,
                                          (unsigned char)timeinfo.tm_hour, (unsigned char)timeinfo.tm_min, (unsigned char)timeinfo.tm_sec, (unsigned char)timeinfo.tm_wday
                                        };
  bht002_commandCharsToSerial(14, cancelConfigCommand);
}

void bht002_init(void(*callback)(const char *, String))
{
  Serial.begin(9600, SERIAL_8N1);
  Serial.setRxBufferSize(200);
  _bht002_callback = callback;
  // Start init sequence
  //  DEBUG ("Sending Query Product Information\n");
  //  unsigned char queryProductCommand[] = { 0x55, 0xAA, 0x00, 0x01, 0x00, 0x00 };
  //  bht002_commandCharsToSerial(6, queryProductCommand);
}

void bht002_senddebug(uint8_t cmdnr)
{
  switch (cmdnr)
  {
    case 0:
    case 9:
      {
        DEBUG ("Sending Heartbeat?\n");
        unsigned char bht002Command[] = { 0x55, 0xAA, 0x00, 0x00, 0x00, 0x00 };
        bht002_commandCharsToSerial(6, bht002Command);
      }
      break;
    case 1:
      {
        DEBUG ("Sending Request Query Product Information\n");
        unsigned char queryProductCommand[] = { 0x55, 0xAA, 0x00, 0x01, 0x00, 0x00 };
        bht002_commandCharsToSerial(6, queryProductCommand);
      }
      break;
    case 2:
      {
        DEBUG ("Sending Request Working Mode\n");
        unsigned char configCommand[] = { 0x55, 0xAA, 0x00, 0x02, 0x00, 0x00 };
        bht002_commandCharsToSerial(6, configCommand);
      }
      break;
    case 3:
      {
        DEBUG ("Sending Query State\n");
        unsigned char queryStateCommand[] = { 0x55, 0xAA, 0x00, 0x08, 0x00, 0x00 };
        bht002_commandCharsToSerial(6, queryStateCommand);
        //wifipairing = false;
      }
      break;
    case 4:
      {
        DEBUG ("Sending Wifi Off\n");
        unsigned char queryStateCommand[] = { 0x55, 0xAA, 0x00, 0x03, 0x00, 0x01, 0x02 };
        bht002_commandCharsToSerial(7, queryStateCommand);
        //wifipairing = false;
      }
      break;
    case 5:
      {
        DEBUG ("Sending Wifi Connected\n");
        unsigned char queryStateCommand[] = { 0x55, 0xAA, 0x00, 0x03, 0x00, 0x01, 0x03 };
        bht002_commandCharsToSerial(7, queryStateCommand);
        //wifipairing = false;
      }
      break;
    case 6:
      {
        DEBUG ("Sending Reset datetime?\n");
        unsigned char queryStateCommand[] = { 0x55, 0xAA, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        bht002_commandCharsToSerial(12, queryStateCommand);
      }
      break;
    case 7:
      {
        DEBUG ("Sending Datetime?\n");
        unsigned char bht002Command[] = { 0x55, 0xAA, 0x00, 0x1c, 0x00, 0x08, 0x01, 0x14, 0x0a, 0x0d, 0x08, 0x0f, 0x03 };
        bht002_commandCharsToSerial(13, bht002Command);
      }
      break;
    case 8:
      {
        DEBUG ("Sending Configuration Request?\n");
        unsigned char bht002Command[] = { 0x55, 0xAA, 0x00, 0x03, 0x00, 0x01, 0x04 };
        bht002_commandCharsToSerial(7, bht002Command);
      }
      break;
    case 10:
      {
        DEBUG ("Sending On\n");
        unsigned char bht002Command[] = { 0x55, 0xAA, 0x00, 0x06, 0x00, 0x05, 0x01, 0x01, 0x00, 0x01, 0x01 };
        bht002_commandCharsToSerial(11, bht002Command);
      }
      break;
    case 11:
      {
        DEBUG ("Sending Off\n");
        unsigned char bht002Command[] = { 0x55, 0xAA, 0x00, 0x06, 0x00, 0x05, 0x01, 0x01, 0x00, 0x01, 0x00 };
        bht002_commandCharsToSerial(11, bht002Command);
      }
      break;
    case 12:
      {
        DEBUG ("Sending setpoint=11\n");
        unsigned char bht002Command[] = { 0x55, 0xAA, 0x00, 0x06, 0x00, 0x08, 0x02, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0b };
        bht002_commandCharsToSerial(14, bht002Command);
      }
      break;
    case 13:
      {
        DEBUG ("Sending setpoint=12\n");
        unsigned char bht002Command[] = { 0x55, 0xAA, 0x00, 0x06, 0x00, 0x08, 0x02, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x0c };
        bht002_commandCharsToSerial(14, bht002Command);
      }
      break;
    case 14:
      {
        DEBUG ("Sending setpoint=25\n");
        unsigned char bht002Command[] = { 0x55, 0xAA, 0x00, 0x06, 0x00, 0x08, 0x02, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 25 };
        bht002_commandCharsToSerial(14, bht002Command);
      }
      break;
    default:
      {
      }
      break;
  }
}

void bht002_sendon()
{
  DEBUG ("Sending On\n");
  unsigned char bht002Command[] = { 0x55, 0xAA, 0x00, 0x06, 0x00, 0x05, 0x01, 0x01, 0x00, 0x01, 0x01 };
  bht002_commandCharsToSerial(11, bht002Command);
}

void bht002_sendoff()
{
  DEBUG ("Sending Off\n");
  unsigned char bht002Command[] = { 0x55, 0xAA, 0x00, 0x06, 0x00, 0x05, 0x01, 0x01, 0x00, 0x01, 0x00 };
  bht002_commandCharsToSerial(11, bht002Command);
}


void bht002_sendsetsetpoint()
{
  DEBUG ("Sending setpoint=%d\n", bht002_setpoint);
  unsigned char bht002Command[] = { 0x55, 0xAA, 0x00, 0x06, 0x00, 0x08, 0x02, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, bht002_setpoint };
  bht002_commandCharsToSerial(14, bht002Command);
}


void bht002_setsetpoint(uint8_t temperature)
{
  bht002_setpoint = temperature;
  if (bht002_initready)
  {
    bht002_sendsetsetpoint();
  }
}

int8_t bht002_getsetpoint()
{
  return bht002_setpoint;
}

int8_t bht002_gettemperature()
{
  return bht002_temperature;
}

int8_t bht002_getheating()
{
  return bht002_heating;
}


void bht002_processSerialCommand(uint8_t commandLength, uint8_t receivedCommand[])
{
  if (commandLength > 0)
  {
    String commandStr = "";
    for (uint8_t i = 0; i < commandLength; i++)
    {
      String hexvalue = String(receivedCommand[i], HEX);
      if (hexvalue.length() == 1) hexvalue = "0" + hexvalue;
      commandStr += "0x" + hexvalue + " ";
    }
    DEBUG ("RECEIVED SERIALDATA=%s\n", commandStr.c_str());
  }
  if (commandLength > 3)
  {
    if (receivedCommand[3] == 0x00)
    {
      switch (receivedCommand[6])
      {
        case 0x00:
        case 0x01:
          //ignore, heartbeat MCU
          //55 aa 01 00 00 01 01
          //55 aa 01 00 00 01 00
          break;
          //default:
          //notifyUnknownCommand();
      }
    }
    else if (receivedCommand[3] == 0x02)
    {
      // Send 2nd command in init sequence
      //    DEBUG ("Sending Working Mode\n");
      //  unsigned char configCommand[] = { 0x55, 0xAA, 0x00, 0x02, 0x00, 0x00 };
      //bht002_commandCharsToSerial(6, configCommand);

    }
    else if (receivedCommand[3] == 0x03)
    {
      //ignore, MCU response to wifi state
      //55 aa 01 03 00 00
    }
    else if (receivedCommand[3] == 0x04)
    {
      //Setup initialization request
      //received: 55 aa 01 04 00 00
      //if (onConfigurationRequest) {
      //send answer: 55 aa 00 03 00 01 00
      //unsigned char configCommand[] = { 0x55, 0xAA, 0x00, 0x03, 0x00, 0x01, 0x00 };
      //bht002_commandCharsToSerial(7, configCommand);
      //onConfigurationRequest();
      //      wifipairing = true;
    }
    else if (receivedCommand[3] == 0x07)
    {
      //Status report from MCU
      if (receivedCommand[6] == 0x01)
      {
        DEBUG ("Device On=%d\n", receivedCommand[10]);
        _bht002_callback("deviceon", String(receivedCommand[10], DEC));
        bht002_deviceon = receivedCommand[10] ? 1 : 0;
        if (!bht002_deviceon)
        {
          if (!bht002_deviceoninitialized || bht002_preventdeviceoff)
          {
            bht002_senddevicestate(true);
            bht002_deviceoninitialized = true;
          }
          _bht002_callback("heating", "0");
          bht002_heating = false;
        }
      }

      if (receivedCommand[6] == 0x02)
      {
        DEBUG ("Room Setpoint=%d\n", receivedCommand[13]);
        _bht002_callback("setpoint", String(receivedCommand[13], DEC));
        bht002_temperature = receivedCommand[13];
      }

      if (receivedCommand[6] == 0x03)
      {
        DEBUG ("Room Temperature=%d\n", receivedCommand[13]);
        _bht002_callback("temperature", String(receivedCommand[13], DEC));
        bht002_setpoint = receivedCommand[13];
      }

      if (receivedCommand[6] == 0x0E)
      {
        if (receivedCommand[10] == 0x00)
        {
          DEBUG ("Heating=1\n");
          if (bht002_deviceon)
          {
            bht002_heating = true;
            _bht002_callback("heating", "1");
          }
          else bht002_heating = false;
        }

        if (receivedCommand[10] == 0x01)
        {
          DEBUG("Heating=0\n");
          _bht002_callback("heating", "0");
          bht002_heating = false;
        }
      }

    }
    else if (receivedCommand[3] == 0x1C)
    {
      bht002_sendtime();
    }
    else
    {
      DEBUG ("Received Unknown Command %02X\n", receivedCommand[3]);
      // Unknown Command
    }
    //bht002_receivingDataFromMcu = false;
  }
}

void bht002_sendwifistate()
{
  if (bht002_wificonnected)
  {
    DEBUG ("Sending Wifi Connected\n");
    unsigned char queryStateCommand[] = { 0x55, 0xAA, 0x00, 0x03, 0x00, 0x01, 0x03 };
    bht002_commandCharsToSerial(7, queryStateCommand);
    bht002_wificonnected = true;
  }
  else
  {
    DEBUG ("Sending Wifi Disconnected\n");
    unsigned char queryStateCommand[] = { 0x55, 0xAA, 0x00, 0x03, 0x00, 0x01, 0x02 };
    bht002_commandCharsToSerial(7, queryStateCommand);
    bht002_wificonnected = false;
  }
}

void bht002_connected()
{
    bht002_wificonnected = true;
  if (bht002_initready)
  {
    bht002_sendwifistate();
  }
}

void bht002_disconnected()
{
  bht002_wificonnected = false;
  if (bht002_initready)
  {
    bht002_sendwifistate();
  }
}

void bht002_handle()
{
  static uint8_t bht002_initialize_counter = 0;

  const unsigned char BHT002_COMMAND_START[] = {0x55, 0xAA};

  static int16_t receiveIndex = -1;
  static uint8_t receivedCommand[receivedCommandLength];
  static uint8_t commandLength = 0;


  while (Serial.available() > 0)
  {
    receiveIndex++;
    unsigned char inChar = Serial.read();
    //DEBUG ("SERIALDATA=%d:%02X\n", receiveIndex, inChar);
    if (receiveIndex < receivedCommandLength) receivedCommand[receiveIndex] = inChar;
    if (receiveIndex < 2)
    {
      //Check command start
      if (BHT002_COMMAND_START[receiveIndex] != receivedCommand[receiveIndex]) {
        receiveIndex = -1;
      }
    }
    else if (receiveIndex == 5)
    {
      //length information now available
      commandLength = receivedCommand[4] * 0x100 + receivedCommand[5];
    }
    else if ((commandLength > 0) && (receiveIndex == (6 + commandLength)))
    {
      //verify checksum
      int expChecksum = 0;
      for (int i = 0; i < receiveIndex; i++) {
        expChecksum += receivedCommand[i];
      }
      expChecksum = expChecksum % 0x100;

      if (expChecksum == receivedCommand[receiveIndex])
      {
        // DEBUG ("bht002 Checksum ok!\n");
        bht002_processSerialCommand(6 + commandLength, receivedCommand);
      }
      else DEBUG ("bht002 Checksum FAIL!\n");
      receiveIndex = -1;
    }
  }

  if ((bht002_initialize_counter == 0) && (millis() > 4000))
  {
    bht002_initialize_counter = 1;
    DEBUG ("Sending Heartbeat?\n");
    unsigned char bht002Command[] = { 0x55, 0xAA, 0x00, 0x00, 0x00, 0x00 };
    bht002_commandCharsToSerial(6, bht002Command);
  }


  if ((bht002_initialize_counter == 1) && (millis() > 5000))
  {
    bht002_initialize_counter = 2;
    bht002_sendon();
  }

  if ((bht002_initialize_counter == 2) && (millis() > 6000))
  {
    bht002_initialize_counter = 3;
    bht002_sendwifistate();
  }


  if ((bht002_initialize_counter == 3) && (millis() > 7000))
  {
    bht002_initialize_counter = 4;
    bht002_sendsetsetpoint();
    bht002_initready = true;
  }
}
