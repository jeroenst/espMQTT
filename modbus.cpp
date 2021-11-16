/*
   Function Code Register Type
   1 Read Coil
   2 Read Discrete Input
   3 Read Holding Registers
   4 Read Input Registers
   5 Write Single Coil
   6 Write Single Holding Register
   15  Write Multiple Coils
   16  Write Multiple Holding Registers
*/

#include "espMQTT.h"

uint8_t modbusDeviceAddress = 0;
uint8_t *modbus_RxBuffer = NULL;
uint8_t modbus_RxBufferPointer = 0;
bool modbux_RxReady = false;

void modbus_request_function_code(uint8_t deviceAddress, uint8_t functionCode, uint16_t startAddress, uint16_t numberOfAddresses )
{
  DEBUG_V("Requesting Modbus Data From %02x...\n", deviceAddress);
  modbux_RxReady = false;
  uint8_t TxBuffer[10];

  modbusDeviceAddress = deviceAddress;
  TxBuffer[0] = deviceAddress; // adres
  TxBuffer[1] = functionCode; // type
  TxBuffer[2] = startAddress >> 8; // H startaddress
  TxBuffer[3] = startAddress && 0xFF; // L startaddress
  TxBuffer[4] = numberOfAddresses >> 8; // H number of addresses
  TxBuffer[5] = numberOfAddresses && 0xFF; // L number of addresses

  //Calc the raw_msg_data_byte CRC code
  uint16_t crc = 0xFFFF;
  String crc_string = "";
  for (int pos = 0; pos < 6; pos++) {
    crc ^= (uint16_t)TxBuffer[pos];   // XOR byte into least sig. byte of crc
    for (int i = 8; i != 0; i--) {    // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }

  TxBuffer[6] = crc & 0xFF;
  TxBuffer[7] = crc >> 8;

  for (int i = 0; i < 8; i++)
  {
    DEBUG_V ("Sending to modbus Device: 0x%02x\n", TxBuffer[i]);
    Serial.write(TxBuffer[i]);
  }

  modbus_RxBufferPointer = 0;
}

uint8_t modbus_handle()
{
  if (Serial.available())
  {
    if (modbus_RxBufferPointer < 250) {
      if (modbus_RxBufferPointer == 0)
      {
        free(modbus_RxBuffer);
        modbus_RxBuffer = (uint8_t*) malloc(sizeof(uint8_t));
      }
      else
      {
        modbus_RxBuffer = (uint8_t *) realloc(modbus_RxBuffer, (modbus_RxBufferPointer + 1) * sizeof(uint8_t));
      }
      modbus_RxBuffer[modbus_RxBufferPointer] = Serial.read();

      DEBUG_V ("Received from modbus device [addr %i] (pointer: %i) : 0x%02x\n", modbus_RxBuffer[0], modbus_RxBufferPointer, modbus_RxBuffer[modbus_RxBufferPointer]);

      if (modbus_RxBuffer[0] != modbusDeviceAddress) {
        modbus_RxBufferPointer = 0;
        DEBUG_V ("modbus device address != received address");
      }
      else
      {
        modbus_RxBufferPointer++;
      }
      
    } else {
      DEBUG_E("Serial Buffer Overflow!!\n");
      DEBUG_V("Serial Buffer Overflow!!\n");
      modbus_RxBufferPointer = 0;
    }

    if (modbus_RxBufferPointer > 2)
    {
      if ((modbus_RxBuffer[0] == modbusDeviceAddress) && (modbus_RxBufferPointer == modbus_RxBuffer[2] + 5))
      {
        modbux_RxReady = true;
      }
    }
  }
}

bool modbus_rx_ready()
{
  return modbux_RxReady;
}

uint8_t modbus_get_byte(uint8_t bytenr)
{

  if (bytenr < modbus_RxBufferPointer)
  {
    return modbus_RxBuffer[bytenr];
  }
  else
  {
    return 0;
  }
}

void modbus_clear_buffer()
{
  free (modbus_RxBuffer);
  modbus_RxBuffer = NULL;
  modbus_RxBufferPointer = 0;
  modbux_RxReady = 0;
}

double glue(unsigned int d1, unsigned int d0) {
  double t;
  t = d1 << 16;
  t += d0;
  return t;
}

double modbus_get_double(uint8_t startbytenr, uint16_t devide)
{
  if (startbytenr < modbus_RxBufferPointer + 1)
  {
    return glue(modbus_RxBuffer[startbytenr], modbus_RxBuffer[startbytenr + 1]) / devide;
  }
  else
  {
    return NAN;
  }
}
