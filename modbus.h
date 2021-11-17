void modbus_clear_buffer(); // Clears the buffer, get function will not return data anymore

uint8_t modbus_get_byte(uint8_t bytenr);
double modbus_get_two_register_double(uint8_t registerstartid, double devide = 1);
uint16_t modbus_get_register(uint8_t registerid);

void modbus_handle(); // Has to be called continously to handle incomming serial data

void modbus_request_function_code(uint8_t deviceAddress, uint8_t functionCode, uint16_t startAddress, uint16_t numberOfAddresses); // Request a function from modbus device

void modbus_request_holding_register(uint8_t deviceAddress, uint16_t startRegister, uint16_t numberOfRegisters); // Request a holding register from modbus device
void modbus_request_input_register(uint8_t deviceAddress, uint16_t startRegister, uint16_t numberOfRegisters); // Request a input register from modbus device

bool modbus_rx_ready(); // Return true when data is received correctly from modbus device and get functions are ready for use
