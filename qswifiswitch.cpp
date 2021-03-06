#include "espMQTT.h"
#include "qswifiswitch.h"

#define QSWIFISWITCH1C_OUTPUT 0
#define QSWIFISWITCH1C_INPUT 0

#define QSWIFISWITCH2C_OUTPUT1 14
#define QSWIFISWITCH2C_OUTPUT2 15

#define QSWIFISWITCH2C_INPUT1 13
#define QSWIFISWITCH2C_INPUT2 12


QsWifiSwitch::QsWifiSwitch(const uint8_t nrofchannels)
{
  this->nrofchannels = nrofchannels;

  switch (nrofchannels)
  {
    case 1:
      digitalWrite(QSWIFISWITCH1C_OUTPUT, 0);
      pinMode(QSWIFISWITCH1C_OUTPUT, OUTPUT);
      break;

    case 2:
      digitalWrite(QSWIFISWITCH2C_OUTPUT1, 0);
      pinMode(QSWIFISWITCH2C_OUTPUT1, OUTPUT);
      digitalWrite(QSWIFISWITCH2C_OUTPUT2, 0);
      pinMode(QSWIFISWITCH2C_OUTPUT2, OUTPUT);

      pinMode(QSWIFISWITCH2C_INPUT1, INPUT_PULLUP);
      pinMode(QSWIFISWITCH2C_INPUT2, INPUT_PULLUP);
      break;
  }
}

void QsWifiSwitch::setRelay(bool state)
{
  this->setRelay(0, state);
}

void QsWifiSwitch::setRelay(uint8_t channel, bool state)
{
  this->relayState[channel] = state;

  switch (nrofchannels)
  {
    case 1:
      break;
    case 2:
      if (channel ==  0)
      {
        digitalWrite(QSWIFISWITCH2C_OUTPUT1, state);
        if (relaycallback != NULL) relaycallback(channel, state);
      }
      if (channel ==  1)
      {
        digitalWrite(QSWIFISWITCH2C_OUTPUT2, state);
        if (relaycallback != NULL) relaycallback(channel, state);
      }
      break;
  }
}

bool QsWifiSwitch::getRelay(uint8_t channel)
{
  return relayState[channel];
}

void QsWifiSwitch::handle()
{
  static bool firstrun = true;
  static uint8_t Spulses[2] = {0, 0};
  static bool S[2] = {0, 0};
  static uint32_t Stimeout[2] = {0, 0};

  switch (nrofchannels)
  {
    case 1:
      break;

    case 2:
      for (uint8_t channel = 0; channel < 2; channel++)
      {
        yield();

        // If firstrun do callbacks
        if ((firstrun) && (switchcallback != NULL)) switchcallback(channel, false);

        // If receiving pulses switchstate = 1
        bool input = digitalRead(channel ? QSWIFISWITCH2C_INPUT2 : QSWIFISWITCH2C_INPUT1);
        if (S[channel] != input)
        {
          S[channel] = input;
          Spulses[channel]++;
          Stimeout[channel] = millis() + 100;
        }

        // If received 10 pulses (100ms) switchstate = 1
        if (Spulses[channel] > 10)
        {
          Spulses[channel] = 10;
          if ((!switchState[channel]) && (switchcallback != NULL)) switchcallback(channel, true);
          switchState[channel] = true;
        }

        // If not receiving pules for 100ms switchstate = 0
        if (Stimeout[channel] < millis())
        {
          Spulses[channel] = 0;
          if ((switchState[channel]) && (switchcallback != NULL)) switchcallback(channel, false);
          switchState[channel] = false;
        }
      }
      break;
  }
  firstrun = false;
}

void QsWifiSwitch::setRelayCallback(void(*callback)(uint8_t, bool))
{
  yield();
  this->relaycallback = callback;
  yield();
}

void QsWifiSwitch::setSwitchCallback(void(*callback)(uint8_t, bool))
{
  yield();
  this->switchcallback = callback;
  yield();
}
