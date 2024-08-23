void growattModbus_init();
void growattModbus_handle();

// RX Error after 5 seconds and wait 5 seconds when all data is received before next requests
#define GROWATTMODBUS_TIMEOUT 5
// After 10 read errors make device offline and set status to commerror
#define GROWATTMODBUS_MAX_READ_ERRORS 10
