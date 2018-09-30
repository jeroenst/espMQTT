//#define BATHROOM
//#define BEDROOM2
//#define GARDEN //ESP8285
//#define MAINPOWERMETER
//#define OPENTHERM
//#define SONOFF4CH //ESP8285
#define DUCOBOX
//#define SONOFFDUAL
//#define OLDBATHROOM
//#define SMARTMETER
//#define WATERMETER
//#define NOISE
//#define IRRIGATION
//#define SOIL
//#define GROWATT
//#define DIMMER
//#define SONOFFS20_PRINTER

#ifdef SONOFFS20_PRINTER
#define ESPNAME "SONOFFS20_PRINTER"
#define SONOFFS20
#define SONOFFCH_TIMEOUT 1800
uint32_t sonoffch_starttime[4];
#endif

#ifdef DIMMER
#define ESPNAME "DIMMER"
#define TRIAC_PIN D1
#define ZEROCROSS_PIN D2
#define FLASHBUTTON D3
#define ESPLED D4
#include "dimmer.h"
#endif

#ifdef NOISE
#define ESPNAME "NOISE"
#define MICOFFSET 0
#define MICGAIN 1
#define FLASHBUTTON D3
#define ESPLED D4
#endif

#ifdef GROWATT
#define ESPNAME "GROWATT"
#define FLASHBUTTON D3
#define ESPLED D4
#include "growatt.h"
#endif

#ifdef SOIL
#define ESPNAME "SOIL"
#define FLASHBUTTON D3
#define ESPLED D4
#endif

#ifdef DUCOBOX
#define ESPNAME "DUCOBOX"
#ifndef ARDUINO_ESP8266_ESP01
#error "Wrong board selected! Select Generic ESP8285 module"
#endif
#define SONOFFDUAL
#define FLASHBUTTON 10
#define ESPLED 13
#include "ducobox.h";
#endif

#ifdef GARDEN
#define ESPNAME "GARDEN"
#ifndef ARDUINO_ESP8266_ESP01
#error "Wrong board selected! Select Generic ESP8285 module"
#endif
#define SONOFF4CH
#endif

#ifdef IRRIGATION
#define ESPNAME "IRRIGATION"
uint32_t sonoffch_starttime[4];
#define SONOFFCH_TIMEOUT 1800
#ifndef ARDUINO_ESP8266_ESP01
#error "Wrong board selected! Select Generic ESP8285 module"
#endif
#define SONOFF4CH
#endif


#ifdef SONOFF4CH
#ifndef ESPNAME
#define ESPNAME "SONOFF4CH"
#endif
#ifndef ARDUINO_ESP8266_ESP01
#error "Wrong board selected! Select Generic ESP8285 module"
#endif
#define FLASHBUTTON 0
#define ESPLED 13
#define SONOFFCH 4
const byte sonoff_relays[4] = {12, 5, 4, 15};
const byte sonoff_buttons[4] = {0, 9, 10, 14};
static bool sonoff_oldbuttons[4] = {1, 1, 1, 1};
#endif

#ifdef SONOFFDUAL
#ifndef ESPNAME
#define ESPNAME "SONOFFDUAL"
#endif
#ifndef ARDUINO_ESP8266_ESP01
#error "Wrong board selected! Select Generic ESP8285 module"
#endif
#define FLASHBUTTON 10
#define ESPLED 13
#define SONOFFCH 2
const byte sonoff_relays[2] = {12, 5};
const byte sonoff_buttons[2] = {0, 9};
static bool sonoff_oldbuttons[2] = {1, 1};
#endif

#ifdef SONOFFS20
// Remember: board: generic esp8266 module, flashmode=dio
#ifndef ESPNAME
#define ESPNAME "SONOFFS20"
#endif
#ifndef ARDUINO_ESP8266_ESP01
#error "Wrong board selected! Select Generic ESP8285 module"
#endif
#define FLASHBUTTON 0
#define ESPLED 13
#define SONOFFCH 1
const byte sonoff_relays[1] = {12};
const byte sonoff_buttons[1] = {0};
static bool sonoff_oldbuttons[1] = {1};
#endif

#ifdef MAINPOWERMETER
#define ESPNAME "MAINPOWERMETER"
#define FLASHBUTTON D3
#define ESPLED D4
#define ADS0_RDY_PIN    D0 //ADS1256 data ready
#define ADS0_CS_PIN    D1 //ADS1256 chip select
#define ADS1_RDY_PIN    D2 //ADS1256 data ready
#define ADS1_CS_PIN    D8 //ADS1256 chip select
#include "circuitspowermeter.h"
#endif

#ifdef OPENTHERM
#define ESPNAME "OPENTHERM"
#define FLASHBUTTON D3
#define ESPLED D4
#define ONEWIREPIN D6
#include "opentherm.h"
#endif

#ifdef WATERMETER
#define ESPNAME "WATERMETER"
#define NODEMCULEDPIN D0
#define FLASHBUTTON D3
#define ESPLED D4
#define WATERPULSEPIN D5
#define I2C_SDA D2
#define I2C_SCL D1
#define I2C_EEPROM_ADDRESS 0x50
#define EE24LC512MAXBYTES  512000
#include "watermeter.h"
#include "i2ceeprom_wearleveling.h"
#endif

#ifdef GARDEN2
#define ESPNAME "GARDEN2"
#define FLASHBUTTON D3
#define ESPLED D4
#define DHTPIN D5
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
#define ONEWIREPIN D2
#endif

#ifdef OLDBATHROOM
#define ESPNAME "OLDBATHROOM"
#define SMALLOLED
#define I2C_SDA D2
#define I2C_SCL D1
#define OLED_ADDRESS 0x3c
#define FLASHBUTTON D3
#define ESPLED D4
#define ESPLED_TIMEOUT_OFF 60 // In bathroom we want the led to go off after some time
#define DHTPIN D5
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
#define OLEDX 32
#define OLEDSMALL
#endif

#ifdef BATHROOM
#define ESPNAME "BATHROOM"
#define ESPLED_TIMEOUT_OFF 0 // In sleepingroom we want the led to go off after some time
#define FLASHBUTTON D3
#define ESPLED D4
#define DHTPIN D5
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
#define DHTTEMPOFFSET -1
#define NEOPIXELPIN D6
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel neopixelleds = Adafruit_NeoPixel(2, NEOPIXELPIN, NEO_RGB + NEO_KHZ400);
#endif

