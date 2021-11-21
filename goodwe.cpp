/*
 * GoodWe Modbus Communication for espMQTT
 * Created by RvdH
 */

#include "espMQTT.h"
#include "goodwe.h"

void(*_goodwe_callback)(const char *, String);

void goodwe_init(void(*callback)(const char *, String))
{
  _goodwe_callback = callback;
  Serial.setRxBufferSize(50);
  Serial.begin(9600);  //Init serial 9600 baud
  Serial.setDebugOutput(false);
  // _goodwe_callback("grid/today/kwh", "0.0");
  // _goodwe_callback("grid/today/kwh", "-");
  // _goodwe_callback("grid/total/kwh", "-");
  // _goodwe_callback("grid/total/hour", "-");
}

void goodwe_send_command(uint8_t mbAddr)
{
  DEBUG_V("Requesting goodwe Data From %02x...\n", mbAddr);
  uint8_t TxBuffer[10];

  TxBuffer[0] = mbAddr; // adres
  TxBuffer[1] = 0x3; // type (03 holding register)
  TxBuffer[2] = 0x3; // H startaddress
  TxBuffer[3] = 0x0d; // L startaddress
  TxBuffer[4] = 0x0; // H number of addresses
  TxBuffer[5] = 0x14; // L number of addresses

  //Calc the raw_msg_data_byte CRC code
  uint16_t crc = 0xFFFF;
  String crc_string = "";
  for (int pos = 0; pos < 6; pos++) {
    crc ^= (uint16_t)TxBuffer[pos];   // XOR byte into least sig. byte of crc
    for (int i = 8; i != 0; i--) {    // Loop over each bit
      if ((crc & 0x0001) != 0) {      // If the LSB is set
        crc >>= 1;                    // Shift right and XOR 0xA001
        crc ^= 0xA001;
      }
      else                            // Else LSB is not set
        crc >>= 1;                    // Just shift right
    }
  }

  TxBuffer[6] = crc & 0xFF;
  TxBuffer[7] = crc >> 8;

  for (int i = 0; i < 8; i++)
  {
    DEBUG_V ("Sending to goodwe inverter: 0x%02x\n", TxBuffer[i]);
    Serial.write(TxBuffer[i]);
  }


}

void goodwe_handle()
{
  static long long  nextupdatetime = 0;
  static long long  querytime = 0;
  static uint8_t    RxBuffer[50];
  static uint8_t    RxBufferPointer = 0;
  static bool       waitForResponse = false;
  static bool       dataReceived = false;
  static bool       lh = 0;
  static uint8_t    curAddr = 1;

  if (waitForResponse == false)
  {

    if (nextupdatetime < millis() ) {

      RxBufferPointer = 0;
      lh = 0;
      waitForResponse = true;
      dataReceived    = false;
      querytime       = millis();
      nextupdatetime  = millis() + (GOODWE_POLL_TIMER * 1000);

      if (curAddr == 2) {
        curAddr = 1;
        goodwe_send_command(0x01);

      } 
      else 
      {
        curAddr = 2;
        goodwe_send_command(0x02);
      }
    }
  } 
  else 
  {
    if (millis() > querytime + (GOODWE_MODBUS_TIMEOUT * 1000) ) {
      if (dataReceived == false) {
        if (curAddr == 1)
        {
          _goodwe_callback("1/inverterstatus", "offline");
          _goodwe_callback("1/grid/kw", "0.000");
        }
        if (curAddr == 2)
        {
          _goodwe_callback("2/inverterstatus", "offline");
          _goodwe_callback("2/grid/kw", "0.000");
        }
      }
      waitForResponse = false;
    }
  }

  if (Serial.available())
  {
    dataReceived = true;
    if (RxBufferPointer < 50) 
    {
      RxBuffer[RxBufferPointer] = Serial.read();
      lh = !lh;

      DEBUG_V ("Received from goodwe inverter[addr %i] (pointer: %i) (lh: %i): 0x%02x\n", RxBuffer[0], RxBufferPointer, lh, RxBuffer[RxBufferPointer]);

      if (lh == 1) DEBUG_V ("value: %f \n", double((uint16_t(RxBuffer[RxBufferPointer - 1]) << 8) + RxBuffer[RxBufferPointer]) );

      if (RxBuffer[0] != curAddr) 
      {
        RxBufferPointer = 0;
        lh = 0;
        DEBUG_V ("RxBuf0 != curAddr");
      }

      if (RxBufferPointer == 1)
      {
        if (RxBuffer[1] != 0x03)  {
          RxBufferPointer = 0;
          lh = 0;
        }
      }
      RxBufferPointer++;
    } 
    else
    {
      DEBUG_E("Serial Buffer Overflow!!\n");
      RxBufferPointer = 0;
    }


    if (RxBuffer[0] == curAddr && RxBuffer[1] == 0x03)
    {
      if (RxBufferPointer == RxBuffer[2] + 5) // full message received
      {
        double value = 0;
        uint32_t intvalue = 0;
        DEBUG_D("Received complete message from goodwe Inverter...\n");

        uint16_t inverterstatus = (RxBuffer[5] << 8) + RxBuffer[6];
        // _goodwe_callback((String)RxBuffer[0]+"/inverterstatus",String(inverterstatus) );

        if (RxBuffer[0] == 1)
        {
          _goodwe_callback("1/grid/kw", String(double((uint16_t(RxBuffer[3]) << 8) + RxBuffer[4]) / 1000.0, 3));
          _goodwe_callback("1/inverterstatus", inverterstatus ? "working" : "waiting");
          if (RxBufferPointer >= 20)  _goodwe_callback("1/grid/total/kwh", String(double((uint16_t(RxBuffer[15]) << 8) + RxBuffer[16]) / 10.0, 1));
          if (RxBufferPointer >= 46)  _goodwe_callback("1/grid/today/kwh", String(double((uint16_t(RxBuffer[41]) << 8) + RxBuffer[42]) / 10.0, 1));
        }
        
        if (RxBuffer[0] == 2)
        {
          _goodwe_callback("2/grid/kw", String(double((uint16_t(RxBuffer[3]) << 8) + RxBuffer[4]) / 1000.0, 3));
          _goodwe_callback("2/inverterstatus", inverterstatus ? "working" : "waiting");
          if (RxBufferPointer >= 20)  _goodwe_callback("2/grid/total/kwh", String(double((uint16_t(RxBuffer[15]) << 8) + RxBuffer[16]) / 10.0, 1));
          if (RxBufferPointer >= 46)  _goodwe_callback("2/grid/today/kwh", String(double((uint16_t(RxBuffer[41]) << 8) + RxBuffer[42]) / 10.0, 1));
        }
        
        waitForResponse = false;
      }
    }
  }
}
