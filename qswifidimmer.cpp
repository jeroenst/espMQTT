#include <ESP8266WiFi.h>
#include "espMQTT.h"
#include "qswifidimmer.h"

/*
  MOESHOUSE QS WIFI DIMMER D01 (QS-WIFI-D01) not tested
  MOESHOUSE QS WIFI DIMMER D01 (QS-WIFI-D02) tested ok

   GPIO13 = S1
   GPIO5  = S2
*/
static uint8_t qswifidimmer_nrofchannels = 0;
static uint8_t qswifidimmer_dimvalue[2] = {1, 1};
static uint8_t qswifidimmer_dimoffset[2] = {20 , 20};
static bool qswifidimmer_dimstate[2] = {0, 0};
static bool qswifidimmer_dimenabled[2] = {1, 1};

void(*_qswifidimmer_callback)(uint8_t, uint8_t, bool);
void(*_qswifidimmer_switchcallback)(uint8_t, bool);

void qswifidimmer_init(const uint8_t nrofchannels, void(*callback)(uint8_t, uint8_t, bool))
{
  _qswifidimmer_callback = callback;
  Serial.begin(9600);  //Init serial 9600 baud
  Serial.setDebugOutput(false);

  // Define all unused GPIO as output to prevent inteference and slow wifi
  pinMode(0, OUTPUT);
  digitalWrite(0,0);
  pinMode(2, OUTPUT);
  digitalWrite(2,0);
  pinMode(3, FUNCTION_0); // Change RX pin to GPIO
  pinMode(3, OUTPUT);
  digitalWrite(3,0);
  pinMode(4, OUTPUT);
  digitalWrite(4,0);
  // Pin 6 t/m 11 = flash
  pinMode(12, OUTPUT);
  digitalWrite(12,0);
  pinMode(14, OUTPUT);
  digitalWrite(14,0);
  pinMode(15, OUTPUT);
  digitalWrite(15,0);
  pinMode(16, OUTPUT);
  digitalWrite(16,0);

  qswifidimmer_nrofchannels = nrofchannels;
  qswifidimmer_setdimvalue(0, 0);
  qswifidimmer_setdimstate(0, 0);
  pinMode(13, INPUT);
  
  if (qswifidimmer_nrofchannels == 2)
  {
    qswifidimmer_setdimvalue(0, 1);
    qswifidimmer_setdimstate(0, 1);
    qswifidimmer_nrofchannels = 2;
    pinMode(5, INPUT);
  }
  else 
  {
    pinMode(5, OUTPUT);
    digitalWrite(5,0);
  }
}

void qswifidimmer_setswitchcallback(void(*callback)(uint8_t, bool))
{
  _qswifidimmer_switchcallback = callback;
  for (int i = 0; i < qswifidimmer_nrofchannels;  i++) _qswifidimmer_switchcallback(i, false);
}

void qswifidimmer_send(uint8_t dimchannel)
{
  uint16_t calculateddimvalue = (((qswifidimmer_dimvalue[0] * (255 - qswifidimmer_dimoffset[0]))) / 100) + qswifidimmer_dimoffset[0];
  if (qswifidimmer_nrofchannels == 1)
  {
    // Code for QS-WIFI-D01
    Serial.write(0xFF); //Header
    Serial.write(0x55); //Header
    Serial.write(qswifidimmer_dimstate[0] ? calculateddimvalue : 0); //Value CH1
    Serial.write(0x05); //Footer
    Serial.write(0xDC); //Footer
    Serial.write(0x0A); //Footer
  }

  if (qswifidimmer_nrofchannels == 2)
  {
    // Code for QS-WIFI-D02
    Serial.write(0xFF); //Header
    Serial.write(0x55); //Header
    Serial.write(dimchannel + 1); //Channel
    Serial.write(qswifidimmer_dimstate[0] ? calculateddimvalue : 0); //Value CH1
    calculateddimvalue = (((qswifidimmer_dimvalue[1] * (255 - qswifidimmer_dimoffset[1]))) / 100) + qswifidimmer_dimoffset[1];
    Serial.write(qswifidimmer_dimstate[1] ? calculateddimvalue : 0); //Value CH2
    Serial.write(0x05); //Footer
    Serial.write(0xDC); //Footer
    Serial.write(0x0A); //Footer
  }
}