#ifdef BEDROOM2
#define ESPNAME "BEDROOM2"
#define FLASHBUTTON D3
#define ESPLED D4
#define ESPLED_TIMEOUT_OFF 60 // In sleepingroom we want the led to go off after some time
#define I2C_SDA D2
#define I2C_SCL D1
#define OLED_ADDRESS 0x3c
#define DHTPIN D5
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
#define OLEDX 0
#define MH_Z19
#endif

#ifdef SMARTMETER
#define ESPNAME "SMARTMETER"
#define FLASHBUTTON D3
#define ESPLED D4
#include "smartmeter.h"
#endif

#define EEPROMSTRINGSIZE 40 // 2 positions are used, one for 0 character and one for checksum

#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>        // Include the mDNS library
#include <RemoteDebug.h>
#include <user_interface.h>
#include "SimpleMap.h";

struct dataStruct
{
  String data;
  bool sendMQTT;
};

SimpleMap<String, String> *dataMap = new SimpleMap<String, String>([](String &a, String &b) -> int {
  if (a == b) return 0;      // a and b are equal
  else if (a > b) return 1;  // a is bigger than b
  else return -1;            // a is smaller than b
});

RemoteDebug Debug;
bool updatemqtt = 0;

static const char webpage_P[] PROGMEM = "<!DOCTYPE html><html><meta charset=\"UTF-8\"><meta name=\"google\" content=\"notranslate\"><meta http-equiv=\"Content-Language\" content=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><style>table{width: 400px; margin: auto;}</style></head><body><CENTER><div align='center' style='width:400px; margin:auto'><CENTER><H1><p id='header'></p></H1></CENTER><p id='table'></p><A HREF='settings'>Settings</A></div></CENTER><script>function refreshsite(){var obj,dbParam,xmlhttp,myObj,x,txt ='';xmlhttp=new XMLHttpRequest();xmlhttp.onreadystatechange=function(){if(this.readyState==4&&this.status==200){myObj=JSON.parse(this.responseText);txt+='<TABLE>';for (x in myObj){if(x=='espname')document.getElementById('header').innerHTML=myObj[x].toUpperCase();txt+='<tr><td>'+x.split('/').join(' ')+'</td><td>'+myObj[x]+'</td></tr>';}txt+='</table>';document.getElementById('table').innerHTML = txt;}};xmlhttp.open('POST','data.json',true);xmlhttp.setRequestHeader('Content-type','application/x-www-form-urlencoded');xmlhttp.send();}refreshsite();window.setInterval(refreshsite, 5000);</script></body></html>";

extern "C" {
#include "user_interface.h"
}

#ifdef DHTPIN
#include "DHT.h"
DHT dht(DHTPIN, DHTTYPE);
#endif



#ifdef OLED_ADDRESS
#include <Wire.h>
#include "SSD1306Wire.h"
SSD1306Wire display(OLED_ADDRESS, I2C_SDA, I2C_SCL);
#endif

#ifdef ONEWIREPIN
#include <OneWire.h>
#include <DallasTemperature.h>

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONEWIREPIN);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature oneWireSensors(&oneWire);


DeviceAddress onewire_chReturnWaterThermometer, onewire_dcwSupplyWaterThermometer;
float onewire_chReturnWaterTemperature = -127, onewire_dcwSupplyWaterTemperature = -127;
float oldonewire_chReturnWaterTemperature = -127, oldonewire_dcwSupplyWaterTemperature = -127;
bool onewire_chReturnWaterEnabled = false, onewire_dcwSupplyWaterEnabled = false;
#endif

WiFiClient wifiClient;
static uint8_t wifiTimer = 0;
PubSubClient mqttclient(wifiClient);
ESP8266WebServer webserver(80);

String chipid;
uint32_t uptime = 0;
bool timertick = 1;
os_timer_t myTimer;
String mqtt_server = "";
String mqtt_username = "";
String mqtt_password = "";
String web_password = "";
String esp_hostname = "";
String esp_orig_hostname = "";
static bool debug;
uint8_t ledontime = 1;
uint8_t ledofftime = 1;
#include "esp8266_peri.h";

void initSerial()
{
  Serial.setRxBufferSize(2048); 
#if defined(MH_Z19) || defined(OPENTHERM) || defined(GROWATT)
  Serial.begin(9600);  //Init serial 9600 baud
  Serial.setDebugOutput(false);
#else
  Serial.begin(115200); //Init serial 19200 baud
#endif
}

String getdatamap(String topic)
{
  return dataMap->get(topic);
}

void showdatamap()
{
  for (int i = 0; i < dataMap->size(); i++)
  {
    DEBUG("%s=%s\n", dataMap->getKey(i).c_str(), dataMap->getData(i).c_str());
    yield();
  }
}

void putdatamap(String topic, String value, bool sendupdate = true, bool forcesend = false)
{
  if (((dataMap->get(topic) != value) || forcesend) && sendupdate)
  {
    DEBUGV ("DATAMAP %s=%s\n", topic.c_str(), value.c_str());
    dataMap->put(topic, value);
    if (mqttclient.connected())
    {
      String mqtttopic = String("home/" + WiFi.hostname() + "/" + topic);
      if (sendupdate)
      {
        mqttclient.publish(mqtttopic.c_str(), value.c_str(), true);
        DEBUG("MQTT PUBLISHING %s=%s\n", mqtttopic.c_str(), value.c_str());
      }
    }
  }
  else dataMap->put(topic, value);
}

