uint32_t watermeter_getliters();
void watermeter_setliters(uint32_t liters);
uint32_t watermeter_getflow();
void watermeter_init(uint8_t watermeter_pulsepin, uint8_t watermeter_ledpin, uint32_t watermeter_liters);
bool watermeter_handle();
