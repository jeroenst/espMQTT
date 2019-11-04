#include <ESP8266WiFi.h>
#include "espMQTT.h"
#include "growatt.h"

#define _GROWATT_FAN_PID_P_GAIN 200
#define _GROWATT_FAN_PID_I_GAIN 20
#define _GROWATT_FAN_PID_D_GAIN 0
#define _GROWATT_FAN_STOP_TEMP_DELTA -5
#define _GROWAT_PWM_RANGE (PWMRANGE-GROWATT_FANSPEED_OFFSET)
#define _GROWATT_FAN_PWM_FREQ 32

void(*_growatt_callback)(String,String);
static int _growatt_fanpin = 0;
static int _growatt_fanspeed = 0;
static float _growatt_fan_pid_p = 0;
static float _growatt_fan_pid_i = 0;
static float _growatt_fan_pid_d = 0;
static float _growatt_fan_pid_error = 0;
static float _growatt_fan_pid_preverror = 0;


void growatt_init(void(*callback)(String,String), int fanpin)
{
  _growatt_callback = callback;
  Serial.setRxBufferSize(2048); 
  Serial.begin(9600);  //Init serial 9600 baud
  Serial.setDebugOutput(false);
  _growatt_callback("grid/today/kwh", "0.0");
  if (fanpin >= 0)
  {
    _growatt_fanpin = fanpin;
    pinMode(fanpin, OUTPUT);
    analogWrite(fanpin, 0);
    analogWriteFreq(_GROWATT_FAN_PWM_FREQ);
    _growatt_fan_pid_p = 0;
    _growatt_fan_pid_i = 0;
    _growatt_fan_pid_d = 0;
  }

  _growatt_callback("grid/today/kwh", "-");
  _growatt_callback("grid/total/kwh", "-");
  _growatt_callback("grid/total/hour", "-");
}

void growatt_send_command(uint8_t c1)
{
  DEBUG("Requesting Growatt Data %#02x...\n");
  uint8_t TxBuffer[10];
  TxBuffer[0] = 0x3F;
  TxBuffer[1] = 0x23;
  TxBuffer[2] = 0x01;
  TxBuffer[3] = 0x32;
  TxBuffer[4] = c1;
  TxBuffer[5] = 0x00;
  uint16_t wStringSum = 0;
  for (int i = 0; i < 6; i++)
  {
    wStringSum = wStringSum + (TxBuffer[i] ^ i);
    if (wStringSum == 0 || wStringSum > 0xFFFF) wStringSum = 0xFFFF;
  }
  TxBuffer[6] = wStringSum >> 8;
  TxBuffer[7] = wStringSum & 0xFF;
  for (int i = 0; i < 8; i++)
  {
    DEBUG ("Sending to Growatt inverter: %#02x\n", TxBuffer[i]);
    Serial.write(TxBuffer[i]);
  }
}