void update_systeminfo(bool writestaticvalues = false)
{
  char uptimestr[20];
  sprintf(uptimestr, "%ld:%02ld:%02ld:%02ld", uptime / 86400, (uptime / 3600) % 24, (uptime / 60) % 60, uptime % 60);
  if (writestaticvalues)
  {
    putdatamap("espname", ESPNAME);
    putdatamap("hostname", WiFi.hostname());
    putdatamap("sourcefile", String(__FILE__).substring(String(__FILE__).lastIndexOf('/') + 1));
    putdatamap("compiletime", String(__DATE__) + " " + __TIME__);
    putdatamap("chipid", String(chipid));
    putdatamap("status", "online");
  }
  putdatamap("uptime", String(uptimestr), uptime % 60 == 0);
  putdatamap("freeram", String(system_get_free_heap_size()), uptime % 60 == 0);
  putdatamap("wifi/state", WiFi.status() == WL_CONNECTED ? "connected" : "disconnected");
  putdatamap("wifi/localip", WiFi.localIP().toString());
  putdatamap("wifi/ssid", String(WiFi.SSID()));
  putdatamap("wifi/rssi", String(WiFi.RSSI()), uptime % 10 == 0);
  putdatamap("wifi/channel", String(WiFi.channel()));
  putdatamap("mqtt/server", String(mqtt_server));
  putdatamap("mqtt/state", mqttclient.connected() ? "connected" : "disconnected");
}

#ifdef DHTPIN
void update_dht()
{
  static uint8 errors = 0;
  float rh = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temp = dht.readTemperature();
#ifdef DHTTEMPOFFSET
  temp += DHTTEMPOFFSET;
#endif

  float hi = dht.computeHeatIndex(temp, rh, false);

  // Check if any reads failed and exit early (to try again).
  if (isnan(temp) || isnan(rh))
  {
    DEBUG("Reading DHT22 failed!\n");
    if (errors < 255) errors++;
  }
  else errors = 0;

  DEBUG(("dht22temp=" + String(temp, 1) + " dht22rh=" + String(rh, 1) + " dht22hi=" + String(hi, 1) + "\n").c_str());

  if (errors == 0)
  {
    putdatamap("dht22/temperature", String(temp, 1));
    putdatamap("dht22/humidity", String(rh, 1));
    putdatamap("dht22/heatindex", String(hi, 1));
  }

  if (errors == 5)
  {
    putdatamap("dht22/temperature", "-");
    putdatamap("dht22/humidity", "-");
    putdatamap("dht22/heatindex", "-");
  }

#ifdef NEOPIXELPIN
  if (rh < 1) neopixelleds.setPixelColor(1, neopixelleds.Color(0, 0, 0));
  else if (rh < 60) neopixelleds.setPixelColor(1, neopixelleds.Color(0, 20, 0));
  else if (22rh < 90) neopixelleds.setPixelColor(1, neopixelleds.Color(30, 15, 0));
  else neopixelleds.setPixelColor(1, neopixelleds.Color(50, 0, 0));
  neopixelleds.show();
#endif
}
#endif

void mqttcallback(char* topic, byte* payload, unsigned int length) {
  String payloadstring = "";
  for (byte i = 0; i < length; i++) payloadstring += char(payload[i]);
  DEBUG("Message arrived in topic: %s, payload: %s\n", topic, payloadstring.c_str());

#ifdef SONOFFCH
  for (byte i = 0; i < SONOFFCH; i++)
  {
    if (String(topic) == String("home/" + WiFi.hostname() + "/setrelay/" + i))
    {
      bool inverse = false;
      #ifdef SONOFFCHINVERSE
        inverse = true;
      #endif
      if (payloadstring == "0") digitalWrite(sonoff_relays[i], inverse ? 1:0);
      if (payloadstring == "1") digitalWrite(sonoff_relays[i], inverse ? 0:1);
#ifdef SONOFFCH_TIMEOUT
      if (payloadstring == "0") sonoffch_starttime[i] = 0;
      if (payloadstring == "1") sonoffch_starttime[i] = uptime;
#endif
      updatemqtt = 1;
    }
  }
#endif

#ifdef OPENTHERM
  if (String(topic) == String("home/" + WiFi.hostname() + "/setthermostattemporary"))
  {
    DEBUG("RECEIVED SETTHERMOSTATTEMPORARY %s", (char*)payload);
    opentherm_setthermosttattemporary(payloadstring.toFloat());
  }
  if (String(topic) == String("home/" + WiFi.hostname() + "/setthermostatcontinue"))
  {
    DEBUG("RECEIVED SETTHERMOSTATCONTINUE %s\n", (char*)payload);
    opentherm_setthermosttatcontinue(payloadstring.toFloat());
  }
#endif

#ifdef DUCOBOX
  if (String(topic) == String("home/" + WiFi.hostname() + "/setfan")) ducobox_setfan(payloadstring.toInt());
#endif

#ifdef DIMMER
  /*  if (String(topic) == String("home/" + WiFi.hostname() + "/setminfanspeed"))
    {
      uint16_t minfanspeed = payloadstring.toInt();
      DEBUG("RECEIVED SETMINFANSPEED %d\n", minfanspeed);
      if ((minfanspeed > 0) && (minfanspeed <= 3000)) ducobox_minfanspeed = minfanspeed;
    }*/
  if (String(topic) == String("home/" + WiFi.hostname() + "/setdimvalue"))
  {
    dimmer_setdimvalue(payloadstring.toInt());
    putdatamap ("dimvalue", String(dimmer_getdimvalue()));
  }
#endif
}

String uint64ToString(uint64_t input) {
  String result = "";
  uint8_t base = 10;

  do {
    char c = input % base;
    input /= base;

    if (c < 10)
      c += '0';
    else
      c += 'A' - 10;
    result = c + result;
  } while (input);
  return result;
}

