#include <ESP8266WiFi.h>
#include "espMQTT.h"
#ifndef D0
#define D0 0
#endif
#define NODEMCULEDPIN D0

static String amgpowercmd = "";
static String amgtempcmd = "";
static uint8_t amgcmdnr = 0;
void(*_amgpelletstove_callback)(String, String);

void amgpelletstove_init(void(*callback)(String, String))
{
  _amgpelletstove_callback = callback;
  Serial.setDebugOutput(false);
  Serial.begin(115200, SERIAL_8N1);
  U0C0 = BIT(UCRXI) | BIT(UCBN) | BIT(UCBN + 1) | BIT(UCSBN) | BIT(UCTXI); // Inverse RX + TX
}

void amgpelletstove_receivemqtt(String topicstring, String payloadstring)
{
  if (topicstring == String("home/" + WiFi.hostname() + "/setpower"))
  {
    int powersetpoint = payloadstring.toInt();
    if ((payloadstring == "0") || ((powersetpoint > 0) && (powersetpoint <= 5)))
    {
      switch (powersetpoint)
      {
        case 0: amgpowercmd = "RF000058&"; break;
        case 1: amgpowercmd = "RF001059&"; break;
        case 2: amgpowercmd = "RF00205A&"; break;
        case 3: amgpowercmd = "RF00305B&"; break;
        case 4: amgpowercmd = "RF00405C&"; break;
        case 5: amgpowercmd = "RF00505D&"; break;
      }
      _amgpelletstove_callback("power/setpoint", String(payloadstring));
    }
  }
  if (topicstring == String("home/" + WiFi.hostname() + "/settemperature"))
  {
    if ((payloadstring.toInt() >= 16) && (payloadstring.toInt() <= 25)) // No wider range supported for now..
    {
      char str[9];
      sprintf(str, "%02X", (uint16_t)payloadstring.toInt());
      String temp = str;
      sprintf(str, "%02X", (uint16_t)(payloadstring.toInt() + 75));
      String checksum = str;
      amgtempcmd = "RF2" + temp + "0" + checksum + "&";
      _amgpelletstove_callback("room/temperature/setpoint", String(payloadstring.toInt()));
    }
  }
}

void _amgpelletstove_sendserial()
{
  amgcmdnr++;
  if (amgpowercmd != "") amgcmdnr = 253;
  else if (amgtempcmd != "") amgcmdnr = 254;
  digitalWrite(NODEMCULEDPIN, 0);
  Serial.write (27);
  static String sendcmd; // needs to be static otherwise debug call fails..
  sendcmd = "";
  switch (amgcmdnr)
  {
    case 1:
      sendcmd = "RD100057&"; // Request room temperature
      break;
    case 2:
      sendcmd = "RC60005B&"; // Request room temperature setpoint
      break;
    case 3:
      sendcmd = "RC50005A&"; // Request power setpoint
      break;
    case 4:
      sendcmd = "RD000056&"; // Request exhaust temperature
      break;
    case 5:
      sendcmd = "RD200058&"; // Request exhaust fanspeed
      break;
    case 6:
      sendcmd = "RD300059&"; // Request room fanspeed
      break;
    case 7:
      sendcmd = "RD40005A&"; // Request auger speed
      break;
    case 8:
      sendcmd = "RDF0006C&"; // Request board temp
      break;
    case 9:
      sendcmd = "RDD0006A&"; // ??? 0000020 Fluegas motor correction (rpm*10)
      break;
    case 10:
      sendcmd = "RDE0006B&"; // ??? 0000020 Pellet correction (%)
      break;
    case 11:
      sendcmd = "RDA00067&"; // 000020 // error code
      break;
    case 12:
      sendcmd = "REF0006D&"; // 00D20036  // FANSPEED EXHAUST
      break;
    case 13:
      sendcmd = "RD80005E&"; // TIMER TO NEXT PHASE
      break;
    case 14:
      sendcmd = "RC000055&"; // 00000020 // Inputs
      break;
    case 15:
      sendcmd = "RDB00068&"; // Extractor sensorlevel
      break;
    case 253:
      DEBUG_V("Writing to amg pelletstove:%s\n",amgpowercmd.c_str());
      sendcmd = amgpowercmd;
      amgpowercmd = "";
      amgcmdnr = 255;
      break;
    case 254:
      DEBUG_V("Writing to amg pelletstove:%s\n", amgtempcmd.c_str());
      sendcmd = amgtempcmd;
      amgtempcmd = "";
      amgcmdnr = 255;
      break;
    default:
      amgcmdnr = 0;
      sendcmd = "RD90005F&"; // 02010023 // cycle phase
      break;
  }
  Serial.print(sendcmd);
  DEBUG_V("Writing to amg pelletstove:%s\n",sendcmd.c_str());
}

