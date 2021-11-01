void bht002_init(void(*callback)(char *,String));
void bht002_connected();
void bht002_disconnected();
void bht002_handle();

void bht002_setsetpoint(uint8_t temperature);
int8_t bht002_getsetpoint();
int8_t bht002_gettemperature();
int8_t bht002_getheating();

void bht002_senddebug(uint8_t cmdnr);
