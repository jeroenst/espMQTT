#include "espMQTT.h"
#ifndef D0
#define D0 0
#endif
#define NODEMCULEDPIN D0

String amgpowercmd = "";
String amgtempcmd = "";
uint8_t amgcmdnr = 0;
bool cooldown_phase_1 = false;

void amgpelletstove_init()
{
  Serial.setDebugOutput(false);
  Serial.begin(115200, SERIAL_8N1);
  U0C0 = BIT(UCRXI) | BIT(UCBN) | BIT(UCBN + 1) | BIT(UCSBN) | BIT(UCTXI); // Inverse RX + TX
}

void amgpelletstove_receivemqtt(const String& topicstring, const String& payloadstring)
{
  if (topicstring.substring(topicstring.length() - 9) == sF("/setpower"))
  {
    int powersetpoint = payloadstring.toInt();
    if ((payloadstring == "0") || ((powersetpoint > 0) && (powersetpoint <= 5)))
    {
      if (!cooldown_phase_1)
      {
        switch (powersetpoint)
        {
          case 0: amgpowercmd = sF("RF000058&"); break;
          case 1: amgpowercmd = sF("RF001059&"); break;
          case 2: amgpowercmd = sF("RF00205A&"); break;
          case 3: amgpowercmd = sF("RF00305B&"); break;
          case 4: amgpowercmd = sF("RF00405C&"); break;
          case 5: amgpowercmd = sF("RF00505D&"); break;
        }
        putdatamap(cF("power/setpoint"), String(payloadstring));
      }
      else
      {
        putdatamap(cF("power/setpoint"), "0");
      }
    }
  }
  if (topicstring.substring(topicstring.length() - 15) == sF("/settemperature"))
  {
    if ((payloadstring.toInt() >= 16) && (payloadstring.toInt() <= 25)) // No wider range supported for now..
    {
      char str[9];
      sprintf(str, "%02X", (uint16_t)payloadstring.toInt());
      String temp = str;
      sprintf(str, "%02X", (uint16_t)(payloadstring.toInt() + 75));
      String checksum = str;
      amgtempcmd = "RF2" + temp + "0" + checksum + "&";
      putdatamap(cF("room/temperature/setpoint"), String(payloadstring.toInt()));
    } 
  }
}

void _amgpelletstove_sendserial()
{
  static bool old_cooldown_phase_1 = 0;

  amgcmdnr++;
  if (cooldown_phase_1) 
  {
    amgpowercmd = ""; // Prevent remote power setting during cooldown phase 1
    if (old_cooldown_phase_1 != cooldown_phase_1) amgcmdnr = 252; // Prevent restarting of stove due race condition
  }
  else if (amgpowercmd != "") amgcmdnr = 253;
  else if (amgtempcmd != "") amgcmdnr = 254;
  old_cooldown_phase_1 = cooldown_phase_1;

  digitalWrite(NODEMCULEDPIN, 0);
  Serial.write (27);
  static String sendcmd; // needs to be static otherwise debug call fails..
  sendcmd = "";
  switch (amgcmdnr)
  {
    case 1:
      sendcmd = sF("RD100057&"); // Request room temperature
      break;
    case 2:
      sendcmd = sF("RC60005B&"); // Request room temperature setpoint
      break;
    case 3:
      sendcmd = sF("RC50005A&"); // Request power setpoint
      break;
    case 4:
      sendcmd = sF("RD000056&"); // Request exhaust temperature
      break;
    case 5:
      sendcmd = sF("RD200058&"); // Request exhaust fanspeed
      break;
    case 6:
      sendcmd = sF("RD300059&"); // Request room fanspeed
      break;
    case 7:
      sendcmd = sF("RD40005A&"); // Request auger speed
      break;
    case 8:
      sendcmd = sF("RDF0006C&"); // Request board temp
      break;
    case 9:
      sendcmd = sF("RDD0006A&"); // ??? 0000020 Fluegas motor correction (rpm*10)
      break;
    case 10:
      sendcmd = sF("RDE0006B&"); // ??? 0000020 Pellet correction (%)
      break;
    case 11:
      sendcmd = sF("RDA00067&"); // 000020 // error code
      break;
    case 12:
      sendcmd = sF("REF0006D&"); // 00D20036  // FANSPEED EXHAUST
      break;
    case 13:
      sendcmd = sF("RD80005E&"); // TIMER TO NEXT PHASE
      break;
    case 14:
      sendcmd = sF("RC000055&"); // 00000020 // Inputs
      break;
    case 15:
      sendcmd = sF("RDB00068&"); // Extractor sensorlevel
      break;
    case 252:
       sendcmd = sF("RF000058&"); // Set power to 0 to prevent unwanted restarting of stove
    break;
    case 253:
      if (Debug.isActive(Debug.VERBOSE)) Debug.printf(cF("Writing to amg pelletstove:%s\n"),amgpowercmd.c_str());
      sendcmd = amgpowercmd;
      amgpowercmd = "";
      amgcmdnr = 255;
      break;
    case 254:
      if (Debug.isActive(Debug.VERBOSE)) Debug.printf(cF("Writing to amg pelletstove:%s\n"), amgtempcmd.c_str());
      sendcmd = amgtempcmd;
      amgtempcmd = "";
      amgcmdnr = 255;
      break;
    default:
      amgcmdnr = 0;
      sendcmd = sF("RD90005F&"); // 02010023 // cycle phase
      break;
  }
  Serial.print(sendcmd);
  if (Debug.isActive(Debug.VERBOSE)) Debug.printf(cF("Writing to amg pelletstove:%s\n"),sendcmd.c_str());
}