void write_oled_display()
{
  int lcdline = -9;
#ifdef OLED_ADDRESS

  DEBUG ("Writing OLED display\n");
  display.clear();
  int rssi = WiFi.RSSI();

#ifndef OLEDSMALL
  display.drawString(OLEDX, lcdline += 9, "IP: " + WiFi.localIP().toString());
  // Display a signal strenght icon
  display.fillRect(114, 10, 15, 1);
  if (WiFi.status() == WL_CONNECTED)
  {
    if (rssi > -90) display.fillRect(114, 8, 2, 2);
    if (rssi > -80) display.fillRect(117, 6, 2, 4);
    if (rssi > -70) display.fillRect(120, 4, 2, 6);
    if (rssi > -60) display.fillRect(123, 2, 2, 8);
    if (rssi > -50) display.fillRect(126, 0, 2, 10);
  }
#else
  display.fillRect(OLEDX + 53, 9, 9, 1);
  if (WiFi.status() == WL_CONNECTED)
  {
    if (rssi > -90) display.fillRect(OLEDX + 53, 8, 1, 2);
    if (rssi > -80) display.fillRect(OLEDX + 55, 6, 1, 4);
    if (rssi > -70) display.fillRect(OLEDX + 57, 4, 1, 6);
    if (rssi > -60) display.fillRect(OLEDX + 59, 2, 1, 8);
    if (rssi > -50) display.fillRect(OLEDX + 61, 0, 1, 10);
  }
#endif

#if defined(DHTPIN) || defined(SHT21)
#ifndef OLEDSMALL
  display.drawString(OLEDX, lcdline += 9, "TEMP: " + getdatamap("dht22/temperature") + " °C");
  display.drawString(OLEDX, lcdline += 9, "RH: " + getdatamap("dht22/humidity") + " %");
  display.drawString(OLEDX, lcdline += 9, "HI: " + getdatamap("dht22/heatindex") + " °C");
#else
  display.drawString(OLEDX, lcdline += 9, "T:" + getdatamap("dht22/temperature") + " °C");
  display.drawString(OLEDX, lcdline += 9, "RH:" + getdatamap("dht22/humidity") + " %");
  display.drawString(OLEDX, lcdline += 9, "HI:" + getdatamap("dht22/heatindex") + " °C");
#endif
#endif


#if defined(MH_Z19)
#ifndef OLEDSMALL
  display.drawString(OLEDX, lcdline += 9, "CO2: " + getdatamap("mhz19/co2") + " ppm");
  display.drawString(OLEDX, lcdline += 9, "MHZ_TEMP: " + getdatamap("mhz19/temperature") + " °C");
#else
  display.drawString(OLEDX, lcdline += 9, "CO2:" + getdatamap("mhz19/co2"));
  display.drawString(OLEDX, lcdline += 9, "MT:" + getdatamap("mhz19/temperature") + "°C");
#endif
#endif

  display.display();
#endif
}

#ifdef NOISE
void handle_noise()
{
  static uint8_t timeout = 0; // We can't read ADC all the time because it will disconnect wifi
  static uint16_t analogmeanready = 0;
  int32_t analogread = 0;
  static double analogmean = 0;

  if (timeout++ == 0)
  {
    analogread = analogRead(A0) - 536;
    if (analogread < 0) analogread *= -1;
    analogmean = (analogread + (analogmean * 99)) / 100;
    if (analogmeanready++ >= 200)
    {
      DEBUG("Analog read=%d, Analog mean=%.0f\n", analogread, analogmean);
      putdatamap ("noise", String(analogmean, 0));
      analogmeanready = 0;
    }
  }
}
#endif


static byte flashbuttonstatus = 0;
static int8_t previouswifistatus = -1;

