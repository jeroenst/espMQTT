#include <ESP8266WiFi.h>
#include "espMQTT.h"
#include "qswifidimmer.h"

static uint8_t qswifidimmer_nrofchannels = 0;
static uint8_t qswifidimmer_dimvalue[2] = {0, 0};
static uint8_t qswifidimmer_minimaldimvalue[2] = {50 , 50};
static bool qswifidimmer_dimstate[2];

void(*_qswifidimmer_callback)(uint8_t, uint8_t, bool);

void qswifidimmer_init(const uint8_t nrofchannels, void(*callback)(uint8_t, uint8_t, bool))
{
  _qswifidimmer_callback = callback;
  Serial.begin(9600);  //Init serial 9600 baud
  Serial.setDebugOutput(false);
  qswifidimmer_nrofchannels = nrofchannels;
  qswifidimmer_setdimvalue(0, 0);
  if (qswifidimmer_nrofchannels > 1) qswifidimmer_setdimvalue(0, 1);
  pinMode(4, OUTPUT); // This is needed because pin 4 is floating and causes bad WIFI connection when undefined.  
  digitalWrite(4, 1);
}

void qswifidimmer_handle()
{
}

void qswifidimmer_send(uint8_t dimchannel)
{
    if (qswifidimmer_nrofchannels == 1)
    {
      // Code for QS-WIFI-D01
      Serial.write(0xFF); //Header
      Serial.write(0x55); //Header
      uint16_t calculateddimvalue = (((qswifidimmer_dimvalue[0] * (255 - qswifidimmer_minimaldimvalue[0]))) / 100) + qswifidimmer_minimaldimvalue[0];
      Serial.write(qswifidimmer_dimstate[0] ? calculateddimvalue : 0); //Value CH1
      Serial.write(0x05); //Footer
      Serial.write(0xDC); //Footer
      Serial.write(0x0A); //Footer
    }
    if (qswifidimmer_nrofchannels == 2)
    {
      // Code for QS-WIFI-D02
      DEBUG_V("Sending dimstates\n");
      Serial.write(0xFF); //Header
      Serial.write(0x55); //Header
      Serial.write(dimchannel + 1); //Channel
      uint16_t calculateddimvalue = (((qswifidimmer_dimvalue[0] * (255 - qswifidimmer_minimaldimvalue[0]))) / 100) + qswifidimmer_minimaldimvalue[0];
      Serial.write(qswifidimmer_dimstate[0] ? calculateddimvalue : 0); //Value CH1
      calculateddimvalue = (((qswifidimmer_dimvalue[1] * (255 - qswifidimmer_minimaldimvalue[1]))) / 100) + qswifidimmer_minimaldimvalue[1];
      Serial.write(qswifidimmer_dimstate[1] ? calculateddimvalue : 0); //Value CH2
      Serial.write(0x05); //Footer
      Serial.write(0xDC); //Footer
      Serial.write(0x0A); //Footer
    }

    _qswifidimmer_callback(dimchannel, qswifidimmer_dimvalue[dimchannel], qswifidimmer_dimstate[dimchannel]);
}

void qswifidimmer_setminimaldimvalue(uint8_t value, uint8_t dimchannel)
{
  if (dimchannel < qswifidimmer_nrofchannels)
  {
    qswifidimmer_minimaldimvalue[dimchannel] = MIN(value, 100);
  }  
}
void qswifidimmer_setdimvalue(uint8_t value, uint8_t dimchannel)
{
  if (dimchannel < qswifidimmer_nrofchannels)
  {
    qswifidimmer_dimvalue[dimchannel] = value;
    qswifidimmer_dimstate[dimchannel] = value > 0 ? true : false;
    qswifidimmer_send(dimchannel);
  }
}

uint8_t qswifidimmer_getdimvalue(uint8_t dimchannel)
{
  return dimchannel < qswifidimmer_nrofchannels ? qswifidimmer_dimvalue[dimchannel] : 0;
}

void qswifidimmer_setdimstate(bool dimstate, uint8_t dimchannel)
{
  if (dimchannel < qswifidimmer_nrofchannels)
  {
    if (qswifidimmer_nrofchannels == qswifidimmer_nrofchannels)
    {
      qswifidimmer_dimstate[dimchannel] = dimstate;
      DEBUG_V("Setting dimstate Channel=%d, Value=%d...\n", dimchannel, dimstate);
      qswifidimmer_send(dimchannel);
    }
  }
}

bool qswifidimmer_getdimstate(uint8_t dimchannel)
{
  return dimchannel < qswifidimmer_nrofchannels ? qswifidimmer_dimstate[dimchannel] : 0;
}
