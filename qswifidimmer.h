void qswifidimmer_init(const uint8_t nrofchannels);
void qswifidimmer_setdim(uint8_t value, uint8_t dimchannel = 0);
uint8_t qswifidimmer_getdim(uint8_t dimchannel = 0);
void qswifidimmer_setstate(bool state, uint8_t dimchannel = 0);
bool qswifidimmer_getstate(uint8_t dimchannel = 0);
