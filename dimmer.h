void dimmer_setdimvalue(byte value);
uint8_t dimmer_getdimvalue();

void dimmer_setstate(bool value);
bool dimmer_getstate();

void dimmer_init(uint8_t zerocrosspin, uint8_t triacpin, bool fade = true);
void dimmer_stop();

void dimmer_handle();
