void growatt_init(void(*callback)(String,String), int fanpin = -1);
void growatt_handle();

#define GROWATT_FANSPEED_OFFSET 300
#define GROWATT_FANSPEED_TEMP 45
#define GROWATT_POLL_TIMER 3
