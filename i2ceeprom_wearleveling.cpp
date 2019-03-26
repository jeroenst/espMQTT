#include <ESP8266WiFi.h>
#include <Wire.h>
#include <RemoteDebug.h>
// DO NOT DISABLE DEBUG, READING EEPROM WILL NOT WORK..
static uint8_t _i2cEeprom_sda = 0;
static uint8_t _i2cEeprom_scl = 0;
static uint16_t _i2cEeprom_address = 0;
static uint16_t _i2cEeprom_bytes = 0;
static uint16_t _i2cEeprom_WritememPage = 0;
void(*_i2cEeprom_debug_callback)(String, String);

#define _DEBUG(message) _i2cEeprom_debug_callback(__func__, message);

void i2cEeprom_init(uint8_t i2c_sda, uint8_t i2c_scl, uint16_t eepromaddress, uint16_t eeprombytes, void(*debugcallback)(String, String))
{
  _i2cEeprom_debug_callback = debugcallback;
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
  if (Wire.endTransmission() != 0) _DEBUG("Failing writing I2C eeprom!\n");
}

uint8_t i2cEeprom_ReadByte(uint32_t theMemoryAddress)
{
  uint8_t u8retVal = 0;
  Wire.beginTransmission(_i2cEeprom_address);
  Wire.write( (theMemoryAddress >> 8) & 0xFF );
  Wire.write( (theMemoryAddress >> 0) & 0xFF );
  Wire.endTransmission();
 if (Wire.requestFrom(_i2cEeprom_address, 1) == 0) _DEBUG("Failing reading I2C eeprom!\n");
  u8retVal = Wire.read();
  return u8retVal ;
}

void i2cEeprom_Writeuint32crc(uint32_t memoryPage, uint32_t data)
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
  _DEBUG("Written i2c eeprom startaddress "+String(theMemoryAddress)+", value "+data+", crc "+calccrc+"\n");
}

uint32_t i2cEeprom_Readuint32crc(uint16_t memoryPage)
{
  uint8_t readbyte = 0;
  uint8_t calccrc = 0;
  uint32_t theMemoryAddress = memoryPage * 128;
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
    _DEBUG("Read i2c eeprom CRC error, page="+String(memoryPage)+", value="+String(returnval)+", readcrc="+String(readcrc)+", calccrc="+String(calccrc)+"\n");
    returnval = 0xFFFF;
  }
  _DEBUG("Read i2c eeprom startaddress "+String(theMemoryAddress)+", value "+String(returnval)+", crc "+String(readcrc)+", calccrc "+String(calccrc)+"\n");
  return returnval;
}

uint32_t i2cEeprom_read()
{
  uint32_t watermetercounter = 0;
  uint16_t watermeterpage = 0;
  for (uint16_t memPage = 0; memPage < 4000; memPage++) // Read data from different pages
  {
    uint32_t eepromdata = i2cEeprom_Readuint32crc(memPage);
    _i2cEeprom_WritememPage = memPage;
    if (eepromdata == 0xFFFFFFFF) break;
    watermetercounter = eepromdata; // Get highest value (= latest value) from eeprom;
    watermeterpage = memPage;
    yield();
    ESP.wdtFeed(); // Prevent HW WD to kick in...
  }
  _DEBUG("Read "+String(watermetercounter)+" from memPage "+String(watermeterpage)+"\n");
  return watermetercounter;
}

void i2cEeprom_write(uint32_t value)
{
  if (_i2cEeprom_WritememPage >= 3999) _i2cEeprom_WritememPage = 0;
  i2cEeprom_Writeuint32crc(_i2cEeprom_WritememPage, value);
  i2cEeprom_Writeuint32crc(_i2cEeprom_WritememPage + 1, 0xFFFFFFFF);
  _i2cEeprom_WritememPage++;
}
