#include "espMQTT.h"
#include "tuya.h"

void(*_tuya_callback)(String, String);

void tuya_commandCharsToSerial(unsigned int length, unsigned char* command)
{
  uint8_t chkSum = 0;
  int i;
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

    Serial.print((char) chkSum);
    String hexvalue = String(chkSum, HEX);
    if (hexvalue.length() == 1) hexvalue = "0" + hexvalue;
    commandStr += "0x" + hexvalue + " ";
    DEBUG ("SEND SERIALDATA=%s\n", commandStr.c_str());
  }
  Serial.flush();
}

void tuya_init(void(*callback)(String, String))
{
  Serial.begin(9600, SERIAL_8N1);
  Serial.setRxBufferSize(2048);
  _tuya_callback = callback;
}

void tuya_processSerialCommand(uint8_t commandLength, uint8_t receivedCommand[])
{
  
}

void tuya_handle()
{
  static uint8_t tuya_initialize_counter = 0;

  const unsigned char TUYA_COMMAND_START[] = {0x55, 0xAA};
#define receivedCommandLength 140

  static int8_t receiveIndex = -1;
  static uint8_t receivedCommand[receivedCommandLength];
  static uint8_t commandLength = 0;

  if (Serial.available() > 0)
  {
    receiveIndex++;
    unsigned char inChar = Serial.read();
    DEBUG ("SERIALDATA=%d:%02X\n", receiveIndex, inChar);
    if (receiveIndex < receivedCommandLength) receivedCommand[receiveIndex] = inChar;
    if (receiveIndex < 2)
    {
      //Check command start
      if (TUYA_COMMAND_START[receiveIndex] != receivedCommand[receiveIndex]) {
        receiveIndex = -1;
      }
    }
    else if (receiveIndex == 5)
    {
      //length information now available
      commandLength = receivedCommand[4] * 0x100 + receivedCommand[5];
    }
    else if ((commandLength > -1) && (receiveIndex == (6 + commandLength)))
    {
      //verify checksum
      int expChecksum = 0;
      for (int i = 0; i < receiveIndex; i++) {
        expChecksum += receivedCommand[i];
      }
      expChecksum = expChecksum % 0x100;

      if (expChecksum == receivedCommand[receiveIndex])
      {
        // DEBUG ("tuya Checksum ok!\n");
        tuya_processSerialCommand(6 + commandLength, receivedCommand);
      }
      else DEBUG ("tuya Checksum FAIL!\n");
      receiveIndex = -1;
    }
  }
}

void tuya_2gangdimmerv2_setdimstate(bool dimstate, uint8_t dimchannel)
{
  unsigned char tuyaCommand[] = { 0x55, 0xAA, 0x00, 0x06, 0x00, 0x05, (dimchannel == 0 ? 0x01 : 0x07), 0x01, 0x00, 0x01, dimstate ? 1 : 0};
  tuya_commandCharsToSerial(11, tuyaCommand);
}

void tuya_2gangdimmerv2_setdimvalue(uint8_t dimvalue, uint8_t dimchannel)
{
  uint16_t calculated_dimvalue = (dimvalue * 10)-9;
  if (calculated_dimvalue > 990) calculated_dimvalue = 1000;
  unsigned char tuyaCommand[] = { 0x55, 0xAA, 0x00, 0x06, 0x00, 0x08, (dimchannel == 0 ? 0x02 : 0x08), 0x02, 0x00, 0x04, 0x00, 0x00, (calculated_dimvalue >> 8) & 0xFF, (calculated_dimvalue & 0xFF)};
  tuya_commandCharsToSerial(14, tuyaCommand);
  tuya_2gangdimmerv2_setdimstate(dimvalue?1:0, dimchannel);
}
