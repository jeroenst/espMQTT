#include <ESP8266WiFi.h>
#include <RemoteDebug.h>

String amgcmd = "";
uint8_t amgcmdnr = 0;
void(*_amgpelletstove_callback)(String,String);
void(*_amgpelletstove_debug_callback)(String);

void amgpelletstove_init(void(*callback)(String,String),void(*debugcallback)(String))
{
  _amgpelletstove_callback = callback;
  _amgpelletstove_debug_callback = debugcallback;
  Serial.setDebugOutput(false);
  Serial.begin(115200, SERIAL_8N1);
  U0C0 = BIT(UCRXI) | BIT(UCBN) | BIT(UCBN + 1) | BIT(UCSBN) | BIT(UCTXI); // Inverse RX + TX
}

void amgpelletstove_receivemqtt(String topicstring, String payloadstring)
{
  if (topicstring == String("home/" + WiFi.hostname() + "/setpower"))
  {
    if (payloadstring == "0") amgcmd = "RF000058&";
    if (payloadstring == "1") amgcmd = "RF001059&";
    if (payloadstring == "2") amgcmd = "RF00205A&";
    if (payloadstring == "3") amgcmd = "RF00305B&";
    if (payloadstring == "4") amgcmd = "RF00405C&";
    if (payloadstring == "5") amgcmd = "RF00505D&";
  }
  if (topicstring == String("home/" + WiFi.hostname() + "/settemperature"))
  {
    if ((payloadstring.toInt() >= 16) && (payloadstring.toInt() <= 25)) // No wider range supported for now..
    {
      char str[9];
      sprintf(str, "%02X", payloadstring.toInt());
      String temp = str;
      sprintf(str, "%02X", payloadstring.toInt() + 75);
      String checksum = str;
      amgcmd = "RF2" + temp + "0" + checksum + "&";
    }
  }
}

void _amgpelletstove_sendserial()
{
    amgcmdnr++;
    if (amgcmd != "") amgcmdnr = 255;
  //  digitalWrite(NODEMCULEDPIN, 0);
    switch (amgcmdnr)
    {
      case 1:
        Serial.write (27);
        Serial.print ("RD100057&"); // Request room temperature
        break;
      case 2:
        Serial.write (27);
        Serial.print ("RC60005B&"); // Request room temperature setpoint
        break;
      case 3:
        Serial.write (27);
        Serial.print ("RC50005A&"); // Request power setpoint
        break;
      case 4:
        Serial.write (27);
        Serial.print ("RD000056&"); // Request exhaust temperature
        break;
      case 5:
        Serial.write (27);
        Serial.print ("RD200058&"); // Request exhaust fanspeed
        break;
      case 6:
        Serial.write (27);
        Serial.print ("RD300059&"); // Request room fanspeed
        break;
      case 7:
        Serial.write (27);
        Serial.print ("RD40005A&"); // Request auger speed
        break;
      case 8:
        Serial.write (27);
        Serial.print ("RDF0006C&"); // Request board temp
        break;
      case 9:
        Serial.write (27);
        Serial.print ("RDD0006A&"); // ??? 0000020 Fluegas motor correction (rpm*10)
        break;
      case 10:
        Serial.write (27);
        Serial.print ("RDE0006B&"); // ??? 0000020 Pellet correction (%)
        break;
      case 11:
        Serial.write (27);
        Serial.print ("RDA00067&"); // 000020 // error code
        break;
      case 12:
        Serial.write (27);
        Serial.print ("REF0006D&"); // 00D20036  // FANSPEED EXHAUST
        break;
      case 13:
        Serial.write (27);
        Serial.print ("RD80005E&"); // TIMER TO NEXT PHASE
        break;
      case 14:
        Serial.write (27);
        Serial.print ("RC000055&"); // 00000020 // Inputs
        break;
      case 15:
        Serial.write (27);
        Serial.print ("RDB00068&"); // Extractor sensorlevel
        break;
      case 255:
        _amgpelletstove_debug_callback("Writing to amg pelletstove:"+amgcmd+"\n");
        Serial.write (27);
        Serial.print (amgcmd);
        amgcmd = "";
        break;
      default:
        amgcmdnr = 0;
        Serial.write (27);
        Serial.print ("RD90005F&"); // 02010023 // cycle phase
        break;

    }
}

void amgpelletstove_handle()
{
  static unsigned long nextupdatetime = millis();
  static String serstr = "";
  while (Serial.available() > 0)
  {
    int serval = Serial.read();
    if (serval != 27) serstr += String(char(serval));
    else serstr += "!"; // convert escape character to !
    if (!Serial.available())
    {
      if (serval == '&')
      {
//        digitalWrite(NODEMCULEDPIN, 1);
        _amgpelletstove_debug_callback("Received from amg pelletstove:"+serstr+"\n");
        long value = strtol(serstr.substring(2, 5).c_str(), 0, 16);
        serstr="";
        switch (amgcmdnr)
        {
          case 12:
            _amgpelletstove_callback("exhaust/fanspeed/measured", String(value * 10));
            break;
          case 1:
            _amgpelletstove_callback("room/temperature/measured", String(float(value) / 10, 1));
            break;
          case 2:
            _amgpelletstove_callback("room/temperature/setpoint", String(value));
            break;
          case 3:
            _amgpelletstove_callback("power/setpoint", String(value));
            break;
          case 4:
            _amgpelletstove_callback("exhaust/temperature", String(value));
            break;
          case 5:
            _amgpelletstove_callback("exhaust/fanspeed", String(value));
            break;
          case 6:
            _amgpelletstove_callback("room/fanspeed", String(value));
            break;
          case 7:
            _amgpelletstove_callback("auger/speed", String(value));
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
          case 0:
            switch (value)
            {
              case 0:
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
                _amgpelletstove_callback("phase", "cooling down phase 1");
                break;
              case 2050:
                _amgpelletstove_callback("phase", "cooling down phase 2");
                break;
              default:
                _amgpelletstove_callback("phase", "unknown");
                break;
            }
            _amgpelletstove_callback("phase/value", String(value));
            break;
          case 13:
            _amgpelletstove_callback("phase/timer", String(value));
            break;
          case 14:
            _amgpelletstove_callback("board/inputs", String(value));
            break;
          case 15:
            _amgpelletstove_callback("extractorsensorlevel", String(value));
            break;
          case 255:
            break;
        }
        nextupdatetime = millis() + 250; // After successfull transfer wait 250ms before requesting next value
      }
    }
  }

  if (millis() > nextupdatetime)
  {
    nextupdatetime = millis() + 2000; // On first or failed transefer wait 2 second before next (re)try
    _amgpelletstove_sendserial();
  }  
}



