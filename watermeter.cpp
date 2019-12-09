#include <ESP8266WiFi.h>

uint32_t _watermeter_liters = 0;
double _watermeter_lmin = 0;
uint8_t _watermeter_pulsepin = 0;
uint8_t _watermeter_ledpin = 0;

uint32_t watermeter_getliters()
{
   return _watermeter_liters;
}

void watermeter_setliters(uint32_t liters)
{
   _watermeter_liters = liters;
}

uint32_t watermeter_getflow()
{
   return _watermeter_lmin;
}

void watermeter_init(uint8_t watermeter_pulsepin, uint8_t watermeter_ledpin, uint32_t watermeter_liters)
{
  _watermeter_pulsepin = watermeter_pulsepin;
  _watermeter_ledpin = watermeter_ledpin;
  _watermeter_liters = watermeter_liters;
  pinMode(_watermeter_pulsepin, INPUT);
  pinMode(_watermeter_ledpin, OUTPUT);
  digitalWrite(_watermeter_ledpin, digitalRead(_watermeter_pulsepin) ? 0 : 1);
}

bool watermeter_handle()
{
  static long long lmincountdownmillis = 0;
  static long long last_pulse_time = 0;
  static bool oldinputpinstate = digitalRead(_watermeter_pulsepin);
  bool inputpinstate = digitalRead(_watermeter_pulsepin);
  bool returnvalue = false;
  digitalWrite(_watermeter_ledpin, !inputpinstate);
  if ((inputpinstate != oldinputpinstate) && (inputpinstate == 0))
  {
    long long pulse_time = millis() - last_pulse_time;
    last_pulse_time = millis();
    if (pulse_time > 1200) // Filter pulses less than 1200 ms which is more than 50l/min which is bouncing
    {
      _watermeter_lmin = double(60000) / pulse_time;
      _watermeter_liters++;
      lmincountdownmillis = millis() + (pulse_time * 2);
      returnvalue = true;
    }
  }
  oldinputpinstate = inputpinstate;

  if ((_watermeter_lmin > 0) && (lmincountdownmillis < millis()))
  {
      _watermeter_lmin = _watermeter_lmin / 2;
      lmincountdownmillis = millis() + (60000 / _watermeter_lmin) + 1000;
      if (_watermeter_lmin <= 0.5)
      {
        _watermeter_lmin = 0;
      }
      returnvalue = true;
  }
  return returnvalue;
}
