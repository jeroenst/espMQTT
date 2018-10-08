#include <ESP8266WiFi.h>
#include "hw_timer.h"

bool dimmer_fade = 1;
bool dimmer_state = 1;
uint8_t dimmer_setBrightness = 0;
uint8_t dimmer_tarBrightness = 0;
uint8_t dimmer_curBrightness = 0;
uint16_t dimmer_dimDelay = 0;
uint8_t dimmer_zerocrosspin = 0;
uint8_t dimmer_triacpin = 0;
uint8_t dimmer_fadedelay = 0;

void dimmer_dimTimerISR();
void dimmer_zcDetectISR();

void dimmer_setdimvalue(byte value)
{
  dimmer_setBrightness = value;
  if (dimmer_state) dimmer_tarBrightness = dimmer_setBrightness;
}

uint8_t dimmer_getdimvalue()
{
  return dimmer_tarBrightness;
}

void dimmer_setstate(bool value)
{
  dimmer_state = value;
  if (dimmer_state) dimmer_tarBrightness = dimmer_setBrightness;
  else dimmer_tarBrightness = 0;
}

bool dimmer_getstate()
{
  return dimmer_state;
}

void dimmer_init(uint8_t zerocrosspin, uint8_t triacpin, bool fade = true)
{
  dimmer_zerocrosspin = zerocrosspin;
  dimmer_triacpin = triacpin;
  dimmer_fade = fade;
  //  system_update_cpu_freq(160); // Minimize flickering of lamp
  pinMode(dimmer_zerocrosspin, INPUT_PULLUP);
  digitalWrite(dimmer_triacpin, 0);
  pinMode(dimmer_triacpin, OUTPUT);
  hw_timer_init(NMI_SOURCE, 0);
  hw_timer_set_func(dimmer_dimTimerISR);
  attachInterrupt(dimmer_zerocrosspin, dimmer_zcDetectISR, RISING);
  dimmer_setdimvalue(0);
}

void dimmer_stop()
{
  detachInterrupt(dimmer_zerocrosspin);
  hw_timer_set_func(0);
  hw_timer_deinit();
  digitalWrite(dimmer_triacpin, 0);
}

void dimmer_zcDetectISR()
{
  digitalWrite(dimmer_triacpin, 0);
  if ((dimmer_curBrightness > 0) || (dimmer_tarBrightness > 0))
  {
    hw_timer_arm(dimmer_dimDelay);
  }
}


void dimmer_dimTimerISR()
{
  digitalWrite(dimmer_triacpin, 1);

  if (dimmer_fadedelay < 1) dimmer_fadedelay++;
  else
  {
    dimmer_fadedelay = 0;
    if (dimmer_fade == 1) {
      if (dimmer_curBrightness > dimmer_tarBrightness && dimmer_curBrightness > 0) {
        --dimmer_curBrightness;
      }
      else if (dimmer_curBrightness < dimmer_tarBrightness && dimmer_curBrightness < 255 ) {
        if ((dimmer_curBrightness == 0) && (dimmer_tarBrightness > 15)) dimmer_curBrightness = 15;
        ++dimmer_curBrightness;
      }
    }
  }

  if (dimmer_fade == 0)
  {
    dimmer_curBrightness = dimmer_tarBrightness;
  }

  dimmer_dimDelay = (30 * (255 - dimmer_curBrightness)) + 400;
}
