void qswifidimmer_init(const uint8_t nrofchannels, void(*callback)(uint8_t, uint8_t, bool));
void qswifidimmer_setdimvalue(uint8_t dimvalue, uint8_t dimchannel = 0);
uint8_t qswifidimmer_getdimvalue(uint8_t dimchannel = 0);
void qswifidimmer_setdimstate(bool dimstate, uint8_t dimchannel = 0);
bool qswifidimmer_getdimstate(uint8_t dimchannel = 0);
void qswifidimmer_handle();
void qswifidimmer_setdimenabled(bool dimenabled, uint8_t dimchannel);
bool qswifidimmer_getdimenabled(uint8_t dimchannel);
