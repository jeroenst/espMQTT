#include <ESP8266WiFi.h>
#include "espMQTT.h"
#include "qswifidimmer.h"

static uint8_t qswifidimmer_nrofchannels = 0;
static uint8_t qswifidimmer_dimvalue[2] = {0, 0};
static uint8_t qswifidimmer_minimaldimvalue[2] = {50 , 50};
static bool qswifidimmer_state[2];

void qswifidimmer_init(const uint8_t nrofchannels)
{
  Serial.begin(9600);  //Init serial 9600 baud
  Serial.setDebugOutput(false);
  qswifidimmer_nrofchannels = nrofchannels;
  qswifidimmer_setdim(0, 0);
  if (qswifidimmer_nrofchannels > 1) qswifidimmer_setdim(1, 0);
  pinMode(4, OUTPUT); // This is needed because pin 4 is floating and causes bad WIFI connection when undefined.  
  digitalWrite(4, 1);
}

void qswifidimmer_send(uint8_t dimchannel)
{
    if (qswifidimmer_nrofchannels == 1)
    {
      // Code for QS-WIFI-D01
      Serial.write(0xFF); //Header
      Serial.write(0x55); //Header
      uint16_t calculateddimvalue = (((qswifidimmer_dimvalue[0] * (255 - qswifidimmer_minimaldimvalue[0]))) / 100) + qswifidimmer_minimaldimvalue[0];
      Serial.write(qswifidimmer_state[0] ? calculateddimvalue : 0); //Value CH1
      Serial.write(0x05); //Footer
      Serial.write(0xDC); //Footer
      Serial.write(0x0A); //Footer
    }
    if (qswifidimmer_nrofchannels == 2)
    {
      // Code for QS-WIFI-D02
      DEBUG_V("Sending dim states\n");
      Serial.write(0xFF); //Header
      Serial.write(0x55); //Header
      Serial.write(dimchannel + 1); //Channel
      uint16_t calculateddimvalue = (((qswifidimmer_dimvalue[0] * (255 - qswifidimmer_minimaldimvalue[0]))) / 100) + qswifidimmer_minimaldimvalue[0];
      Serial.write(qswifidimmer_state[0] ? calculateddimvalue : 0); //Value CH1
      calculateddimvalue = (((qswifidimmer_dimvalue[1] * (255 - qswifidimmer_minimaldimvalue[1]))) / 100) + qswifidimmer_minimaldimvalue[1];
      Serial.write(qswifidimmer_state[1] ? calculateddimvalue : 0); //Value CH2
      Serial.write(0x05); //Footer
      Serial.write(0xDC); //Footer
      Serial.write(0x0A); //Footer
    }
}

void qswifidimmer_setminimaldim(uint8_t value, uint8_t dimchannel)
{
  if (dimchannel < qswifidimmer_nrofchannels)
  {
    qswifidimmer_minimaldimvalue[dimchannel] = MIN(value, 100);
  }  
}
void qswifidimmer_setdim(uint8_t value, uint8_t dimchannel)
{
  if (dimchannel < qswifidimmer_nrofchannels)
  {
    qswifidimmer_dimvalue[dimchannel] = value;
    qswifidimmer_state[dimchannel] = value > 0 ? true : false;
    qswifidimmer_send(dimchannel);
  }
}

uint8_t qswifidimmer_getdim(uint8_t dimchannel)
{
  return dimchannel < qswifidimmer_nrofchannels ? qswifidimmer_dimvalue[dimchannel] : 0;
}

void qswifidimmer_setstate(bool state, uint8_t dimchannel)
{
  if (dimchannel < qswifidimmer_nrofchannels)
  {
    if (qswifidimmer_nrofchannels == qswifidimmer_nrofchannels)
    {
      qswifidimmer_state[dimchannel] = state;
      DEBUG_V("Setting dim state Channel=%d, Value=%d...\n", dimchannel, state);
      qswifidimmer_send(dimchannel);
    }
  }
}

bool qswifidimmer_getstate(uint8_t dimchannel)
{
  return dimchannel < qswifidimmer_nrofchannels ? qswifidimmer_state[dimchannel] : 0;
}
