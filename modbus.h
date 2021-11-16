void modbus_clear_buffer();

uint8_t modbus_get_byte(uint8_t bytenr);
double modbus_get_two_register_double(uint8_t registerstartid, double devide = 1);
uint16_t modbus_get_register(uint8_t registerid);

uint8_t modbus_handle();
void modbus_request_function_code(uint8_t deviceAddress, uint8_t functionCode, uint16_t startAddress, uint16_t numberOfAddresses);
bool modbus_rx_ready();
