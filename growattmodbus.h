void growattModbus_init(void(*callback)(const char *,String));
void growattModbus_handle();

#define GROWATTMODBUS_TIMEOUT 10
#define GROWATTMODBUS_WAIT_AFTER_STARTUP_TIMEOUT 5