void amgpelletstove_handle()
{
  static unsigned long nextupdatetime = millis() + 1000; // Wait 1 second after startup before sending first cmd...
  static String serstr = "";
  static int8_t currentpower = -1;
  static bool commok = 1;
  while (Serial.available() > 0)
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
          cooldown_phase_1 = false;
          switch (value)
          {
            case 0:
              currentpower = 0;
              putdatamap(cF("phase"), "off");
              break;
            case 257:
              putdatamap(cF("phase"), "heating ignitor");
              break;
            case 258:
              putdatamap(cF("phase"), "load wood phase 1");
              break;
            case 259:
              putdatamap(cF("phase"), "load wood phase 2");
              break;
            case 260:
              putdatamap(cF("phase"), "fire on");
              break;
            case 513:
              putdatamap(cF("phase"), "heating");
              break;
            case 1025:
              putdatamap(cF("phase"), "burn pot cleaning");
              break;
            case 2049:
              currentpower = 0;
              putdatamap(cF("phase"), "cooling down phase 1");
              cooldown_phase_1 = true;
              break;
            case 2050:
              currentpower = 0;
              putdatamap(cF("phase"), "cooling down phase 2");
              break;
            default:
              putdatamap(cF("phase"), "unknown ("+String(amgcmdnr)+")");
              break;
          }
          putdatamap(cF("phase/value"), String(value));
          if (currentpower >= 0) putdatamap(cF("power/value"), String(currentpower));
          break;
        case 1:
          putdatamap(cF("room/temperature/measured"), String(float(value) / 10, 1));
          break;
        case 2:
          putdatamap(cF("room/temperature/setpoint"), String(value));
          break;
        case 3:
          putdatamap(cF("controlpanel/power/setpoint"), String(value));
          break;
        case 4:
          putdatamap(cF("exhaust/temperature"), String(value));
          break;
        case 5:
          putdatamap(cF("exhaust/fanspeed"), String(value));
          break;
        case 6:
          putdatamap(cF("room/fanspeed"), String(value));
          currentpower = value;
          break;
        case 7:
          putdatamap(cF("auger/speed"), String(value));
          if ((value > 0) && (currentpower > value)) currentpower = value;
          break;
        case 8:
          putdatamap(cF("board/temperature"), String(value));
          break;
        case 9:
          putdatamap(cF("exhaust/fanspeed/correction"), String(value * 10));
          break;
        case 10:
          putdatamap(cF("auger/correction"), String(value));
          break;
        case 11:
          putdatamap(cF("board/errorcode"), String(value));
          break;
        case 12:
          putdatamap(cF("exhaust/fanspeed/measured"), String(value * 10));
          break;
        case 13:
          putdatamap(cF("phase/timer"), String(value));
          break;
        case 14:
          putdatamap(cF("board/inputs"), String(value));
          break;
        case 15:
          putdatamap(cF("extractorsensorlevel"), String(value));
          putdatamap(cF("status"), "ready");
          break;
      }
      serstr = "";
      nextupdatetime = millis() + 500; // After successfull transfer wait 500ms before requesting next value
      commok = 1;
    }
  }

  if (millis() > nextupdatetime)
  {
    if (!commok) putdatamap(cF("status"), sF("commerror"));
    commok = 0;
    nextupdatetime = millis() + 5000; // On first or failed transefer wait 5 second before next (re)try
    _amgpelletstove_sendserial();
  }
}
