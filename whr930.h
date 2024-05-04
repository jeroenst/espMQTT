#ifndef ZEHNDER_WHR930_h
#define ZEHNDER_WHR930_h

#include "espMQTT.h"

class ZEHNDER_WHR930
{
  private:
    uint8_t sendCounter = 0;
    uint8_t sendCleared = false;
    struct 
    {
      uint8_t exhaustabsent = 10;
      uint8_t exhaustlow = 20;
      uint8_t exhaustmedium = 50;
      uint8_t exhausthigh = 90;
      uint8_t intakeabsent = 15;
      uint8_t intakelow = 25;
      uint8_t intakemedium = 55;
      uint8_t intakehigh = 95;
      bool send = 0;
    } fanspeed;

    struct
    {
      uint8_t level;
      bool send = 0;
    } fanlevel;

    struct
    {
      uint8_t calculated_temperature;
      bool send = 0;
    } comfort;

    void sendPacket(uint16_t command, uint8_t *data = nullptr, uint8_t length = 0);
    void requestData(bool startSequence = false);
    static void mqttClientCallback(char* topic, byte* payload, unsigned int length);
  public:
    void loop();
    void setup();
    static void secondTick(uint16_t uptime);
    void setfanlevel(uint8_t level);
    void setcomforttemperature(float temperature); // In 0,5 degrees celcius
};

extern ZEHNDER_WHR930 zehnder_whr930;


#endif
