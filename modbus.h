void modbus_clear_buffer();
double modbus_get_double(uint8_t startbytenr, uint16_t devide = 1);
uint8_t modbus_get_byte(uint8_t bytenr);
uint8_t modbus_handle();
void modbus_request_function_code(uint8_t deviceAddress, uint8_t functionCode, uint16_t startAddress, uint16_t numberOfAddresses);
bool modbus_rx_ready();