void loop()
{
  static int8_t rssi = 0;
  static int8_t oldrssi = 0;

  ESP.wdtFeed(); // Prevent HW WD to kick in...
  ArduinoOTA.handle();
  Debug.handle();

  
  mqttclient.loop();
  webserver.handleClient();
  yield();

#ifdef SONOFFCH
  sonoff_handle();
#endif

#ifdef NOISE
  handle_noise();
#endif

#ifdef DUCOBOX
  ducobox_handle();
#endif

#ifdef WATERMETER
  static uint32_t watermeter_liters = i2cEeprom_read();
  if (watermeter_handle())
  {
    putdatamap("water/lmin", String(watermeter_getflow()));
    putdatamap("water/m3h", String(double(watermeter_getflow()) * 0.06, 3));

    if (watermeter_liters != watermeter_getliters())
    {
      watermeter_liters = watermeter_getliters();
      putdatamap("water/liter", String(watermeter_liters));
      putdatamap("water/m3", String(double(watermeter_liters) / 1000, 3));
      i2cEeprom_write(watermeter_liters);
    }
  }
#endif

#ifdef OPENTHERM
  opentherm_handle();
#endif

#ifdef SMARTMETER
  smartmeter_handle();
#endif

#ifdef GROWATT
  growatt_handle();
#endif

  if (timertick == 1) // Every 1 second check sensors and update display (it would be insane to do it more often right?)
  {
    timertick = 0;
    updatemqtt = 1;

    update_systeminfo();

#ifdef MAINPOWERMETER
  static uint8_t circuitnr = 0;
  int32_t mW;
  int32_t mVA;
  int32_t mA;
  int32_t mV;
  uint8_t nrofsamples;
  circuitspowermeter_read(circuitnr, mW, mVA, mA, mV, 10);
  if (circuitnr==0) putdatamap("mainsvoltage", String(mV / 1000));
  putdatamap("circuit/" + String((circuitnr + 1)) + "/mA", String(mA));
  putdatamap("circuit/" + String((circuitnr + 1)) + "/W", String(mW / 1000));
  putdatamap("circuit/" + String((circuitnr + 1)) + "/VA", String(mVA / 1000));
  if (circuitnr < 14) circuitnr++;
  else circuitnr = 0;
#endif

    if (wifiTimer < 20) wifiTimer++;

#ifdef FLASHBUTTON
    handleflashbutton();
#endif

#ifdef ESPLED_TIMEOUT_OFF
    static uint16_t esp_timeout_off_timer = 0;
    if (ledontime > 0)
    {
      esp_timeout_off_timer++;
      if (esp_timeout_off_timer > ESPLED_TIMEOUT_OFF)
      {
        ledontime = 0;
        ledofftime = 1;
      }
    }
    else esp_timeout_off_timer = 0;
#endif

#ifdef DHTPIN
    update_dht();
#endif

#ifdef OPENTHERM
    static uint8_t ds18b20timer = 0;
    if (ds18b20timer == 0)
    {
      ds18b20timer = 10;
      DEBUG("Requesting DS18B20 temperatures...\n");
      //oneWireSensors.setWaitForConversion(false);
      oneWireSensors.requestTemperatures();
      float temperature;
      temperature = oneWireSensors.getTempC(onewire_chReturnWaterThermometer);
      DEBUG("chreturnwatertemp=%f\n", temperature);
      if ((onewire_chReturnWaterEnabled) && (temperature != -127)) putdatamap("ow/ch/returnwatertemperature", String(temperature,1));
      temperature = oneWireSensors.getTempC(onewire_dcwSupplyWaterThermometer);
      DEBUG("dcwsupplywatertemp=%f\n", temperature);
      if ((onewire_dcwSupplyWaterEnabled) && (temperature != -127)) putdatamap("ow/dcw/temperature", String(temperature,1));
    }
    else ds18b20timer--;
#endif


#ifdef MH_Z19
    read_MHZ19();
#endif

    write_oled_display();

    if (WiFi.status() == WL_CONNECTED)
    {
      static uint8_t mqttreconnecttimer = 10;
      wifiTimer = 0;
      if (WiFi.status() != previouswifistatus)
      {
        DEBUG("Wifi connected to %s\n", WiFi.SSID().c_str());
#ifdef NEOPIXELPIN
        neopixelleds.setPixelColor(0, neopixelleds.Color(30, 15, 0));
        neopixelleds.show();
#endif
        if (flashbuttonstatus == 0)
        {
          ledontime = 10;
          ledofftime = 10;
        }
        mqttreconnecttimer = 1; // reconnect mqtt
      }

      static bool mqttnewconnection = false;
      if (mqttreconnecttimer > 0) mqttreconnecttimer--;
      if (!mqttclient.connected())
      {
        if (mqttreconnecttimer == 0)
        {
          DEBUG("Connecting mqtt\n");
          mqttclient.connect(("ESP8266Client" + chipid).c_str(), mqtt_username.c_str(), mqtt_password.c_str(), String("home/" + WiFi.hostname() + "/status").c_str(), 0, 1, "offline"); // Every 10 seconds try to reconnect...
          mqttreconnecttimer = 10;
        }
        mqttnewconnection = true;
      }
      else
      {
#ifdef NEOPIXELPIN
        neopixelleds.setPixelColor(0, neopixelleds.Color(0, 20, 0));
        neopixelleds.show();
#endif
        if (mqttnewconnection)
        {
          mqttnewconnection = false;
          // Publish all values to mqtt upon connect
          publishdatamap();

#ifdef SONOFFCH
          for (byte i = 0; i < SONOFFCH; i++) mqttclient.subscribe(("home/" + WiFi.hostname() + "/setrelay/" + String(i)).c_str());
#endif
#ifdef OPENTHERM
          mqttclient.subscribe(("home/" + WiFi.hostname() + "/setthermostattemporary").c_str());
          mqttclient.subscribe(("home/" + WiFi.hostname() + "/setthermostatcontinue").c_str());
#endif
#ifdef DUCOBOX
          mqttclient.subscribe(("home/" + WiFi.hostname() + "/setfan").c_str());
#endif
#ifdef DIMMER
          mqttclient.subscribe(("home/" + WiFi.hostname() + "/setdimvalue").c_str());
          mqttclient.subscribe(("home/" + WiFi.hostname() + "/setdimstate").c_str());
#endif
        }
      }
    }
    else
    {
#ifdef NEOPIXELPIN
      neopixelleds.setPixelColor(0, neopixelleds.Color(30, 0, 0));
      neopixelleds.show();
#endif
      if (flashbuttonstatus == 0)
      {
        ledontime = 1;
        ledofftime = 1;
      }
      if (wifiTimer >= 30)
      {
        WiFi.disconnect(false);
        delay(10);
        WiFi.begin();
        wifiTimer = 0;
      }
    }
    previouswifistatus = WiFi.status();
  }
}

#ifdef SONOFFCH
void sonoff_init()
{
  bool inverse = false;
  #ifdef SONOFFCHINVERSE
    inverse = true;
  #endif
  for (byte i = 0; i < SONOFFCH; i++)
  {
    digitalWrite(sonoff_relays[i], inverse ? true : false);
    pinMode(sonoff_relays[i], OUTPUT);
    pinMode(sonoff_buttons[i], INPUT_PULLUP);
  }

#ifdef SONOFFCH_TIMEOUT
  for (uint8_t i = 0; i < SONOFFCH; i++) sonoffch_starttime[i] = 0;
#endif
}

void sonoff_handle()
{
#ifdef SONOFFCH_TIMEOUT
  for (uint8_t i = 0; i < SONOFFCH; i++)
  {
    if ((sonoffch_starttime[i] > 0) && (sonoffch_starttime[i] + SONOFFCH_TIMEOUT < uptime))
    {
      sonoffch_starttime[i] = 0;
      bool inverse = false;
      #ifdef SONOFFCHINVERSE
        inverse = true;
      #endif
      digitalWrite(sonoff_relays[i], inverse ? 1 : 0);
    }
  }
#endif

  if (flashbuttonstatus == 0)
  {
    for (uint8_t i = 0; i < SONOFFCH; i++)
    {
      bool buttonstate = digitalRead(sonoff_buttons[i]);
      if (buttonstate)
      {
        if (sonoff_oldbuttons[i] != buttonstate)
        {
          DEBUG ("SONOFF BUTTON %d PRESSED\n", i);
          digitalWrite(sonoff_relays[i], digitalRead(sonoff_relays[i]) ? 0 : 1);
          bool inverse = false;
          #ifdef SONOFFCHINVERSE
            inverse = true;
          #endif
#ifdef SONOFFCH_TIMEOUT
          if (digitalRead(sonoff_relays[i]) != inverse) sonoffch_starttime[i] = uptime;
          else sonoffch_starttime[i] = 0;
#endif
          updatemqtt = 1;
        }
      }
      sonoff_oldbuttons[i] = buttonstate;
    }
  }
  for (int i = 0; i < SONOFFCH; i++)
  {
    bool inverse = false;
    #ifdef SONOFFCHINVERSE
      inverse = true;
    #endif
    String relaystate = digitalRead(sonoff_relays[i]) != inverse ? "1" : "0";
    putdatamap ("relay/" + String(i), relaystate);
  }
}
#endif

