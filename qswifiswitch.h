/*
 * https://nl.aliexpress.com/item/4000179514914.html?spm=a2g0s.9042311.0.0.27424c4d9XoFO8
 * https://community.home-assistant.io/t/qs-wifi-s04-2c/195663
 */

class QsWifiSwitch {
  private:
    uint8_t nrofchannels = 0;
    bool relayState[2] = {0, 0};
    bool switchState[2] = {0, 0};

    void(*relaycallback)(uint8_t, bool) = NULL;
    void(*switchcallback)(uint8_t, bool) = NULL;

  public:
    QsWifiSwitch(const uint8_t nrofchannels);
    void setRelay(bool state);
    void setRelay(uint8_t relaychannel, bool state);
    bool getRelay(uint8_t relaychannel = 0);
    void handle();
    void setRelayCallback(void(*callback)(uint8_t, bool));
    void setSwitchCallback(void(*callback)(uint8_t, bool));
};
