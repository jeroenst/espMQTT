void growattModbus_init(void(*callback)(const char *,String), int fanpin = -1);
void growattModbus_handle();

#define GROWATTMODBUS_POLL_SHORT_TIMER 1
#define GROWATTMODBUS_POLL_LONG_TIMER 10