void growatt_handle()
{
  static long long nextupdatetime = 0;
  static uint8_t RxBuffer[50];
  static uint8_t RxBufferPointer = 0;
  static bool RxPowerDataOk = 0;
  static double pv1volt = 0;

  if (millis() > nextupdatetime)
  {
    RxBufferPointer = 0;
    nextupdatetime = millis() + (GROWATT_POLL_TIMER * 1000);

    if (!RxPowerDataOk)
    {
      _growatt_callback("inverterstatus", "offline");
      _growatt_callback("inverterstatus/value", "-");
      _growatt_callback("pv/1/volt", "-");
      _growatt_callback("pv/2/volt", "-");
      _growatt_callback("pv/watt", "-");
      _growatt_callback("grid/volt", "-");
      _growatt_callback("grid/amp", "-");
      _growatt_callback("grid/frequency", "-");
      _growatt_callback("grid/watt", "-");
      _growatt_callback("fault/temperature", "-");
      _growatt_callback("fault/type", "-");
      _growatt_callback("temperature", "-");
      _growatt_callback("status", "commerror");
      if (_growatt_fanpin >= 0)
      {
         _growatt_callback("fanspeed", "0");
         analogWrite(_growatt_fanpin, 0);
      }
    }
    RxPowerDataOk = 0;

    growatt_send_command(0x41);
    _growatt_callback("status", "querying");
  }

  if (Serial.available())
  {
    if (RxBufferPointer < 50)
    {
      RxBuffer[RxBufferPointer] = Serial.read();
//      DEBUG ("Received from Growatt inverter: 0x%02x\n", RxBuffer[RxBufferPointer]);
      if (RxBuffer[0] != 0x23) RxBufferPointer = 0;
      if (RxBufferPointer == 1)
      {
        if (RxBuffer[1] != 0x3F) RxBufferPointer = 0;
      }
      RxBufferPointer++;
    }
    else
    {
//      DEBUG("Serial Buffer Overflow!!\n");
      RxBufferPointer = 0;
    }
    if (RxBufferPointer > 5)
    {
      if (RxBufferPointer > RxBuffer[5] + 7)
      {
        double value = 0;
        uint32_t intvalue = 0;
        DEBUG("Received complete message from Growatt Inverter...\n");
        if ((RxBuffer[3] == 0x32) && (RxBuffer[4] == 0x41) && (RxBufferPointer >= 34))
        {
          DEBUG("Received power data from Growatt Inverter...\n");
          intvalue = RxBuffer[6];
          _growatt_callback("inverterstatus/value", String(intvalue));
          _growatt_callback("inverterstatus", intvalue == 0 ? "waiting" : intvalue == 1 ? "ready" : intvalue == 3 ? "fault" : "unknown");
          value = double((uint16_t(RxBuffer[7]) << 8) + RxBuffer[8]) / 10;
          pv1volt = value;
          _growatt_callback("pv/1/volt", String(value, 1));
          value = double((uint16_t(RxBuffer[9]) << 8) + RxBuffer[10]) / 10;
          _growatt_callback("pv/2/volt", String(value, 1));
          value = double((uint16_t(RxBuffer[11]) << 8) + RxBuffer[12]) / 10;
          _growatt_callback("pv/watt", String(value, 1));
          value = double((uint16_t(RxBuffer[13]) << 8) + RxBuffer[14]) / 10;
          _growatt_callback("grid/volt", String(value, 1));
          value = double((uint16_t(RxBuffer[15]) << 8) + RxBuffer[16]) / 10;
          _growatt_callback("grid/amp", String(value, 1));
          value = double((uint16_t(RxBuffer[17]) << 8) + RxBuffer[18]) / 100;
          _growatt_callback("grid/frequency", String(value, 1));
          value = double((uint16_t(RxBuffer[19]) << 8) + RxBuffer[20]) / 10;
          _growatt_callback("grid/watt", String(value, 1));
          value = double((uint16_t(RxBuffer[33]) << 8) + RxBuffer[34]) / 10;
          _growatt_callback("fault/temperature", String(value, 1));
          intvalue = double((uint16_t(RxBuffer[35]) << 8) + RxBuffer[36]);
          _growatt_callback("fault/type", String(intvalue));
          value = double((uint16_t(RxBuffer[37]) << 8) + RxBuffer[38]) / 10;
          _growatt_callback("temperature", String(value, 1));
          if (_growatt_fanpin >= 0)
          {
            //if (value >= GROWATT_FANSPEED_MINTEMP) digitalWrite(_growatt_fanpin,1);
            //else digitalWrite(_growatt_fanpin, 0);

            // PID control of fans on top of growatt inverter
            _growatt_fan_pid_error = value - GROWATT_FANSPEED_TEMP;
            _growatt_fan_pid_preverror = _growatt_fan_pid_error;
            _growatt_fan_pid_p = _growatt_fan_pid_error * _GROWATT_FAN_PID_P_GAIN;
            _growatt_fan_pid_i = max(min(float(_GROWAT_PWM_RANGE), float(_growatt_fan_pid_i + (_growatt_fan_pid_error * _GROWATT_FAN_PID_I_GAIN))), float(0));
            _growatt_fan_pid_d = (_growatt_fan_pid_preverror - _growatt_fan_pid_error) * _GROWATT_FAN_PID_D_GAIN;
            _growatt_fanspeed = max(min(PWMRANGE,int(_growatt_fan_pid_p+_growatt_fan_pid_i+_growatt_fan_pid_d+GROWATT_FANSPEED_OFFSET)),GROWATT_FANSPEED_OFFSET);
            static bool fanstop = 1;
            if (_growatt_fan_pid_error <= _GROWATT_FAN_STOP_TEMP_DELTA) fanstop = 1;
            if (_growatt_fan_pid_error >= 0) fanstop = 0;
            if (fanstop == 1) _growatt_fanspeed = 0;
            analogWrite(_growatt_fanpin, _growatt_fanspeed);
            _growatt_callback("fanspeed", String((100 * _growatt_fanspeed)/PWMRANGE));
            DEBUG("Temperature=%.01f, Fanspeed=%d\n", value, _growatt_fanspeed);

            
            //int fanspeed = 0;
           
            //fanspeed = max(min((int)((value - GROWATT_FANSPEED_MINTEMP) * GROWATT_FANSPEED_MULTIPLIER) + GROWATT_FANSPEED_OFFSET, PWMRANGE), GROWATT_FANSPEED_OFFSET);
            //analogWrite(_growatt_fanpin, fanspeed);
            //DEBUG("Temperature=%.01f, Fanspeed=%d\n", value, fanspeed);
            //_growatt_callback("fan/speed", String((fanspeed*100)/PWMRANGE));
            //_growatt_callback("fan/on", String(digitalRead(_growatt_fanpin)));
          }
          
          RxPowerDataOk = 1;
          growatt_send_command(0x42);
        }
        if ((RxBuffer[3] == 0x32) && (RxBuffer[4] == 0x42) && (RxBufferPointer >= 22))
        {
          DEBUG("Received energy data from Growatt Inverter...\n");
          value = double((uint16_t(RxBuffer[13]) << 8) + RxBuffer[14]) / 10;
          if (pv1volt > 100) _growatt_callback("grid/today/kwh", String(value, 1)); // Only reset today value when pv 1 volt is above 100 volt (steady voltage) otherwise this gets resets during shutdown
          value = double((uint32_t(RxBuffer[15]) << 24) + (uint32_t(RxBuffer[16]) << 16) + (uint16_t(RxBuffer[17]) << 8) + RxBuffer[18]) / 10;
          _growatt_callback("grid/total/kwh", String(value, 1));
          intvalue = ((uint32_t(RxBuffer[19]) << 24) + (uint32_t(RxBuffer[20]) << 16) + (uint16_t(RxBuffer[21]) << 8) + RxBuffer[22]);
          _growatt_callback("grid/total/hour", String(intvalue));
          _growatt_callback("status", "ready");
        }
        RxBufferPointer = 0;
      }
    }
  }
}