void write_eeprom(String data, byte eepromindex)
{
  //DEBUG("(String("write_eeprom(" + data + "," + eepromindex = ");\n")).c_str());
  char checksum = 20;
  for (int i = 0; i < EEPROMSTRINGSIZE - 1; i++)
  {
    char chr = data.c_str()[i];
    checksum += chr;
    EEPROM.write(i + (EEPROMSTRINGSIZE * eepromindex), chr);
    EEPROM.write(i + (EEPROMSTRINGSIZE * eepromindex) + 1, checksum);
    if (chr == 0) break;
  }
  EEPROM.commit();
}

void publishdatamap()
{
  for (int i = 0; i < dataMap->size(); i++)
  {
    String topic = String("home/" + WiFi.hostname() + "/" + dataMap->getKey(i));
    String payload = dataMap->getData(i);
    mqttclient.publish(topic.c_str(), payload.c_str(), true);
    DEBUG("MQTT PUBLISHING %s=%s\n", topic.c_str(), payload.c_str());
  }
}

void handleflashbutton()
{
#ifdef FLASHBUTTON
  static uint8_t flashbuttontimer = 0;
  if ((digitalRead(FLASHBUTTON) == 0) && (flashbuttontimer < (2 ^ (sizeof(flashbuttontimer) * 8))))
  {
    flashbuttontimer++;
    DEBUG("Flash button is pressed, flashbuttontimer=%d\n", flashbuttontimer);
  }
  else
  {
    flashbuttontimer = 0;
  }

  if (flashbuttontimer == 6) // After 6 seconds start access point
  {
    flashbuttonstatus = 1;
    if (!WiFi.softAP(WiFi.hostname().c_str(), "", 6, 0)) DEBUG("Failed setting WiFi.softAP()");
    WiFi.mode(WIFI_AP_STA);
    DEBUG("Wifi Accesspoint started!\n");
    ledontime = 1;
    ledofftime = 0;
  }

  if (flashbuttontimer == 3) // After 3 seconds clear passwords
  {
    flashbuttonstatus = 2;
    web_password = "";
    DEBUG("Web Password cleared until reboot!\n");
    ledontime = 1;
    ledofftime = 4;
  }

  if (flashbuttontimer > 10) flashbuttontimer = 10;

  if (flashbuttontimer == 1) // When short pressed flash button reeanble led
  {
    if (WiFi.status() == WL_CONNECTED)
      if (flashbuttonstatus == 0)
      {
        ledontime = 10;
        ledofftime = 10;
      }
      else
      {
        if (flashbuttonstatus == 0)
        {
          ledontime = 1;
          ledofftime = 1;
        }
      }
  }
#endif
}

uint8_t read_eeprom(String * data, byte eepromindex)
{
  Serial.println(String("read_eeprom(" + String(eepromindex) + ");\n").c_str());
  String eepromstring = "";
  char calcchecksum = 20;
  char readchecksum = 0;
  for (int i = 0; i < EEPROMSTRINGSIZE - 1; i++)
  {
    char eepromval = EEPROM.read(i + (EEPROMSTRINGSIZE * eepromindex));
    calcchecksum += eepromval;
    readchecksum = EEPROM.read(i + (EEPROMSTRINGSIZE * eepromindex) + 1);
    if (eepromval == 0) break;
    eepromstring += char(eepromval);
  }
  if (calcchecksum == readchecksum)
  {
    *data = eepromstring;
    return 1;
  }
  else return 0;
}


void timerCallback(void *pArg)
{
  static uint8_t ms = 0;
  ms++;
  if (ms >= 10)
  {
    uptime++;
    timertick = 1;
    ms = 0;
  }
#ifdef ESPLED
  static uint8_t ledtimer = 0;
  #ifdef ESPLEDINVERSE
  if ((ledtimer == ledontime) && (ledofftime > 0)) digitalWrite(ESPLED, 0);
  if ((ledtimer == 0) && (ledontime > 0)) digitalWrite(ESPLED, 1);
  #else
  if ((ledtimer == ledontime) && (ledofftime > 0)) digitalWrite(ESPLED, 1);
  if ((ledtimer == 0) && (ledontime > 0)) digitalWrite(ESPLED, 0);
  #endif
  ledtimer++;
  if (ledtimer >= ledontime + ledofftime) ledtimer = 0;
#endif
}

#ifdef MH_Z19
void read_MHZ19()
{
  static uint8_t errorcounter = 0;
  bool error = 0;
  uint8_t cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  // command to ask for data
  uint8_t response[9]; // for answer

  Serial.flush();
  Serial.swap();

  Serial.write(cmd, 9); //request PPM CO2

  delay(20); // Wait 20 ms for packets to receive (104us per bit, +/- 1ms per byte + some execution time)

  // The serial stream can get out of sync. The response starts with 0xff, try to resync.
  uint8_t maxcounter = 0;
  while (Serial.available() > 0 && (uint8_t)Serial.peek() != 0xFF && maxcounter++ < 10) {
    Serial.read();
  }

  memset(response, 0, 9);
  Serial.readBytes(response, 9);
  Serial.swap();
  /* for (int i = 0; i < 9; i++)
    {
     DEBUG(" %d", response[i]);
    }
    DEBUG("\n");
  */
  if (response[1] == 0x86)
  {
    uint8_t crc = 0;
    for (uint8_t i = 1; i < 8; i++) {
      crc += response[i];
    }
    crc = 255 - crc + 1;

    if (response[8] == crc)
    {
      int responseHigh = (int) response[2];
      int responseLow = (int) response[3];
      int ppm = (256 * responseHigh) + responseLow;
      int temp = response[4] - 40;
      putdatamap ("mhz19/co2", String(ppm));
      putdatamap ("mhz19/temperature", String(temp));
      errorcounter = 0;
    }
  }
  else
  {
    DEBUG("Invalid response from MHZ_19 CO2 sensor!\n");
    if (errorcounter < 255) errorcounter++;
    if (errorcounter == 5)
    {
      putdatamap ("mhz19/co2", "-");
      putdatamap ("mhz19/temperature", "-");
    }
  }
}
#endif


