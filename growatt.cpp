#include "espMQTT.h"
#include "growatt.h"

#define _GROWATT_FAN_PID_P_GAIN 20
#define _GROWATT_FAN_PID_I_GAIN 2
#define _GROWATT_FAN_PID_D_GAIN 0
#define _GROWATT_FAN_STOP_TEMP_DELTA -50
#define _GROWAT_PWM_RANGE (PWMRANGE-GROWATT_FANSPEED_OFFSET)
#define _GROWATT_FAN_PWM_FREQ 32

void(*_growatt_callback)();
static int _growatt_fanpin = 0;
static int _growatt_fanspeed = 0;
static float _growatt_fan_pid_p = 0;
static float _growatt_fan_pid_i = 0;
static float _growatt_fan_pid_d = 0;
static float _growatt_fan_pid_error = 0;
static float _growatt_fan_pid_preverror = 0;

Growatt_DataMap_Struct growatt_DataMap;

void growatt_init(void(*callback)(), int fanpin)
{
  _growatt_callback = callback;
  Serial.setRxBufferSize(100); 
  Serial.begin(9600);  //Init serial 9600 baud
  Serial.setDebugOutput(false);
  //_growatt_callback("grid/today/kwh", "0.0");
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

}

void growatt_send_command(uint8_t c1)
{
  DEBUG_V("Requesting Growatt Data %02x...\n", c1);
  uint8_t TxBuffer[10];
  TxBuffer[0] = 0x3F;
  TxBuffer[1] = 0x23;
  TxBuffer[2] = 0x01;
  TxBuffer[3] = 0x32;
  TxBuffer[4] = c1;
  TxBuffer[5] = 0x00;
  uint32_t wStringSum = 0;
  for (int i = 0; i < 6; i++)
  {
    wStringSum = wStringSum + (TxBuffer[i] ^ i);
    if (wStringSum == 0 || wStringSum > 0xFFFF) wStringSum = 0xFFFF;
  }
  TxBuffer[6] = wStringSum >> 8;
  TxBuffer[7] = wStringSum & 0xFF;
  for (int i = 0; i < 8; i++)
  {
    DEBUG_V ("Sending to Growatt inverter: %#02x\n", TxBuffer[i]);
    Serial.write(TxBuffer[i]);
  }
}

