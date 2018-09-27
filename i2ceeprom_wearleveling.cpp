#include <ESP8266WiFi.h>
#include <Wire.h>

uint8_t _i2cEeprom_sda = 0;
uint8_t _i2cEeprom_scl = 0;
uint16_t _i2cEeprom_address = 0;
uint16_t _i2cEeprom_bytes = 0;
static uint16_t _i2cEeprom_WritememPage = 0;

void i2cEeprom_init(uint8_t i2c_sda, uint8_t i2c_scl, uint16_t eepromaddress, uint16_t eeprombytes)
{
  _i2cEeprom_sda = i2c_sda;
  _i2cEeprom_scl = i2c_scl;
  _i2cEeprom_address = eepromaddress;
  _i2cEeprom_bytes = eeprombytes;
  Wire.begin(_i2cEeprom_sda, _i2cEeprom_scl);
}

void i2cEeprom_WriteByte(uint16_t theMemoryAddress, uint8_t u8Byte)
{
  delay(1);
  Wire.beginTransmission(_i2cEeprom_address);
  delay(1);
  Wire.write( (theMemoryAddress >> 8) & 0xFF );
  delay(1);
  Wire.write( (theMemoryAddress >> 0) & 0xFF );
  delay(1);
  Wire.write(u8Byte);
  delay(1);
//  if (Wire.endTransmission() != 0) DEBUG("Failing writing I2C eeprom!\n");
}

uint8_t i2cEeprom_ReadByte(uint16_t theMemoryAddress)
{
  uint8_t u8retVal = 0;
  Wire.beginTransmission(_i2cEeprom_address);
  Wire.write( (theMemoryAddress >> 8) & 0xFF );
  Wire.write( (theMemoryAddress >> 0) & 0xFF );
  Wire.endTransmission();
//  if (Wire.requestFrom(_i2cEeprom_address, 1) == 0) DEBUG("Failing reading I2C eeprom!\n");
  u8retVal = Wire.read();
  return u8retVal ;
}

void i2cEeprom_Writeuint32crc(uint16_t memoryPage, uint32_t data)
{
  uint8_t readbyte = 0;
  uint8_t calccrc = 0;
  uint16_t theMemoryAddress = memoryPage * 128;
  for (uint8_t pointer = 0; pointer < 4; pointer++)
  {
    uint8_t writebyte = (data >> (8 * (3 - pointer))) & 0xFF;
    i2cEeprom_WriteByte(theMemoryAddress + pointer, writebyte);
    calccrc += writebyte;
  }
  i2cEeprom_WriteByte(theMemoryAddress + 4, calccrc);
  //DEBUG("Written i2c eeprom startaddress %u, value %u, crc %u\n", theMemoryAddress, data, calccrc);
}

uint32_t i2cEeprom_Readuint32crc(uint16_t memoryPage)
{
  uint8_t readbyte = 0;
  uint8_t calccrc = 0;
  uint16_t theMemoryAddress = memoryPage * 128;
  uint32_t returnval = 0;
  for (uint8_t pointer = 0; pointer < 4; pointer++)
  {
    readbyte = i2cEeprom_ReadByte(theMemoryAddress + pointer);
    uint32_t readlong = readbyte;
    returnval |= readlong << (8 * (3 - pointer));
    calccrc += readbyte;
  }
  uint8_t readcrc = i2cEeprom_ReadByte(theMemoryAddress + 4);
  if (readcrc != calccrc)
  {
  //  DEBUG("Read i2c eeprom CRC error, page=%u, value=%u, readcrc=%u, calccrc=%u", memoryPage, returnval, readcrc, calccrc);
    returnval = 0xFFFF;
  }
  //  DEBUG("Read i2c eeprom startaddress %u, value %ul, crc %u, calccrc %u\n", theMemoryAddress, returnval, readcrc, calccrc);
  return returnval;
}

uint32_t i2cEeprom_read()
{
  uint32_t watermetercounter = 0;
  uint16_t watermeterpage = 0;
  for (uint16_t memPage = 0; memPage <= 4000; memPage++) // Read data from different pages
  {
    uint32_t eepromdata = i2cEeprom_Readuint32crc(memPage);
    _i2cEeprom_WritememPage = memPage;
    if (eepromdata == 0xFFFF) break;
    watermetercounter = eepromdata; // Get highest value (= latest value) from eeprom;
    watermeterpage = memPage;
    yield();
    ESP.wdtFeed(); // Prevent HW WD to kick in...
  }
//  DEBUG("Read %u from memPage %u\n", watermetercounter, watermeterpage);
  return watermetercounter;
}

void i2cEeprom_write(uint32_t value)
{
  if (_i2cEeprom_WritememPage >= 4000) _i2cEeprom_WritememPage = 0;
  i2cEeprom_Writeuint32crc(_i2cEeprom_WritememPage, value);
  i2cEeprom_Writeuint32crc(_i2cEeprom_WritememPage + 1, 0xFFFF);
  _i2cEeprom_WritememPage++;
}
