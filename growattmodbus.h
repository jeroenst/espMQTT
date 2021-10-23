void growattModbus_init(void(*callback)(String,String), int fanpin = -1);
void growattModbus_handle();

#define GROWATTMODBUS_POLL_TIMER 3
