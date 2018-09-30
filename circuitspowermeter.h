void circuitspowermeter_init(uint8_t ads0_cs_pin, uint8_t ads0_rdy_pin, uint8_t ads1_cs_pin, uint8_t ads1_rdy_pin);
void circuitspowermeter_read(uint8_t circuitnr, int32_t &mW, int32_t &mVA, int32_t &mA, int32_t &mV, uint8_t nrofsamples);