void handleWWWSettings()
{
  if (!webserver.authenticate("admin", web_password.c_str())) {
    return webserver.requestAuthentication();
  }

  String wifiselectoptions = "<option value=\"" + WiFi.SSID() + "\" selected>" + WiFi.SSID() + "</option>";
  uint8_t n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    if (WiFi.SSID(i) != WiFi.SSID()) wifiselectoptions += "<option value=\"" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</option>";
  }

  if (webserver.method() == HTTP_POST)
  {
    String postwifissid = "";
    String postwifikey = "";
    for (uint8_t i = 0; i < webserver.args(); i++)
    {
      if (webserver.argName(i) == "rebootdevice")
      {
        webserver.send(200, "text/html", "<HTML><BODY>Device Rebooting...<BR><A HREF=\"/\">Return to main page</A></BODY></HTML>");
        delay(3000);
        ESP.restart();
      }
    }
    for (uint8_t i = 0; i < webserver.args(); i++)
    {
      if (webserver.argName(i) == "wifissid") postwifissid = webserver.arg(i);
      if (webserver.argName(i) == "wifikey") postwifikey = webserver.arg(i);
      if (webserver.argName(i) == "mqttserver") mqtt_server = webserver.arg(i);
      if (webserver.argName(i) == "mqttusername") mqtt_username = webserver.arg(i);
      if (webserver.argName(i) == "mqttpassword") mqtt_password = webserver.arg(i);
      if (webserver.argName(i) == "webpassword") web_password = webserver.arg(i);
      if (webserver.argName(i) == "hostname") esp_hostname = webserver.arg(i);
#ifdef WATERMETER
      if (webserver.argName(i) == "watermeterliter")
      {
        if (webserver.arg(i) != getdatamap("water/liter"))
        {
          watermeter_setliters(atoi(webserver.arg(i).c_str()));
          i2cEeprom_write(watermeter_getliters());
          putdatamap("water/liter", String(watermeter_getliters()));
          putdatamap("water/m3", String(double(watermeter_getliters()) / 1000, 3));
        }
      }
#endif
    }
    write_eeprom(mqtt_server, 0);
    write_eeprom(mqtt_username, 1);
    write_eeprom(mqtt_password, 2);
    write_eeprom(web_password, 3);
    write_eeprom(esp_hostname, 4);
    ArduinoOTA.setPassword(web_password.c_str());
    mqttclient.disconnect(); // Disconnect mqtt server, it will auto reconnect in main loop...
    mqttclient.setServer(mqtt_server.c_str(), 1883);
    WiFi.mode(WIFI_STA); // After saving settings return to wifi client mode and disable AP

    if ((postwifissid != WiFi.SSID()) || (postwifikey != WiFi.psk()) || (esp_hostname != WiFi.hostname()))
    {
      webserver.send(200, "text/html", "<HTML><BODY>Settings Saved.<BR>Please connect to proper wifi network and open the page of the saved hostname.</BODY></HTML>");
      delay(3000);
      if (esp_hostname != "") WiFi.hostname(esp_hostname);
      WiFi.disconnect(true);
      WiFi.begin(postwifissid.c_str(), postwifikey.c_str()); // Save wifi ssid and key and also activate new hostname...
      flashbuttonstatus = 0;
      previouswifistatus = -1;
    }
    else webserver.send(200, "text/html", "<HTML><BODY>Settings Saved.<BR><A HREF=\"/\">Return to main page</A></BODY></HTML>");
  }

  String webpage = "";
  webpage += "<HTML><HEAD><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></HEAD><BODY><CENTER><div align=\"left\" style=\"width:400px; margin:auto\">";
  webpage += String("<CENTER><H1>") + WiFi.hostname() + "</H1></CENTER><form action=\"/settings\" method=\"post\" autocomplete=\"off\"><TABLE style=\"width:400px; margin:auto\">";
  webpage += String("<TR><TD>Hostname</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"") + String(EEPROMSTRINGSIZE - 2) + "\" name=\"hostname\" value=\"" + WiFi.hostname() + "\"></TD></TR>";
  webpage += String("<TR><TD>WifiSSID</TD><TD><select style=\"width:200\" name=\"wifissid\">") + wifiselectoptions + "</select></TD></TR>";
  webpage += String("<TR><TD>WifiKEY</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"64\" name=\"wifikey\" value=\"") + String(WiFi.psk()) + "\"></TD></TR>";
  webpage += String("<TR><TD>MQTT Server</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"") + String(EEPROMSTRINGSIZE - 2) + "\" name=\"mqttserver\" value=\"" + mqtt_server + "\"></TD></TR>";
  webpage += String("<TR><TD>MQTT Username</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"") + String(EEPROMSTRINGSIZE - 2) + "\" name=\"mqttusername\" value=\"" + mqtt_username + "\"></TD></TR>";
  webpage += String("<TR><TD>MQTT Password</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"") + String(EEPROMSTRINGSIZE - 2) + "\" name=\"mqttpassword\" value=\"" + mqtt_password + "\"></TD></TR>";
  webpage += String("<TR><TD>Settings Password</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"") + String(EEPROMSTRINGSIZE - 2) + "\" name=\"webpassword\" value=\"" + web_password + "\"></TD></TR>";
#ifdef WATERMETER
  webpage += String("<TR><TD>Watermeter Liter</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"64\" name=\"watermeterliter\" value=\"") + getdatamap("water/liter") + "\"></TD></TR>";
#endif
  webpage += "</TABLE><BR><CENTER><input type=\"hidden\" name=\"rebootdevice\" value=\"1\"><input type=\"submit\" value=\"Reboot Device\">&nbsp;&nbsp;&nbsp;&nbsp;<input type=\"submit\" value=\"Save Settings\"></form><BR><BR><A HREF=\"/\">Return to main page</A></CENTER></div></BODY></HTML>";
  webserver.send(200, "text/html", webpage);
}

void handleJsonData() {
  String json = "{";
  for (int i = 0; i < dataMap->size(); i++)
  {
    json += "\"" + dataMap->getKey(i) + "\":\"" + dataMap->getData(i) + "\",";
  }
  json.remove(json.length() - 1);
  json += "}";
  webserver.send(200, "text/html", json);
}

void handleWWWRoot() {
  DEBUG("New webclient connected...\n");
  webserver.send_P(200, "text/html", webpage_P);
}

void ducoboxcallback (String topic, String payload)
{
  putdatamap(topic, payload);
}

void openthermcallback (String topic, String payload)
{
  putdatamap(topic, payload);
}

void growattcallback (String topic, String payload)
{
  putdatamap(topic, payload);
}

void smartmetercallback (String topic, String payload)
{
  putdatamap(topic, payload);
}

void setup() {

  ESP.wdtDisable(); // Use hardware watchdog of 6 seconds to prevent auto reboot when function takes more time..
  EEPROM.begin(512);

  initSerial();
  Serial.println("\n\nInitializing ESP8266...\n\n");

  char buffer[25];
  snprintf(buffer, 25, "%08X", ESP.getChipId());
  chipid = buffer;

  // Read settings from EEPROM
  Serial.println("Reading internal EEPROM...\n");
  if (!read_eeprom(&mqtt_server, 0))
  {
    Serial.println("Error reading mqtt server from internal eeprom\n");
  }
  if (!read_eeprom(&mqtt_username, 1))
  {
    Serial.println("Error reading mqtt username from internal eeprom\n");
  }
  if (!read_eeprom(&mqtt_password, 2))
  {
    Serial.println("Error reading mqtt password from internal eeprom\n");
  }
  if (!read_eeprom(&web_password, 3))
  {
    Serial.println("Error reading web password from internal eeprom\n");
  }
  if (!read_eeprom(&esp_hostname, 4))
  {
    Serial.println("Error reading hostname from internal eeprom\n");
    esp_hostname = String("ESP_") + ESPNAME;
  }

#ifdef SONOFFCH
  sonoff_init();
#endif
  WiFi.setAutoReconnect(true);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.mode(WIFI_STA);
  Serial.println("Wifi hostname=" + esp_hostname + "\n\n");
  WiFi.hostname(esp_hostname);
  WiFi.begin();

  ArduinoOTA.setHostname(esp_hostname.c_str());

#if defined(MH_Z19) || defined(OPENTHERM) || defined (DUCOBOX) || defined (SMARTMETER) || defined (GROWATT)
  Debug.setSerialEnabled(false);
#else
  Debug.setSerialEnabled(true);
#endif
  Debug.begin(esp_hostname.c_str());
  Debug.setResetCmdEnabled(true);
  Debug.setCallBackProjectCmds(&processCmdRemoteDebug);
  DEBUG("ESP8266 started...\n");
  DEBUG("Hostname=%s\n", WiFi.hostname().c_str());

  ArduinoOTA.setPassword(web_password.c_str());

  ArduinoOTA.onStart([]() {
    Serial.end();
#ifdef DIMMER    
    dimmer_stop();
#endif
  });

  ArduinoOTA.onEnd([]() {
    initSerial();
#ifdef DIMMER
    dimmer_init(ZEROCROSS_PIN, TRIAC_PIN);
#endif
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
//    ESP.wdtFeed();
  });
  
  ArduinoOTA.begin();

  update_systeminfo(true);



  // Initialize display

#ifdef OLED_ADDRESS
  display.init();
#ifndef OLEDSMALL
  display.flipScreenVertically();
#endif
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.clear(); // clearing the display
  display.drawString(OLEDX, 0, "Initializing...");
  display.display();
#endif

#ifdef WATERMETER
  watermeter_init(WATERPULSEPIN, NODEMCULEDPIN, i2cEeprom_read());
#endif

#ifdef MAINPOWERMETER
  circuitspowermeter_init(ADS0_CS_PIN, ADS0_RDY_PIN, ADS1_CS_PIN, ADS1_RDY_PIN);
#endif

#ifdef ONEWIREPIN
  if (!oneWireSensors.getAddress(onewire_dcwSupplyWaterThermometer, 0)) {
    DEBUG("Unable to find address for onewire_dcwSupplyWaterThermometer\n");
  }
  else onewire_dcwSupplyWaterEnabled = true;
  if (!oneWireSensors.getAddress(onewire_chReturnWaterThermometer, 1)) {
    DEBUG("Unable to find address for Device onewire_chReturnWaterThermometer\n");

  }
  else onewire_chReturnWaterEnabled = true;
#endif


  webserver.on("/", handleWWWRoot);
  webserver.on("/data.json", handleJsonData);
  webserver.on("/settings", handleWWWSettings);
  webserver.begin();
  mqttclient.setServer(mqtt_server.c_str(), 1883);
  MDNS.begin(esp_hostname.c_str());
  MDNS.addService("http", "tcp", 80);
  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, 100, true);

#ifdef ESPLED
  pinMode(ESPLED, OUTPUT);
  #ifdef ESPLEDINVERSE
  digitalWrite(ESPLED, 1);
  #else
  digitalWrite(ESPLED, 0);
  #endif
  
#endif

#ifdef FLASHBUTTON
  pinMode(FLASHBUTTON, INPUT_PULLUP);
#endif;

#ifdef SMARTMETER
  smartmeter_init(smartmetercallback);
#endif;


  mqttclient.setCallback(mqttcallback);

#ifdef OPENTHERM
  opentherm_init(openthermcallback);
#endif

#ifdef GROWATT
  growatt_init(growattcallback);
#endif

#ifdef NEOPIXELPIN
  neopixelleds.begin();               // init van de strip
  neopixelleds.setPixelColor(0, neopixelleds.Color(0, 0, 0));
  neopixelleds.setPixelColor(1, neopixelleds.Color(0, 0, 0));
  neopixelleds.show();
#endif

#ifdef DIMMER
  dimmer_init(ZEROCROSS_PIN, TRIAC_PIN);  
#endif

#ifdef DUCOBOX
  ducobox_init(sonoff_relays[0], sonoff_relays[1], 10, ducoboxcallback);
#endif
}

void processCmdRemoteDebug()
{
  String lastCmd = Debug.getLastCommand();
  if (lastCmd == "help") DEBUG("Available Debug Commands:\n");
  if (lastCmd == "help") DEBUG("showdatamap\n");
  if (lastCmd == "showdatamap") showdatamap();
#ifdef WATERMETER
  if (lastCmd == "help") DEBUG("  watermeterreadeeprom\n  watermeterwriteeeprom\n");
  if (lastCmd == "watermeterreadeeprom")
  {
    watermeter_setliters(i2cEeprom_read());
  }
  if (lastCmd == "watermeterwriteeeprom")
  {
    i2cEeprom_write(watermeter_getliters());
  }
#endif
}
