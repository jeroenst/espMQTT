void i2cEeprom_init(uint8_t i2c_sda, uint8_t i2c_scl, uint16_t eepromaddress, uint16_t eeprombytes, void(*debugcallback)(String, String));
uint32_t i2cEeprom_read();
void i2cEeprom_write(uint32_t value);
