#include "espMQTT.h"
#include "qswifidimmer.h"

/*
  MOESHOUSE QS WIFI DIMMER D01 (QS-WIFI-D01) tested ok
  MOESHOUSE QS WIFI DIMMER D01 (QS-WIFI-D02) tested ok

   GPIO13 = S1
   GPIO5  = S2
*/
static uint8_t qswifidimmer_nrofchannels = 0;
static uint8_t qswifidimmer_dimvalue[2] = {100, 100};
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
  digitalWrite(0, 0);
  // GPIO 1 is TX
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  pinMode(3, FUNCTION_0); // Change RX pin to GPIO
  pinMode(3, OUTPUT);
  digitalWrite(3, 0);
  pinMode(4, OUTPUT);
  // Gpio 5 = Switch input 2
  digitalWrite(4, 0);
  // Pin 6 t/m 11 = flash
  pinMode(12, OUTPUT);
  digitalWrite(12, 0);
  // Gpio 13 = Switch input 1
  pinMode(14, OUTPUT);
  digitalWrite(14, 0);
  pinMode(15, OUTPUT);
  digitalWrite(15, 0);
  pinMode(16, OUTPUT);
  digitalWrite(16, 0);

  qswifidimmer_nrofchannels = nrofchannels;
  qswifidimmer_setdimvalue(0, 0);
  qswifidimmer_setdimstate(0, 0);
  pinMode(13, INPUT_PULLUP);

  if (qswifidimmer_nrofchannels == 2)
  {
    qswifidimmer_setdimvalue(0, 1);
    qswifidimmer_setdimstate(0, 1);
    pinMode(5, INPUT_PULLUP);
  }
  else
  {
    qswifidimmer_nrofchannels = 1;
    pinMode(5, OUTPUT);
    digitalWrite(5, 0);
  }
}

void qswifidimmer_setswitchcallback(void(*callback)(uint8_t, bool))
{
  _qswifidimmer_switchcallback = callback;
  for (uint8_t i = 0; i < qswifidimmer_nrofchannels;  i++) _qswifidimmer_switchcallback(i, false);
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
  static bool fullbrightstart[2] = {0, 0};
  static uint8_t dimchannel = 0;

  uint32_t msec = millis(); // store millis in variable because otherwise wifi gets slow.
  // Iterate trough dimchannels (D01 or D02 dimmer)
  if (dimchannel >= qswifidimmer_nrofchannels) dimchannel = 0;
  yield();

  // Count pulses on input (50hz when button is pressed, but 2 pulses per full sine, so 10ms per pulse)
  bool Stemp = (dimchannel == 0 ? digitalRead(13) : digitalRead(5));
  Spulse[dimchannel] = 0;
  if (Stemp != S[dimchannel])
  {
    S[dimchannel] = Stemp;
    Scounter[dimchannel]++;
    Stime[dimchannel] = msec;
    Spulse[dimchannel] = 1;
  }
  yield();

  // If 6 pulses are counted do a callback to inform switch is pressed
  if (Scounter[dimchannel] == 6)
  {
    // Callback switch is pressed
    _qswifidimmer_switchcallback(dimchannel, true);
  }
  yield();

  // Filter ghost pulses
  if (Stime[dimchannel] + 150 < msec) Scounter[dimchannel] = 0;

  // When switch was pressed for more then 5 pulses and released for about 100ms switch from on to off or from off to on and also reset counters etc.
  if ((Scounter[dimchannel] > 5) && (Stime[dimchannel] + 100 < msec))
  {
    if (Scounter[dimchannel] < 100)
    {
      // Toggle lamp on / off
      qswifidimmer_dimstate[dimchannel] = qswifidimmer_dimstate[dimchannel] ? 0 : 1;

      // If dimming is disabled set dimvalue to 100 when dimstate is 1 or 0 if dimstate is 0
      if (!qswifidimmer_dimenabled[dimchannel])
      {
        qswifidimmer_dimvalue[dimchannel] = qswifidimmer_dimstate[dimchannel] ? 100 : 0;
      }

      // Send update to dimchannel
      qswifidimmer_send(dimchannel);

      // Callback to parent
      _qswifidimmer_callback(dimchannel, qswifidimmer_dimstate[dimchannel] ? qswifidimmer_dimvalue[dimchannel] : 0, qswifidimmer_dimstate[dimchannel]);
    }
    // Callback switch is released
    _qswifidimmer_switchcallback(dimchannel, false);
    Scounter[dimchannel] = 0;
    Stime[dimchannel] = 0;
    fullbrightstart[dimchannel] = 0;
  }
  yield();

  // If lamps are off and switch is pressed more than 100 pulses (10ms*100=2000ms), put lamps on with full brightness
  if (!qswifidimmer_dimstate[dimchannel] && Scounter[dimchannel] > 100)
  {
    qswifidimmer_dimstate[dimchannel] = true;
    qswifidimmer_dimvalue[dimchannel] = 100;
    qswifidimmer_send(dimchannel);
    _qswifidimmer_callback(dimchannel, qswifidimmer_dimstate[dimchannel] ? qswifidimmer_dimvalue[dimchannel] : 0, qswifidimmer_dimstate[dimchannel]);
    fullbrightstart[dimchannel] = 1;
  }
  yield();


  // If dimming for this channel is enabled and state is on and lamps are not just set on with fullbright start changing dimlevel
  if ((qswifidimmer_dimenabled[dimchannel]) && (qswifidimmer_dimstate[dimchannel]) && !fullbrightstart[dimchannel])
  {
    // When switch is released reset pulse counter and timeout, change dim direction and send values to callback handler
    if ((Stimeout[dimchannel] > 0) && (Stimeout[dimchannel] < msec))
    {
      Scounter[dimchannel] = 0;
      Stimeout[dimchannel] = 0;
      Sdimup[dimchannel] = Sdimup[dimchannel] ? 0 : 1;
      _qswifidimmer_callback(dimchannel, qswifidimmer_dimstate[dimchannel] ? qswifidimmer_dimvalue[dimchannel] : 0, qswifidimmer_dimstate[dimchannel]);
    }

    if (Scounter[dimchannel] > 100)
    {
      Stimeout[dimchannel] = msec + 50;
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
  yield();
  dimchannel++; 
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
    if ((qswifidimmer_dimvalue[dimchannel] != value) || !qswifidimmer_dimstate[dimchannel])
    {
      if (value != 0) qswifidimmer_dimvalue[dimchannel] = value;
      qswifidimmer_dimstate[dimchannel] = value > 0 ? true : false;
      qswifidimmer_send(dimchannel);
      _qswifidimmer_callback(dimchannel, qswifidimmer_dimstate[dimchannel] ? qswifidimmer_dimvalue[dimchannel] : 0, qswifidimmer_dimstate[dimchannel]);
    }
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
    if (qswifidimmer_dimstate[dimchannel] != dimstate)
    {
      qswifidimmer_dimstate[dimchannel] = dimstate;
      DEBUG_V("Setting dimstate Channel=%d, Value=%d...\n", dimchannel, dimstate);
      qswifidimmer_send(dimchannel);
      _qswifidimmer_callback(dimchannel, qswifidimmer_dimstate[dimchannel] ? qswifidimmer_dimvalue[dimchannel] : 0, qswifidimmer_dimstate[dimchannel]);
    }
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