void qswifidimmer_handle()
{
  static bool S[2] = {0, 0};
  static bool Spulse[2] = {0, 0};
  static uint16_t Scounter[2] = {0, 0};
  static uint32_t Stime[2] = {0, 0};
  static uint32_t Stimeout[2] = {0, 0};
  static bool Sdimup[2] = {1, 1};

  for (uint8_t dimchannel = 0; dimchannel < qswifidimmer_nrofchannels; dimchannel++)
  {
    bool Stemp = (dimchannel == 0 ? digitalRead(13) : digitalRead(5));
    Spulse[dimchannel] = 0;
    if (Stemp != S[dimchannel])
    {
      S[dimchannel] = Stemp;
      Scounter[dimchannel]++;
      Stime[dimchannel] = millis();
      Spulse[dimchannel] = 1;
    }

    if (Scounter[dimchannel] == 6)
    {
      _qswifidimmer_switchcallback(dimchannel, true);
    }

    if ((Scounter[dimchannel] > 5) && (Stime[dimchannel] + 100 < millis())) // When switch was pressed before and released 100ms ago
    {
      if (Scounter[dimchannel] < 100)
      {
        qswifidimmer_dimstate[dimchannel] = qswifidimmer_dimstate[dimchannel] ? 0 : 1;
        if (!qswifidimmer_dimenabled[dimchannel])
        {
          qswifidimmer_dimvalue[dimchannel] = qswifidimmer_dimstate[dimchannel] ? 100 : 0;
        }
        qswifidimmer_send(dimchannel);
        _qswifidimmer_callback(dimchannel, qswifidimmer_dimstate[dimchannel] ? qswifidimmer_dimvalue[dimchannel] : 0, qswifidimmer_dimstate[dimchannel]);
      }
      _qswifidimmer_switchcallback(dimchannel, false);
      Scounter[dimchannel] = 0;
      Stime[dimchannel] = 0;
    }

    if (!qswifidimmer_dimstate[dimchannel] && Scounter[dimchannel] > 100) // If lamps are off and switch is pressed long enough, put lamps on with full brightness
    {
        qswifidimmer_dimstate[dimchannel] = true;
        qswifidimmer_dimvalue[dimchannel] = 100;
        qswifidimmer_send(dimchannel);
    }

    if ((qswifidimmer_dimenabled[dimchannel]) && (qswifidimmer_dimstate[dimchannel]))
    {
      if ((Stimeout[dimchannel] > 0) && (Stimeout[dimchannel] < millis()))
      {
        Scounter[dimchannel] = 0;
        Stimeout[dimchannel] = 0;
        Sdimup[dimchannel] = Sdimup[dimchannel] ? 0 : 1;
        _qswifidimmer_callback(dimchannel, qswifidimmer_dimstate[dimchannel] ? qswifidimmer_dimvalue[dimchannel] : 0, qswifidimmer_dimstate[dimchannel]);
      }

      if (Scounter[dimchannel] > 100)
      {
        Stimeout[dimchannel] = millis() + 50;
        if (Sdimup[dimchannel])
        {
          if (qswifidimmer_dimvalue[dimchannel] < 100)
          {
            if ((Scounter[dimchannel] % 3) == 0)
            {
              if (Spulse[dimchannel])
              {
                qswifidimmer_dimvalue[dimchannel] = qswifidimmer_dimvalue[dimchannel] + 1;
                qswifidimmer_send(dimchannel);
              }
            }
          }
          else Sdimup[dimchannel] = 0;
        }
        else
        {
          if (qswifidimmer_dimvalue[dimchannel] > 1)
          {
            if ((Scounter[dimchannel] % 3) == 0)
            {
              if (Spulse[dimchannel])
              {
                qswifidimmer_dimvalue[dimchannel] = qswifidimmer_dimvalue[dimchannel] - 1;
                qswifidimmer_send(dimchannel);
              }
            }
          }
          else Sdimup[dimchannel] = 1;
        }
      }
    }
  }
}

void qswifidimmer_setdimoffset(uint8_t value, uint8_t dimchannel)
{
  if (dimchannel < qswifidimmer_nrofchannels)
  {
    qswifidimmer_dimoffset[dimchannel] = MIN(value, 100);
  }
}

uint8_t qswifidimmer_getdimoffset(uint8_t dimchannel)
{
  if (dimchannel < qswifidimmer_nrofchannels)
  {
    return qswifidimmer_dimoffset[dimchannel];
  }
  return 0;
}

void qswifidimmer_setdimvalue(uint8_t value, uint8_t dimchannel)
{
  if (dimchannel < qswifidimmer_nrofchannels)
  {
    if (value != 0) qswifidimmer_dimvalue[dimchannel] = value;
    qswifidimmer_dimstate[dimchannel] = value > 0 ? true : false;
    qswifidimmer_send(dimchannel);
    _qswifidimmer_callback(dimchannel, qswifidimmer_dimstate[dimchannel] ? qswifidimmer_dimvalue[dimchannel] : 0, qswifidimmer_dimstate[dimchannel]);
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
    qswifidimmer_dimstate[dimchannel] = dimstate;
    DEBUG_V("Setting dimstate Channel=%d, Value=%d...\n", dimchannel, dimstate);
    qswifidimmer_send(dimchannel);
    _qswifidimmer_callback(dimchannel, qswifidimmer_dimstate[dimchannel] ? qswifidimmer_dimvalue[dimchannel] : 0, qswifidimmer_dimstate[dimchannel]);
  }
}

bool qswifidimmer_getdimstate(uint8_t dimchannel)
{
  return dimchannel < qswifidimmer_nrofchannels ? qswifidimmer_dimstate[dimchannel] : 0;
}

void qswifidimmer_setdimenabled(bool dimenabled, uint8_t dimchannel)
{
  if (dimchannel < qswifidimmer_nrofchannels)
  {
    qswifidimmer_dimenabled[dimchannel] = dimenabled;
  }
}

bool qswifidimmer_getdimenabled(uint8_t dimchannel)
{
  return dimchannel < qswifidimmer_nrofchannels ? qswifidimmer_dimenabled[dimchannel] : 0;
}