void growatt_handle()
{
  static long long nextupdatetime = 0;
  static uint8_t RxBuffer[50];
  static uint8_t RxBufferPointer = 0;
  static bool RxPowerDataOk = 0;
  
  if (millis() > nextupdatetime)
  {
    RxBufferPointer = 0;
    nextupdatetime = millis() + (GROWATT_POLL_TIMER * 1000);

    if (!RxPowerDataOk)
    {
      growatt_DataMap.status = Growatt_status::offline;
      growatt_DataMap.dataReady = false;
      _growatt_callback();
    }
    else 
    {
      growatt_DataMap.status = Growatt_status::querying;
      _growatt_callback();
    }
    RxPowerDataOk = 0;
    growatt_send_command(0x41);
  }

  while (Serial.available())
  {
    if (RxBufferPointer < 50)
    {
      RxBuffer[RxBufferPointer] = Serial.read();
      DEBUG_V ("Received from Growatt inverter: 0x%02x\n", RxBuffer[RxBufferPointer]);
      if (RxBuffer[0] != 0x23) RxBufferPointer = 0;
      if (RxBufferPointer == 1)
      {
        if (RxBuffer[1] != 0x3F) RxBufferPointer = 0;
      }
      RxBufferPointer++;
    }
    else
    {
      DEBUG_E("Serial Buffer Overflow!!\n");
      RxBufferPointer = 0;
    }
    if (RxBufferPointer > 5)
    {
      if (RxBufferPointer > RxBuffer[5] + 7)
      {
//        double value = 0;
        uint32_t uintvalue = 0;
        DEBUG_D("Received complete message from Growatt Inverter...\n");
        if ((RxBuffer[3] == 0x32) && (RxBuffer[4] == 0x41) && (RxBufferPointer >= 34))
        {
          DEBUG_D("Received power data from Growatt Inverter...\n");
          
          uintvalue = RxBuffer[6];
          growatt_DataMap.changed.inverter_status_value = uintvalue != growatt_DataMap.inverter_status_value;
          growatt_DataMap.inverter_status_value = uintvalue;

          growatt_DataMap.changed.inverter_status = uintvalue != growatt_DataMap.inverter_status_value;
          growatt_DataMap.inverter_status = (uintvalue == 0 ? "waiting" : uintvalue == 1 ? "ready" : uintvalue == 3 ? "fault" : "unknown");
          
          uintvalue = (uint16_t(RxBuffer[7]) << 8) + RxBuffer[8];
          growatt_DataMap.changed.pv_1_voltage = uintvalue != growatt_DataMap.pv_1_voltage;
          growatt_DataMap.pv_1_voltage = uintvalue;

          uintvalue = (uint16_t(RxBuffer[9]) << 8) + RxBuffer[10];
          growatt_DataMap.changed.pv_2_voltage = uintvalue != growatt_DataMap.pv_2_voltage;
          growatt_DataMap.pv_2_voltage = uintvalue;

          uintvalue = (uint16_t(RxBuffer[11]) << 8) + RxBuffer[12];
          growatt_DataMap.changed.pv_power = uintvalue != growatt_DataMap.pv_power;
          growatt_DataMap.pv_power = uintvalue;


          uintvalue = (uint16_t(RxBuffer[13]) << 8) + RxBuffer[14];
          growatt_DataMap.changed.grid_voltage = uintvalue != growatt_DataMap.grid_voltage;
          growatt_DataMap.grid_voltage = uintvalue;

          uintvalue = (uint16_t(RxBuffer[15]) << 8) + RxBuffer[16];
          growatt_DataMap.changed.grid_current = uintvalue != growatt_DataMap.grid_current;
          growatt_DataMap.grid_current = uintvalue;

          uintvalue = (uint16_t(RxBuffer[17]) << 8) + RxBuffer[18];
          growatt_DataMap.changed.grid_frequency = uintvalue != growatt_DataMap.grid_frequency;
          growatt_DataMap.grid_frequency = uintvalue;

          uintvalue = (uint16_t(RxBuffer[19]) << 8) + RxBuffer[20];
          growatt_DataMap.changed.grid_power = (uintvalue != growatt_DataMap.grid_power);
          growatt_DataMap.grid_power = uintvalue;

          uintvalue = (uint16_t(RxBuffer[33]) << 8) + RxBuffer[34];
          growatt_DataMap.changed.fault_temperature = uintvalue != growatt_DataMap.fault_temperature;
          growatt_DataMap.fault_temperature = uintvalue;

          uintvalue = (uint16_t(RxBuffer[35]) << 8) + RxBuffer[36];
          growatt_DataMap.changed.fault_type = uintvalue != growatt_DataMap.fault_type;
          growatt_DataMap.fault_type = uintvalue;
          
          uintvalue = (uint16_t(RxBuffer[37]) << 8) + RxBuffer[38];
          growatt_DataMap.changed.temperature = uintvalue != growatt_DataMap.temperature;
          growatt_DataMap.temperature = uintvalue;
          
          if (_growatt_fanpin >= 0)
          {
            //if (value >= GROWATT_FANSPEED_MINTEMP) digitalWrite(_growatt_fanpin,1);
            //else digitalWrite(_growatt_fanpin, 0);

            // PID control of fans on top of growatt inverter
            _growatt_fan_pid_error = growatt_DataMap.temperature - GROWATT_FANSPEED_TEMP;
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
            
            uintvalue = (100 * _growatt_fanspeed)/PWMRANGE;
            growatt_DataMap.changed.fanspeed = uintvalue != growatt_DataMap.fanspeed;
            growatt_DataMap.fanspeed = uintvalue;

            DEBUG_D("Temperature=%d%01d, Fanspeed=%d\n", uintvalue/10, uintvalue%10, _growatt_fanspeed);
          }
          RxPowerDataOk = 1;
          growatt_send_command(0x42);
        }
        if ((RxBuffer[3] == 0x32) && (RxBuffer[4] == 0x42) && (RxBufferPointer >= 22))
        {
          DEBUG_D("Received energy data from Growatt Inverter...\n");
          if (growatt_DataMap.pv_power > 10)  // Only reset today value when pvwatt above 1 watt otherwise this gets resets during shutdown
          {
            uintvalue = (uint16_t(RxBuffer[13]) << 8) + RxBuffer[14];
            growatt_DataMap.changed.grid_today_energy = uintvalue != growatt_DataMap.grid_today_energy;
            growatt_DataMap.grid_today_energy = uintvalue;
          }

          uintvalue = (uint32_t(RxBuffer[15]) << 24) + (uint32_t(RxBuffer[16]) << 16) + (uint16_t(RxBuffer[17]) << 8) + RxBuffer[18];
          growatt_DataMap.changed.grid_total_energy = uintvalue != growatt_DataMap.grid_total_energy;
          growatt_DataMap.grid_total_energy = uintvalue;

          uintvalue = ((uint32_t(RxBuffer[19]) << 24) + (uint32_t(RxBuffer[20]) << 16) + (uint16_t(RxBuffer[21]) << 8) + RxBuffer[22]);
          growatt_DataMap.changed.inverter_time = uintvalue != growatt_DataMap.inverter_time;
          growatt_DataMap.inverter_time = uintvalue;

          
          growatt_DataMap.status = Growatt_status::ready;
          growatt_DataMap.dataReady = true;
          _growatt_callback();
        }
        RxBufferPointer = 0;
      }
    }
  }
}
