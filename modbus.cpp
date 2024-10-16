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
bool modbux_RxError = false;

uint16_t modbus_write_and_calculate_crc(uint16_t crc, uint8_t data)
{
  Serial.write(data);

  //Calc the raw_msg_data_byte CRC code
  crc ^= data;   // XOR byte into least sig. byte of crc
  for (int i = 8; i != 0; i--) {    // Loop over each bit
    if ((crc & 0x0001) != 0) {      // If the LSB is set
      crc >>= 1;                    // Shift right and XOR 0xA001
      crc ^= 0xA001;
    }
    else                            // Else LSB is not set
      crc >>= 1;                    // Just shift right
  }
  return crc;
}

uint8_t modbus_send_function_code(uint8_t deviceAddress, uint8_t functionCode, uint16_t startAddress, uint16_t registerCount, uint16_t *data)
/*
 * Send funtion code to modbus device
 * 
 * Available function codes: 4,5,6,16
 * 
 * Return values:
 * 0 = Data send OK
 * 1 = Wrong function code
 * 2 = Wrong number of registers
 * 3 = Data aray is NULL
 */
{
  uint16_t crc = 0xFFFF;

  modbux_RxReady = false;
  modbux_RxError = false;
  modbus_RxBufferPointer = 0;
  modbusDeviceAddress = deviceAddress;

  switch (functionCode)
  {
    case 3:
    case 4:
      if ((registerCount == 0) || (registerCount > 30)) return 2;
    break;
    case 6:
      if (registerCount != 1) return 2;
      if (data == NULL) return 3;
    break;
    case 16:
      if ((registerCount == 0) || (registerCount > 30)) return 2;
      if (data == NULL) return 3;
    break;
    default:
      return 1;
    break;
  }

  DEBUG_V ("Sending data to modbus Device deviceAddress:%d, functionCode:%d, startAddress%d, registerCount:%d\n", deviceAddress, functionCode, startAddress, registerCount);
  
  crc = modbus_write_and_calculate_crc(crc, deviceAddress);
  crc = modbus_write_and_calculate_crc(crc, functionCode);
  crc = modbus_write_and_calculate_crc(crc, startAddress >> 8);
  crc = modbus_write_and_calculate_crc(crc, startAddress & 0xFF);
  switch (functionCode)
  {
    case 3:
    case 4:
      crc = modbus_write_and_calculate_crc(crc, registerCount >> 8);
      crc = modbus_write_and_calculate_crc(crc, registerCount & 0xFF);
    break;
    case 6:
      crc = modbus_write_and_calculate_crc(crc, data[0] >> 8);
      crc = modbus_write_and_calculate_crc(crc, data[0] & 0xFF);
    break;
    case 16:
      crc = modbus_write_and_calculate_crc(crc, registerCount >> 8);
      crc = modbus_write_and_calculate_crc(crc, registerCount & 0xFF);
      crc = modbus_write_and_calculate_crc(crc, registerCount * 2);
      for (uint8_t dataPointer = 0; dataPointer < registerCount; dataPointer++)
      {
        crc = modbus_write_and_calculate_crc(crc, data[dataPointer] >> 8);
        crc = modbus_write_and_calculate_crc(crc, data[dataPointer] & 0xFF);    
      }
    break;
  }
  
  Serial.write(crc & 0xFF);
  Serial.write(crc >> 8);
  Serial.flush();

  return 0;
}

uint8_t modbus_write_holding_registers(uint8_t deviceAddress, uint16_t startRegister, uint16_t numberOfRegisters, uint16_t *values)
{
  return modbus_send_function_code(deviceAddress, 16, startRegister, numberOfRegisters, values);
}

uint8_t modbus_write_holding_register(uint8_t deviceAddress, uint16_t startRegister, uint16_t value)
{
  return modbus_send_function_code(deviceAddress, 6, startRegister, 1, &value);
}

uint8_t modbus_request_holding_registers(uint8_t deviceAddress, uint16_t startRegister, uint16_t numberOfRegisters)
{
  return modbus_send_function_code(deviceAddress, 3, startRegister, numberOfRegisters, NULL);
}

uint8_t modbus_request_input_registers(uint8_t deviceAddress, uint16_t startRegister, uint16_t numberOfRegisters)
{
  return modbus_send_function_code(deviceAddress, 4, startRegister, numberOfRegisters, NULL);  
}

void modbus_handle()
{
  yield();
  while (Serial.available())
  {
    if (modbux_RxError)
    {
      Serial.flush();
      if (modbus_RxBuffer) free(modbus_RxBuffer);
      return;
    }
    if (modbus_RxBufferPointer < 250) {
      if (modbus_RxBufferPointer == 0)
      {
        if (modbus_RxBuffer) free(modbus_RxBuffer);
        modbus_RxBuffer = (uint8_t*) malloc(sizeof(uint8_t));
      }
      else
      {
        modbus_RxBuffer = (uint8_t *) realloc(modbus_RxBuffer, (modbus_RxBufferPointer + 1) * sizeof(uint8_t));
      }

      int serialData = Serial.read();
      if (serialData >= 0)
      {

        modbus_RxBuffer[modbus_RxBufferPointer] = serialData;

        DEBUG_V ("Received from modbus Device [addr %i] (pointer: %i) : %d (0x%02x)\n", modbus_RxBuffer[0], modbus_RxBufferPointer, modbus_RxBuffer[modbus_RxBufferPointer], modbus_RxBuffer[modbus_RxBufferPointer]);
        yield();

        if (modbus_RxBuffer[0] != modbusDeviceAddress) {
          modbus_RxBufferPointer = 0;
          DEBUG_V ("modbus device address != received address");
          Serial.flush();
          modbux_RxError = true;
        }
        else
        {
          modbus_RxBufferPointer++;
        }
      }

    } else {
      DEBUG_E("Serial Buffer Overflow!!\n");
      Serial.flush();
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

uint16_t modbus_get_register(uint8_t registerid)
{

  uint8_t bytenr = 3 + (registerid * 2);
  if (bytenr < modbus_RxBufferPointer)
  {
    uint16_t value;
    value = modbus_RxBuffer[bytenr];
    value <<= 8;
    value |= modbus_RxBuffer[bytenr + 1];
    return value;
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
  modbux_RxReady = false;
}

// This function creates a double from 2 16 bit values
double glue(unsigned int d1, unsigned int d0) {
  double t;
  t = d1 << 16;
  t += d0;
  return t;
  }

double modbus_get_two_register_double(uint8_t registerstartid, double devide)
{
  if (registerstartid * 2 <= modbus_RxBufferPointer + 5)
  {
    return glue(modbus_get_register(registerstartid), modbus_get_register(registerstartid+1)) / devide;
  }
  else
  {
    return NAN;
  }
}