void amgpelletstove_handle()
{
  static unsigned long nextupdatetime = millis() + 1000; // Wait 1 second after startup before sending first cmd...
  static String serstr = "";
  static uint8_t currentpower = 0;
  static bool commok = 1;
  if (Serial.available() > 0)
  {
    int serval = Serial.read();
    if (serval != 27) serstr += String(char(serval));
    else serstr += "!"; // convert escape character to !
    if (serval == '&')
    {
      digitalWrite(NODEMCULEDPIN, 1);
      DEBUG_V("Received from amg pelletstove:%s\n",serstr.c_str());
      long value = strtol(serstr.substring(2, 5).c_str(), NULL, 16);
      serstr = "";
      switch (amgcmdnr)
      {
        case 0:
          switch (value)
          {
            case 0:
              currentpower = 0;
              _amgpelletstove_callback("phase", "off");
              break;
            case 258:
              _amgpelletstove_callback("phase", "load wood phase 1");
              break;
            case 259:
              _amgpelletstove_callback("phase", "load wood phase 2");
              break;
            case 260:
              _amgpelletstove_callback("phase", "fire on");
              break;
            case 513:
              _amgpelletstove_callback("phase", "heating");
              break;
            case 1025:
              _amgpelletstove_callback("phase", "burn pot cleaning");
              break;
            case 2049:
              currentpower = 0;
              _amgpelletstove_callback("phase", "cooling down phase 1");
              break;
            case 2050:
              currentpower = 0;
              _amgpelletstove_callback("phase", "cooling down phase 2");
              break;
            default:
              _amgpelletstove_callback("phase", "unknown");
              currentpower = 0;
              break;
          }
          _amgpelletstove_callback("phase/value", String(value));
          _amgpelletstove_callback("power/value", String(currentpower));
          break;
        case 1:
          _amgpelletstove_callback("room/temperature/measured", String(float(value) / 10, 1));
          break;
        case 2:
          _amgpelletstove_callback("room/temperature/setpoint", String(value));
          break;
        case 3:
          _amgpelletstove_callback("controlpanel/power/setpoint", String(value));
          break;
        case 4:
          _amgpelletstove_callback("exhaust/temperature", String(value));
          break;
        case 5:
          _amgpelletstove_callback("exhaust/fanspeed", String(value));
          break;
        case 6:
          _amgpelletstove_callback("room/fanspeed", String(value));
          currentpower = value;
          break;
        case 7:
          _amgpelletstove_callback("auger/speed", String(value));
          if ((value > 0) && (currentpower > value)) currentpower = value;
          break;
        case 8:
          _amgpelletstove_callback("board/temperature", String(value));
          break;
        case 9:
          _amgpelletstove_callback("exhaust/fanspeed/correction", String(value * 10));
          break;
        case 10:
          _amgpelletstove_callback("auger/correction", String(value));
          break;
        case 11:
          _amgpelletstove_callback("board/errorcode", String(value));
          break;
        case 12:
          _amgpelletstove_callback("exhaust/fanspeed/measured", String(value * 10));
          break;
        case 13:
          _amgpelletstove_callback("phase/timer", String(value));
          break;
        case 14:
          _amgpelletstove_callback("board/inputs", String(value));
          break;
        case 15:
          _amgpelletstove_callback("extractorsensorlevel", String(value));
          _amgpelletstove_callback("status", "ready");
          break;
      }
      serstr = "";
      nextupdatetime = millis() + 500; // After successfull transfer wait 500ms before requesting next value
      commok = 1;
    }
  }

  if (millis() > nextupdatetime)
  {
    if (!commok) _amgpelletstove_callback("status", "commerror");
    commok = 0;
    nextupdatetime = millis() + 5000; // On first or failed transefer wait 5 second before next (re)try
    _amgpelletstove_sendserial();
  }
}
