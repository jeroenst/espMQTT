void growatt_init(void(*callback)(const char *,String), int fanpin = -1);
void growatt_handle();

#define GROWATT_FANSPEED_OFFSET 150
#define GROWATT_FANSPEED_TEMP 47
#define GROWATT_POLL_TIMER 3
