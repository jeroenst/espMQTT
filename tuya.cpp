#include "espMQTT.h"
#include "tuya.h"

/* https://images.tuyacn.com/smart/aircondition/Guide-to-Interworking-with-the-Tuya-MCU.pdf  
 * Tuyapacket with DP command = 0x55, 0xAA, VERSION, COMMAND, 0x00, LENGTH, DPID, DATA TYPE, FUNCTION LENGTH, FUCTION DATA
 *
 * COMMAND
 * 0x00 Heartbeat detection.
 * 0x01 Query product information.
 * 0x02 Query the working mode of the Wi-Fi module.
 * 0x03 Report the network connection status of the device.
 * 0x04 Reset the Wi-Fi module and switch the network configuration mode.
 * 0x05 Reset the Wi-Fi module and select a network configuration mode.
 * 0x06 Deliver DP commands.
 * 0x07 Report DP status.
 * 0x08 Query the device initialization status.
 * 0x0a (Optional) Start OTA upgrade.
 * 0x0b (Optional) Transmit the OTA upgrade package.
 * 0x1c (Optional) Obtain the local time.
 * 0x0e Test the Wi-Fi function (product testing command).
 */

static uint8_t tuya_dimvalue0 = 0;
static uint8_t tuya_dimvalue1 = 0;
static uint8_t tuya_dimstate0 = 0;
static uint8_t tuya_dimstate1 = 0;


void(*_tuya_callback)(const char *, String);

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

void tuya_init(void(*callback)(const char *, String))
{
  Serial.begin(9600, SERIAL_8N1);
  Serial.setRxBufferSize(100);
  _tuya_callback = callback;

  unsigned char tuyaCommand[] = { 0x55, 0xAA, 0x00, 0x08, 0x00, 0x00};
  tuya_commandCharsToSerial(6, tuyaCommand);    
}

void tuya_connected()
{
  unsigned char tuyaCommand[] = { 0x55, 0xAA, 0x00, 0x03, 0x00, 0x01, 0x04};
  tuya_commandCharsToSerial(7, tuyaCommand);    
}

void tuya_connectedMQTT()
{
  unsigned char tuyaCommand[] = { 0x55, 0xAA, 0x00, 0x03, 0x00, 0x01, 0x03};
  tuya_commandCharsToSerial(7, tuyaCommand);  
}

void tuya_disconnected()
{
  unsigned char tuyaCommand[] = { 0x55, 0xAA, 0x00, 0x03, 0x00, 0x01, 0x02};
  tuya_commandCharsToSerial(7, tuyaCommand);  
}

void tuya_apmode()
{
  unsigned char tuyaCommand[] = { 0x55, 0xAA, 0x00, 0x03, 0x00, 0x01, 0x01};
  tuya_commandCharsToSerial(7, tuyaCommand);  
}

void tuya_processSerialCommand(uint8_t commandLength, uint8_t receivedCommand[])
{
  if (commandLength > 6)
  {
    if (receivedCommand[2] == 0x00) // Version = 0
    {
      if (receivedCommand[3] == 0x07) // Command == Report DP status.
      {
        if (commandLength > 10)
        {
          if (receivedCommand[6] == 0x01) // DPID = 1
          {
            tuya_dimstate0 = receivedCommand[10];
          }
          if (receivedCommand[6] == 0x07) // DPID = 7
          {
            tuya_dimstate1 = receivedCommand[10];
          }
        }
        if (commandLength > 13)
        {
          if (receivedCommand[6] == 0x02) // DPID = 2
          {
            tuya_dimvalue0 = tuya_dimstate0 ? int(((receivedCommand[12] << 8) + receivedCommand[13]) / 10) : 0;
          }
          if (receivedCommand[6] == 0x08) // DPID = 8
          {
            tuya_dimvalue1 = tuya_dimstate1 ? int(((receivedCommand[12] << 8) + receivedCommand[13]) / 10) : 0;
          }
        }
        _tuya_callback("dimstate/0", String(tuya_dimstate0, DEC));
        _tuya_callback("dimstate/1", String(tuya_dimstate1, DEC));
      }
    }
  }
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
    DEBUG ("SERIALDATA=%d:0x%02X\n", receiveIndex, inChar);
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

  static unsigned long oldticktime = 0;
  static uint8_t olddimvalue0 = 0;
  static uint8_t olddimvalue1 = 0;
  if (millis() > oldticktime + 1000)
  {
        // Prevent constant update of dimvalue while changing dimvalue
        if (olddimvalue0 == tuya_dimvalue0)
        {
            _tuya_callback("dimvalue/0", String(tuya_dimvalue0, DEC));
        }
        else olddimvalue0 = tuya_dimvalue0;
    
        if (olddimvalue1 == tuya_dimvalue1)
        {
            _tuya_callback("dimvalue/1", String(tuya_dimvalue1, DEC));
        }
        else olddimvalue1 = tuya_dimvalue1;
        oldticktime = millis();
    }
}

void tuya_2gangdimmerv2_setdimstate(bool dimstate, uint8_t dimchannel)
{
  unsigned char tuyaCommand[] = { 0x55, 0xAA, 0x00, 0x06, 0x00, 0x05, (dimchannel == 0 ? 0x01 : 0x07), 0x01, 0x00, 0x01, dimstate ? 1 : 0};
  tuya_commandCharsToSerial(11, tuyaCommand);
}

void tuya_2gangdimmerv2_setdimvalue(uint8_t dimvalue, uint8_t dimchannel)
{
  uint16_t calculated_dimvalue = MIN((dimvalue * 10), 1000);
  unsigned char tuyaCommand[] = { 0x55, 0xAA, 0x00, 0x06, 0x00, 0x08, (dimchannel == 0 ? 0x02 : 0x08), 0x02, 0x00, 0x04, 0x00, 0x00, (calculated_dimvalue >> 8) & 0xFF, (calculated_dimvalue & 0xFF)};
  tuya_commandCharsToSerial(14, tuyaCommand);
  tuya_2gangdimmerv2_setdimstate(dimvalue ? 1 : 0, dimchannel);
}
