/*
   Needed libraries:
    https://github.com/jeroenst/RemoteDebug
    https://github.com/jeroenst/ESPAsyncTCP
    https://github.com/jeroenst/async-mqtt-client
    https://github.com/jeroenst/Syslog
    
   Optional libraries depending on defines:
    (Sonoff POW) https://github.com/jeroenst/hlw8012
    (RGB LED) https://github.com/jeroenst/my92xx
    (DS18B20) https://github.com/jeroenst/OneWire
    (DS18B20) https://github.com/jeroenst/Arduino-Temperature-Control-Library
    (I2C OLED DISPLAY) https://github.com/jeroenst/esp8266-oled-ssd1306

   Libraries via Arduino Library Manager:
    (I2C) Wire


   Known bugs:
    MQTT SSL not working (https://github.com/marvinroger/async-mqtt-client/issues/107)

*/
//#define GENERIC8266
//#define BATHROOM
//#define BEDROOM2
//#define GARDEN //ESP8285
//#define MAINPOWERMETER
//#define OPENTHERM
#define SONOFF4CH //ESP8285
//#define DUCOBOX
//#define SONOFFDUAL
//#define OLDBATHROOM
//#define SMARTMETER
//#define WATERMETER
//#define NOISE
//#define IRRIGATION
//#define SOIL
//#define GROWATT
//#define DIMMER
//#define SONOFFS20
//#define SONOFFBULB
//#define SONOFFS20_PRINTER
//#define SONOFFPOW // not finished yet
//#define SONOFFPOWR2
//#define WEATHER
//#define AMGPELLETSTOVE

#ifdef SONOFFS20_PRINTER
#define ESPNAME "SONOFFS20_PRINTER"
#define SONOFFS20
#define SONOFFCH_TIMEOUT 1800
uint32_t sonoffch_starttime[1];
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

#ifdef GENERIC8266
#define ESPNAME "GENERIC8266"
#define FLASHBUTTON D3
#define ESPLED D4
#endif

#ifdef AMGPELLETSTOVE
#define ESPNAME "AMGPELLETSTOVE"
#define NODEMCULEDPIN D0
#define FLASHBUTTON D3
#define ESPLED D4
#include "amgpelletstove.h";
#endif

#ifdef WEATHER
#define ESPNAME "WEATHER"
#define NODEMCULEDPIN D0
#define FLASHBUTTON D3
#define ESPLED D4
#define ONEWIREPIN D5
#define RAINMETERPIN D1
#endif


#ifdef GROWATT
#define ESPNAME "GROWATT"
#define NODEMCULEDPIN D0
#define FLASHBUTTON D3
#define ESPLED D4
#include "growatt.h"
#endif

#ifdef SOIL
#define ESPNAME "SOIL"
#define FLASHBUTTON D3
#define ESPLED D4
#define SERIALLLOG
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

#ifdef SONOFFPOW
#define ESPNAME "SONOFFPOW"
#ifndef ARDUINO_ESP8266_ESP01
#error "Wrong board selected! Select Generic ESP8285 module"
#endif
#define FLASHBUTTON 0
#define ESPLED 13
#define SONOFFCH 1

const byte sonoff_relays[1] = {12};
const byte sonoff_buttons[1] = {0};
static bool sonoff_oldbuttons[1] = {1};

#include <HLW8012.h>

// GPIOs
#define SONOFFPOW_SEL_PIN 5
#define SONOFFPOW_CF1_PIN 13
#define SONOFFPOW_CF_PIN 14

// Set SEL_PIN to HIGH to sample current
// This is the case for Itead's Sonoff POW, where a
// the SEL_PIN drives a transistor that pulls down
// the SEL pin in the HLW8012 when closed
#define SONOFFPOW_CURRENT_MODE HIGH

// These are the nominal values for the resistors in the circuit
#define SONOFFPOW_CURRENT_RESISTOR 0.001
#define SONOFFPOW_VOLTAGE_RESISTOR_UPSTREAM ( 5 * 470000 ) // Real: 2280k
#define SONOFFPOW_VOLTAGE_RESISTOR_DOWNSTREAM ( 1000 ) // Real 1.009k

HLW8012 hlw8012;

// When using interrupts we have to call the library entry point
// whenever an interrupt is triggered
void ICACHE_RAM_ATTR hlw8012_cf1_interrupt() {
  hlw8012.cf1_interrupt();
}
void ICACHE_RAM_ATTR hlw8012_cf_interrupt() {
  hlw8012.cf_interrupt();
}

#endif

#ifdef SONOFFPOWR2
#define ESPNAME "SONOFFPOWR2"
#ifndef ARDUINO_ESP8266_ESP01
#error "Wrong board selected! Select Generic ESP8285 module"
#endif
#define FLASHBUTTON 0
#define ESPLED 13
#define SONOFFCH 1

const byte sonoff_relays[1] = {12};
const byte sonoff_buttons[1] = {0};
static bool sonoff_oldbuttons[1] = {1};
double voltval = 0;
double currentval = 0;
double powerval = 0;
#endif

#ifdef SONOFFBULB
// Remember: board: generic esp8266 module, flashmode=dio
#ifndef ESPNAME
#define ESPNAME "SONOFFBULB"
#define APONBOOT 1
#include "my92xx.h";
my92xx * _my92xx;
#define MY92XX_MODEL        MY92XX_MODEL_MY9231
#define MY92XX_CHIPS        2
#define MY92XX_DI_PIN       12
#define MY92XX_DCKI_PIN     14

#define MY92XX_RED          4
#define MY92XX_GREEN        3
#define MY92XX_BLUE         5
#define MY92XX_CW           1
#define MY92XX_WW           0
#endif
#ifndef ARDUINO_ESP8266_ESP01
#error "Wrong board selected! Select Generic ESP8285 module"
#endif
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
#define ESPLED_TIMEOUT_OFF 60 // In bedroom we want the led to go off after some time
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

// ################################################################################################################# END OF DEFINES ####################################################################################################################

#include <time.h>
#include <sys/time.h>                   // struct timeval
#include <coredecls.h>                  // settimeofday_cb()
#include "sntp.h"

#include <EEPROM.h>
#include <ESP8266WiFi.h>
//#include <WiFiUdp.h>
//#include <PubSubClient.h>
#include <AsyncMqttClient.h>
AsyncMqttClient mqttClient;
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>        // Include the mDNS library
#include <RemoteDebug.h>
#include <user_interface.h>
#include "SimpleMap.h";
#include <Syslog.h>
#include <ESP8266Ping.h>

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udpClientSyslog;

// Create a new empty syslog instance
Syslog syslog(udpClientSyslog, SYSLOG_PROTO_IETF);

struct Mainstate {
  bool wificonnected = false;
  bool mqttconnected = false;
  bool mqttready = false;
  bool mqttsenddatamap = false;
  bool defaultpassword = false;
  bool accesspoint = false;
} mainstate;

struct dataMapStruct {
  String payload = "";
  bool send = true;
};

SimpleMap<String, dataMapStruct> *dataMap = new SimpleMap<String, dataMapStruct>([](String &a, String &b) -> int {
  if (a == b) return 0;      // a and b are equal
  else if (a > b) return 1;  // a is bigger than b
  else return -1;            // a is smaller than b
});

SimpleMap<int, String> *eepromMap = new SimpleMap<int, String>([](int &a, int &b) -> int {
  if (a == b) return 0;      // a and b are equal
  else if (a > b) return 1;  // a is bigger than b
  else return -1;            // a is smaller than b
});


RemoteDebug Debug;
bool updatemqtt = 0;

static const char webpage_P[] PROGMEM = "<!DOCTYPE html><html><meta charset=\"UTF-8\"><meta name=\"google\" content=\"notranslate\"><meta http-equiv=\"Content-Language\" content=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><style>table{width: 400px; margin: auto;}</style></head><body><CENTER><div align='center' style='width:400px; margin:auto'><CENTER><H1><p id='header'></p></H1></CENTER><p id='table'></p><A HREF='settings'>Settings</A></div></CENTER><script>function refreshsite(){var obj,dbParam,xmlhttp,myObj,x,txt ='';xmlhttp=new XMLHttpRequest();xmlhttp.onreadystatechange=function(){if(this.readyState==4&&this.status==200){myObj=JSON.parse(this.responseText);txt+='<TABLE>';for (x in myObj){if(x=='hostname')document.getElementById('header').innerHTML=myObj[x].toUpperCase();txt+='<tr><td>'+x.split('/').join(' ')+'</td><td>'+myObj[x]+'</td></tr>';}txt+='</table>';document.getElementById('table').innerHTML = txt;}};xmlhttp.open('POST','data.json',true);xmlhttp.setRequestHeader('Content-type','application/x-www-form-urlencoded');xmlhttp.send();}refreshsite();window.setInterval(refreshsite, 5000);</script></body></html>";

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

#ifdef OPENTHERM
DeviceAddress onewire_chReturnWaterThermometer, onewire_dcwSupplyWaterThermometer;
float onewire_chReturnWaterTemperature = -127, onewire_dcwSupplyWaterTemperature = -127;
float oldonewire_chReturnWaterTemperature = -127, oldonewire_dcwSupplyWaterTemperature = -127;
bool onewire_chReturnWaterEnabled = false, onewire_dcwSupplyWaterEnabled = false;
#endif

#ifdef WEATHER
DeviceAddress onewire_OutsideAddress;
float onewire_chOutsideTemperature = -127;
float oldonewire_chOutsideTemperature = -127;
#endif
#endif

//WiFiClientSecure wifiClientSecure;
//WiFiClient wifiClient;
static uint8_t wifiTimer = 0;
//PubSubClient mqttClient;
ESP8266WebServer webserver(80);
#include <WiFiUdp.h>

String chipid;
uint32_t uptime = 0;
bool timertick = 1;
os_timer_t myTimer;
String mqtt_server = "";
String mqtt_username = "";
String mqtt_password = "";
String mqtt_maintopic = "";
int mqtt_port = 1883;
bool mqtt_ssl = 0;
String esp_password = "esplogin";
String esp_hostname = "";
String esp_orig_hostname = "";
static bool debug;
static bool mqttReady = false;
#include "esp8266_peri.h";

#if defined(MH_Z19) || defined(OPENTHERM) || defined (DUCOBOX) || defined (SMARTMETER) || defined (GROWATT) || defined (SONOFFPOWR2) || defined (AMGPELLETSTOVE)
#undef SERIALLOG
#else
#define SERIALLOG
#endif

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
#include <Ticker.h>
Ticker mqttReconnectTimer;
Ticker wifiReconnectTimer;


#define LOG(message) mylog(__func__, message)
#define LOGL(message, level) mylog(__func__, message,level)

void publishdatamap(int32_t packetId = -1, bool publishall = false);

void mylog (char const * caller_name, String message, int level = LOG_INFO)
{
  int debuglevel = Debug.ANY;
  switch (level) {
    case LOG_INFO: debuglevel = Debug.INFO; break;
    case LOG_ERR: debuglevel = Debug.ERROR; break;
    case LOG_WARNING: debuglevel = Debug.WARNING; break;
    case LOG_DEBUG: debuglevel = Debug.DEBUG; break;
    case LOG_NOTICE: debuglevel = Debug.VERBOSE; break;
  }
  if (Debug.isActive(debuglevel)) Debug.printf("(%s) %s\n", caller_name, message.c_str());
  syslog.log(level, (String("(") + String(caller_name) + ") " + message).c_str());
}

String getdatamap(String topic)
{
  return dataMap->get(topic).payload;
}

void showdatamap()
{
  for (int i = 0; i < dataMap->size(); i++)
  {
    DEBUG("%s=%s (%d)\n", dataMap->getKey(i).c_str(), dataMap->getData(i).payload.c_str(), dataMap->getData(i).send);
    yield();
  }
}

void putdatamap(String topic, String value, bool sendupdate = true, bool forcesend = false)
{
  DEBUGV ("DATAMAP %s=%s\n", topic.c_str(), value.c_str());

  dataMapStruct datamapstruct;
  datamapstruct.payload = value;
  datamapstruct.send = dataMap->get(topic).send;
  
  if (((dataMap->get(topic).payload != value) || forcesend) && sendupdate)
  {
    datamapstruct.send = true;
  }

  dataMap->put(topic, datamapstruct);
}

void update_systeminfo(bool writestaticvalues = false, bool sendupdate = true)
{
  char uptimestr[20];
  sprintf(uptimestr, "%ld:%02ld:%02ld:%02ld", uptime / 86400, (uptime / 3600) % 24, (uptime / 60) % 60, uptime % 60);
  if (writestaticvalues)
  {
    putdatamap("espname", ESPNAME, sendupdate);
    putdatamap("hostname", WiFi.hostname(), sendupdate);
    putdatamap("sourcefile", String(__FILE__).substring(String(__FILE__).lastIndexOf('/') + 1), sendupdate);
    putdatamap("compiletime", String(__DATE__) + " " + __TIME__, sendupdate);
    putdatamap("chipid", String(chipid), sendupdate);
    putdatamap("status", "online", sendupdate);
    putdatamap("flash/id", String(ESP.getFlashChipId()), sendupdate);
    putdatamap("flash/size/real", String(ESP.getFlashChipRealSize()), sendupdate);
    putdatamap("flash/size/ide", String(ESP.getFlashChipSize()), sendupdate);
    FlashMode_t ideMode = ESP.getFlashChipMode();
    putdatamap("flash/mode", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"), sendupdate);
    putdatamap("flash/speed", String(ESP.getFlashChipSpeed()), sendupdate);
  }
  putdatamap("uptime", String(uptimestr), uptime % 60 == 0, sendupdate);
  putdatamap("freeram", String(system_get_free_heap_size()), uptime % 60 == 0, sendupdate);
  putdatamap("wifi/state", WiFi.status() == WL_CONNECTED ? "connected" : "disconnected", sendupdate);
  putdatamap("wifi/localip", WiFi.localIP().toString(), sendupdate);
  putdatamap("wifi/ssid", String(WiFi.SSID()), sendupdate);
  putdatamap("wifi/rssi", String(WiFi.RSSI()), uptime % 10 == 0, sendupdate);
  putdatamap("wifi/channel", String(WiFi.channel()), sendupdate);
  putdatamap("mqtt/server", String(mqtt_server), sendupdate);
  putdatamap("mqtt/port", String(mqtt_port), sendupdate);
  putdatamap("mqtt/ssl", String(mqtt_ssl), sendupdate);
  putdatamap("mqtt/state", mqttClient.connected() ? "connected" : "disconnected", sendupdate);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event)
{
  DEBUG("Connected to Wi-Fi.");
  mainstate.wificonnected = true;
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event)
{
  DEBUG("Disconnected from Wi-Fi.\n");
  mainstate.wificonnected = false;
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  if (!mainstate.accesspoint) wifiReconnectTimer.once(2, connectToWifi); // trying to connect to wifi can cause AP to fail
}

void initWifi()
{
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  WiFi.setAutoReconnect(false); // We handle reconnect our self
  //WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.mode(WIFI_STA);
  DEBUG("Wifi hostname=%s\n", esp_hostname.c_str());
  WiFi.hostname(esp_hostname);
}

void connectToWifi()
{
  LOG("Connecting to Wi-Fi...");
  WiFi.begin();
}

void initMqtt()
{
  // Because mqtt lib uses a pointer to const char[], we have to make a static variables
  static String clientid = String("ESP8266_") + chipid;
  static String willtopic = String(mqtt_maintopic) + "status";

  mqttClient.onMessage(onMqttMessage);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setClientId(clientid.c_str());
  mqttClient.setCredentials(mqtt_username.c_str(), mqtt_password.c_str());
  mqttClient.setServer(mqtt_server.c_str(), mqtt_port);
  mqttClient.setWill(willtopic.c_str(), 0, 1, "offline");
  mqttClient.setSecure(mqtt_ssl);
  //#define MQTT_SERVER_FINGERPRINT {0x3F, 0x80, 0xCF, 0x16, 0xD9, 0x43, 0x3B, 0x92, 0xB6, 0x3A, 0x0A, 0x02, 0xFE, 0x27, 0x0B, 0x60, 0xC1, 0x9A, 0x8B, 0xB1}
  //mqttClient.addServerFingerprint((const uint8_t[])MQTT_SERVER_FINGERPRINT);
}

void connectToMqtt()
{
  DEBUGV("connectToMqtt()\n");
  if (!mainstate.mqttconnected) 
  {
    LOG("Connecting mqtt");
    mqttReconnectTimer.once(30, connectToMqtt); // retry over 30 seconds if connection can not be established
    mqttClient.connect();
  }
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
  LOG("Disconnected from MQTT (" + String(int(reason)) + ")");

  if (reason == AsyncMqttClientDisconnectReason::TLS_BAD_FINGERPRINT) {
    LOG("Bad server fingerprint.\n");
  }

  mainstate.mqttconnected = false;
  mainstate.mqttready = false;

  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttUnsubscribe(uint16_t packetId) {
  LOG ("Unsubscribe acknowledged. packetid=" + String(packetId));
}


void mqttdosubscriptions(int32_t packetId = -1)
{
  DEBUG("mqttdosubscriptions (%d)\n", packetId);
  static int32_t nextpacketid = -1;
  static uint16_t nextsubscribe = 0;
  bool startpublish = false;
  static String subscribetopic = ""; // We need this static variable because mqttclient.subscribe uses a pointer

  if (packetId == -1) nextsubscribe = 0;
  if (packetId > 0) nextsubscribe++;
  nextpacketid = -1;
  subscribetopic = "";
  while ((subscribetopic == "") && (nextsubscribe < 14))
  {
    //DEBUG("mqttdosubscriptions while nextsubscribe=%d\n", nextsubscribe);
    switch (nextsubscribe)
    {
#if defined(OPENTHERM) || defined(GENERIC8266)
      case 0: subscribetopic = mqtt_maintopic + "setthermostattemporary"; break;
      case 1: subscribetopic = mqtt_maintopic + "setthermostatcontinue"; break;
      case 2: subscribetopic = mqtt_maintopic + "setchwatertemperature"; break;
#endif
#ifdef DUCOBOX
      case 3: subscribetopic = mqtt_maintopic + "setfan"; break;
#endif
#ifdef DIMMER
      case 4: subscribetopic = mqtt_maintopic + "setdimvalue"; break;
      case 5: subscribetopic = mqtt_maintopic + "setdimstate"; break;
#endif
#ifdef SONOFFBULB
      case 6: subscribetopic = mqtt_maintopic + "setcolor"; break;
#endif
#if defined(AMGPELLETSTOVE)
      case 7: subscribetopic = mqtt_maintopic + "setonoff"; break;
      case 8: subscribetopic = mqtt_maintopic + "setpower"; break;
      case 9: subscribetopic = mqtt_maintopic + "settemperature"; break;
#endif
#ifdef SONOFFCH
      case 10:  if (0 < SONOFFCH) subscribetopic = mqtt_maintopic + "setrelay/" + String(0); break;
      case 11:  if (1 < SONOFFCH) subscribetopic = mqtt_maintopic + "setrelay/" + String(1); break;
      case 12:  if (2 < SONOFFCH) subscribetopic = mqtt_maintopic + "setrelay/" + String(2); break;
      case 13:  if (3 < SONOFFCH) subscribetopic = mqtt_maintopic + "setrelay/" + String(3); break;
#endif
    }
    if (subscribetopic == "") nextsubscribe++;
  }

  if (subscribetopic == "") publishdatamap(-1, true); // When subscibtion has finished start publishing of datamap
  else nextpacketid = mqttClient.subscribe(subscribetopic.c_str() ,1);
  DEBUG("mqttdosubscriptions end nextpacketid=%d\n", nextpacketid);
}
  
void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
  DEBUG ("Subscribe acknowledged packetid=%d qos=%d", packetId, qos);
  mqttdosubscriptions(packetId);
}


void onMqttPublish(uint16_t packetId)
{
  DEBUG ("Publish acknowledged packetid=%d",packetId);
  publishdatamap(packetId);
}

void onMqttConnect(bool sessionPresent) {
  LOG ("Connected to MQTT sessionPresent="+String(sessionPresent));
  mainstate.mqttconnected = true;
  update_systeminfo(true);
  mqttdosubscriptions();
}

void initSerial()
{
  Serial.setRxBufferSize(2048);
#if defined(MH_Z19) || defined(OPENTHERM) || defined(GROWATT)
  Serial.setDebugOutput(false);
  Serial.begin(9600);  //Init serial 9600 baud
#elif defined (SONOFFPOWR2)
  Serial.setDebugOutput(false);
  Serial.begin(4800, SERIAL_8E1);
#elif defined (AMGPELLETSTOVE)
  amgpelletstove_init(amgpelletstovecallback, logdebug);
#else
  Serial.begin(115200); //Init serial 115200 baud
#endif
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

  DEBUG("%s", ("dht22temp=" + String(temp, 1) + " dht22rh=" + String(rh, 1) + " dht22hi=" + String(hi, 1) + "\n").c_str());

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

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  String payloadstring = "";
  for (int i = 0; i < len; i++)
  {
    payloadstring += char(payload[i]);
  }
  String topicstring = topic;
  LOG("MQTT RECEIVED len=" + String(len) + " topic=\"" + topicstring + "\" payload=\"" + payloadstring + "\"");

#ifdef SONOFFCH
  for (byte i = 0; i < SONOFFCH; i++)
  {
    if (String(topic) == String(mqtt_maintopic + "setrelay/" + i))
    {
      bool inverse = false;
#ifdef SONOFFCHINVERSE
      inverse = true;
#endif
      if (payloadstring == "0") digitalWrite(sonoff_relays[i], inverse ? 1 : 0);
      if (payloadstring == "1") digitalWrite(sonoff_relays[i], inverse ? 0 : 1);
#ifdef SONOFFCH_TIMEOUT
      if (payloadstring == "0") sonoffch_starttime[i] = 0;
      if (payloadstring == "1") sonoffch_starttime[i] = uptime;
#endif
      updatemqtt = 1;
    }
  }
#endif

#ifdef OPENTHERM
  if (String(topic) == String(mqtt_maintopic + "setthermostattemporary"))
  {
    LOG(String("RECEIVED SETTHERMOSTATTEMPORARY ") + payloadstring);
    opentherm_setthermosttattemporary(payloadstring.toFloat());
  }
  if (String(topic) == String(mqtt_maintopic + "setthermostatcontinue"))
  {
    LOG(String("RECEIVED SETTHERMOSTATCONTINUE ") + payloadstring);
    opentherm_setthermosttatcontinue(payloadstring.toFloat());
  }
  if (String(topic) == String(mqtt_maintopic + "setchwatertemperature"))
  {
    LOG(String("RECEIVED SETCHWATERTEMPERATURE ") + payloadstring);
    opentherm_setchwatertemperature(payloadstring.toFloat());
  }
#endif

#ifdef DUCOBOX
  if (String(topic) == String(mqtt_maintopic + "setfan")) ducobox_setfan(payloadstring.toInt());
#endif

#ifdef DIMMER
  if (String(topic) == String(mqtt_maintopic + "setdimvalue"))
  {
    dimmer_setdimvalue(payloadstring.toInt());
    putdatamap ("dimvalue", String(dimmer_getdimvalue()));
  }
#endif

#ifdef SONOFFBULB
  if (String(topic) == String(mqtt_maintopic + "setcolor"))
  {
    long number = strtol(payloadstring.substring(0, 6).c_str(), NULL, 16);
    _my92xx->setChannel(MY92XX_RED, (number >> 16) & 0xFF);
    _my92xx->setChannel(MY92XX_GREEN, (number >> 8) & 0xFF);
    _my92xx->setChannel(MY92XX_BLUE, number & 0xFF);

    number = strtol(payloadstring.substring(6, 10).c_str(), NULL, 16);
    _my92xx->setChannel(MY92XX_CW, (number >> 8) & 0xFF);
    _my92xx->setChannel(MY92XX_WW, number & 0xFF);
    _my92xx->update();
    putdatamap ("color", payloadstring);
  }
#endif

#ifdef AMGPELLETSTOVE
  amgpelletstove_receivemqtt(topicstring, payloadstring);
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

  if (mainstate.mqttready) publishdatamap();

  //mqttClient.loop();
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


#ifdef RAINMETERPIN
#define PULSEMM 0.3636
  static uint32_t rainpinmillis = 0;
  static uint32_t rainpulses = 0;
  static uint32_t rainpulsesminute = 0;
  static uint32_t rainpulseshour = 0;
  bool rainpinstate = 0;
  static bool count = 0;
  rainpinstate = digitalRead(RAINMETERPIN);
  if ((rainpinstate == 1) && (millis() - 50 > rainpinmillis) && count)
  {
    rainpulses++; // Pulse has to settle for 50ms before counting
    rainpulseshour++; // Pulse has to settle for 50ms before counting
    rainpulsesminute++; // Pulse has to settle for 50ms before counting
    String mqtttopic = String(mqtt_maintopic + "rain/pulse");
    mqttClient.publish(mqtttopic.c_str(), 0, false, "1");
    count = 0;
  }
  if (rainpinstate == 0)
  {
    rainpinmillis = millis();
    count = 1;
  }
  putdatamap ("rain/pulses", String(rainpulses));
  putdatamap ("rain/mm", String((double(rainpulses)*PULSEMM), 1));

  static bool hourreg = false;
  static bool firsthourreg = true;
  if ((uptime % 3600) || (firsthourreg))
  {
    putdatamap ("rain/hour/pulses", String(rainpulseshour));
    putdatamap ("rain/hour/mm", String((double(rainpulseshour)*PULSEMM), 1));
    firsthourreg = false;
    if (!hourreg)
    {
      putdatamap ("rain/lasthour/pulses", String(rainpulseshour));
      putdatamap ("rain/lasthour/mm", String((double(rainpulseshour)*PULSEMM), 1));
      rainpulseshour = 0;
      hourreg = true;
    }
  }
  else hourreg = false;

  static bool minreg = false;
  static bool firstminreg = true;
  if ((uptime % 60) || (firstminreg))
  {
    putdatamap ("rain/minute/pulses", String(rainpulsesminute));
    putdatamap ("rain/minute/mm", String((double(rainpulsesminute)*PULSEMM), 1));
    firstminreg = false;
    if (!minreg)
    {
      putdatamap ("rain/lastminute/pulses", String(rainpulsesminute));
      putdatamap ("rain/lastminute/mm", String((double(rainpulsesminute)*PULSEMM), 1));
      rainpulsesminute = 0;
      minreg = true;
    }
  }
  else minreg = false;

  digitalWrite(NODEMCULEDPIN, rainpinstate);
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

#ifdef FLASHBUTTON
  flashbutton_handle();
#endif

#ifdef AMGPELLETSTOVE
  amgpelletstove_handle();
#endif

#ifdef SONOFFPOWR2
  static char serbuffer[100];
  static int serpointer = -1;
  static char oldserval = 0;
  if (Serial.available() > 0)
  {
    int serval = Serial.read();
    if ((serpointer >= 0) && (serpointer < 100))
    {
      serbuffer[serpointer] = serval;
      serpointer++;
    }
    if (serpointer >= 100)
    {
      Serial.flush();
      serpointer = -1;
    }
    if (((oldserval == 0x55) || ((oldserval & 0xf0) == 0xf0) || (oldserval == 0xAA)) && (serval == 0x5A))
    {
      static uint8_t statusbyte = 0;
      if ((oldserval & 0xf0) == 0xf0) statusbyte = (oldserval & 0x0f);
      else statusbyte = 0;
      if (serpointer == 24)
      {
        uint32_t valueco = 0;
        uint32_t valueca = 0;
        float value = 0;
        valueco = (uint32_t(serbuffer[0]) << 16) + (uint16_t(serbuffer[1]) << 8) + serbuffer[2];
        valueca = (uint32_t(serbuffer[3]) << 16) + (uint16_t(serbuffer[4]) << 8) + serbuffer[5];
        if (valueca > 0) voltval = double(double(valueco) / double(valueca));
        else voltval = 0;

        valueco = (uint32_t(serbuffer[6]) << 16) + (uint16_t(serbuffer[7]) << 8) + serbuffer[8];
        valueca = (uint32_t(serbuffer[9]) << 16) + (uint16_t(serbuffer[10]) << 8) + serbuffer[11];
        if (valueca > 0) currentval = double(double(valueco) / double(valueca));
        else currentval = 0;
        if (statusbyte & 0x02) currentval = 0;

        valueco = (uint32_t(serbuffer[12]) << 16) + (uint16_t(serbuffer[13]) << 8) + serbuffer[14];
        valueca = (uint32_t(serbuffer[15]) << 16) + (uint16_t(serbuffer[16]) << 8) + serbuffer[17];
        if (valueca > 0) powerval = double(double(valueco) / double(valueca));
        else powerval = 0;
        if (statusbyte & 0x02) powerval = 0;
        Serial.flush();
        serpointer = -1;
      }
      serpointer = 0;
    }
    oldserval = serval;
  }
#endif

  if (timertick == 1) // Every 1 second check sensors and update display (it would be insane to do it more often right?)
  {
    timertick = 0;
    updatemqtt = 1;

#ifdef GENERIC8266
    putdatamap("uptime", String(uptime));
#endif

    if (((uptime % 60) == 0) && (WiFi.status() == WL_CONNECTED))
    {
      time_t now;
      time(&now);
      LOG(String("Uptime=") + String(uptime) + String(" DateTime=") + ctime(&now));
    }

#ifdef SONOFFPOWR2
    static uint8_t powr2sec = 0;
    if (powr2sec++ > 5) // Every 5 seconds send update about power usage
    {
      putdatamap("voltage", String(voltval, 1));
      putdatamap("power", String(powerval, 1));
      putdatamap("current", String(currentval, 3));
      powr2sec = 0;
    }
#endif

#ifdef APONBOOT
    if ((uptime == 30) && (WiFi.status() != WL_CONNECTED))
    {
      DEBUG("Connection to wifi failed, starting accesspoint");
      if (!WiFi.softAP(WiFi.hostname().c_str(), "esplogin", 6, 0)) DEBUG("Failed setting WiFi.softAP()");
      WiFi.mode(WIFI_AP_STA);
      esp_password = "esplogin";
    }
    if (uptime == 330)
    {
      DEBUG("Stopping accesspoint");
      WiFi.mode(WIFI_STA);
    }
#endif

    update_systeminfo();

#ifdef MAINPOWERMETER
    static uint8_t circuitnr = 0;
    int32_t mW;
    int32_t mVA;
    int32_t mA;
    int32_t mV;
    uint8_t nrofsamples;
    circuitspowermeter_read(circuitnr, mW, mVA, mA, mV, 10);
    if (circuitnr == 0) putdatamap("mainsvoltage", String(mV / 1000));
    putdatamap("circuit/" + String((circuitnr + 1)) + "/mA", String(mA));
    putdatamap("circuit/" + String((circuitnr + 1)) + "/W", String(mW / 1000));
    putdatamap("circuit/" + String((circuitnr + 1)) + "/VA", String(mVA / 1000));
    if (circuitnr < 14) circuitnr++;
    else circuitnr = 0;
#endif

    if (wifiTimer < 20) wifiTimer++;


#ifdef DHTPIN
    update_dht();
#endif

#ifdef ONEWIREPIN
    static uint8_t ds18b20timer = 0;
    if (ds18b20timer == 0)
    {
      ds18b20timer = 10;
      DEBUG("Requesting DS18B20 temperatures...\n");
      //oneWireSensors.setWaitForConversion(false);
      oneWireSensors.requestTemperatures();
      float temperature;
#ifdef OPENTHERM
      temperature = oneWireSensors.getTempC(onewire_chReturnWaterThermometer);
      DEBUG("chreturnwatertemp=%f\n", temperature);
      if ((onewire_chReturnWaterEnabled) && (temperature != -127)) putdatamap("ow/ch/returnwatertemperature", String(temperature, 1));
      temperature = oneWireSensors.getTempC(onewire_dcwSupplyWaterThermometer);
      DEBUG("dcwsupplywatertemp=%f\n", temperature);
      if ((onewire_dcwSupplyWaterEnabled) && (temperature != -127)) putdatamap("ow/dcw/temperature", String(temperature, 1));
#endif
#ifdef WEATHER
      temperature = oneWireSensors.getTempC(onewire_OutsideAddress);
      DEBUG("Outside Temperature=%f\n", temperature);
      if (temperature != -127) putdatamap("temperature", String(temperature, 1));

#endif
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
      }

#ifdef NEOPIXELPIN
      neopixelleds.setPixelColor(0, neopixelleds.Color(0, 20, 0));
      neopixelleds.show();
#endif
    }
    else
    {
#ifdef NEOPIXELPIN
      neopixelleds.setPixelColor(0, neopixelleds.Color(30, 0, 0));
      neopixelleds.show();
#endif
      if (flashbuttonstatus == 0)
      {

        /*if (wifiTimer >= 30)
          {
          WiFi.disconnect(false);
          delay(10);
          WiFi.begin();
          wifiTimer = 0;
          }*/
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

#ifdef SONOFFPOW
  // Initialize HLW8012
  // void begin(unsigned char cf_pin, unsigned char cf1_pin, unsigned char sel_pin, unsigned char currentWhen = HIGH, bool use_interrupts = false, unsigned long pulse_timeout = PULSE_TIMEOUT);
  // * cf_pin, cf1_pin and sel_pin are GPIOs to the HLW8012 IC
  // * currentWhen is the value in sel_pin to select current sampling
  // * set use_interrupts to true to use interrupts to monitor pulse widths
  // * leave pulse_timeout to the default value, recommended when using interrupts
  hlw8012.begin(SONOFFPOW_CF_PIN, SONOFFPOW_CF1_PIN, SONOFFPOW_SEL_PIN, SONOFFPOW_CURRENT_MODE, true);

  // These values are used to calculate current, voltage and power factors as per datasheet formula
  // These are the nominal values for the Sonoff POW resistors:
  // * The CURRENT_RESISTOR is the 1milliOhm copper-manganese resistor in series with the main line
  // * The VOLTAGE_RESISTOR_UPSTREAM are the 5 470kOhm resistors in the voltage divider that feeds the V2P pin in the HLW8012
  // * The VOLTAGE_RESISTOR_DOWNSTREAM is the 1kOhm resistor in the voltage divider that feeds the V2P pin in the HLW8012
  hlw8012.setResistors(SONOFFPOW_CURRENT_RESISTOR, SONOFFPOW_VOLTAGE_RESISTOR_UPSTREAM, SONOFFPOW_VOLTAGE_RESISTOR_DOWNSTREAM);

  attachInterrupt(SONOFFPOW_CF1_PIN, hlw8012_cf1_interrupt, FALLING);
  attachInterrupt(SONOFFPOW_CF_PIN, hlw8012_cf_interrupt, FALLING);
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

#ifdef SONOFFPOW
  static unsigned long nextupdatetime = 0;
  if (millis() > nextupdatetime)
  {
    putdatamap("voltage", String(hlw8012.getVoltage()));
    putdatamap("voltage/multiplier", String(hlw8012.getVoltageMultiplier(), 2));
    putdatamap("current", String(hlw8012.getCurrent(), 2));
    putdatamap("current/multiplier", String(hlw8012.getCurrentMultiplier(), 2));
    putdatamap("power/active", String(hlw8012.getActivePower()));
    putdatamap("power/apparent", String(hlw8012.getApparentPower()));
    putdatamap("power/reactive", String(hlw8012.getReactivePower()));
    putdatamap("power/ws", String(hlw8012.getEnergy()));
    putdatamap("power/factor", String(hlw8012.getPowerFactor(), 2));
    putdatamap("power/multiplier", String(hlw8012.getPowerMultiplier(), 2));
    nextupdatetime = millis() + 2000;
  }
#endif
}
#endif


// Publish datamap publishes the datamap one by one to mqtt broker to prevent buffer overflow
void publishdatamap(int32_t packetId, bool publishall)
{
  static uint16_t datamappointer = 0;
  static int32_t nextpacketId = -1;
  static bool waitingforack = false;

  if ((packetId != -1) || publishall) LOG(String("Publishdatamap (") + packetId + "," + publishall + ") datamappointer="+datamappointer+" datamapsize="+dataMap->size()+" nexpacketId="+nextpacketId+" waitingforack="+waitingforack);

  if (publishall) 
  {
    uint32_t publishallpointer = 0;
    while (publishallpointer < dataMap->size())
    {
      String topic = dataMap->getKey(publishallpointer);
      dataMapStruct data = dataMap->getData(publishallpointer);
      data.send = true;
      dataMap->put(topic, data);
      publishallpointer++;
      //DEBUG("publishallpointer=%d datamapsize=%d\n",publishallpointer, dataMap->size());
    }
    datamappointer = 0;
  }

  if (waitingforack)
  {
    if (packetId == 0)
    {
      waitingforack = true;
      // If packetId == 0 resend because packet was not acked
      String topic = dataMap->getKey(datamappointer);
      topic = String("home/" + esp_hostname + "/" + topic);
      dataMapStruct data = dataMap->getData(datamappointer);
      String payload = data.payload;
      nextpacketId = mqttClient.publish(topic.c_str(), 1, true, payload.c_str());
      if (nextpacketId == 0) waitingforack = false;
    }
    if (packetId == nextpacketId) 
    {
      // Packet succesfull delivered proceed to next item
      String topic = dataMap->getKey(datamappointer);
      dataMapStruct data = dataMap->getData(datamappointer);
      data.send = false;
      dataMap->put(topic, data);
      datamappointer++;
      waitingforack = false;
    }
  }

  if (!waitingforack)
  {
    if (datamappointer < dataMap->size())
    {
      nextpacketId = -1;
      while ((datamappointer < dataMap->size()) && (nextpacketId == -1))
      {
        String topic = dataMap->getKey(datamappointer);
        dataMapStruct data = dataMap->getData(datamappointer);
        //DEBUG ("datamappointer=%d datamapsize=%d send=%d\n", datamappointer, dataMap->size(), data.send);
        if (data.send)
        {
          topic = String("home/" + esp_hostname + "/" + topic);
          nextpacketId = mqttClient.publish(topic.c_str(), 1, true, data.payload.c_str());
          if (nextpacketId > 0) waitingforack = true;
          DEBUG("MQTT PUBLISHING DATAMAP %s=%s (nextpacketId=%d)\n", topic.c_str(), data.payload.c_str(), nextpacketId);
        }
        else 
        {
          datamappointer++;    
        }
      }
      if (nextpacketId == -1)
      {
        datamappointer = 0;
        mainstate.mqttready = true;
      }
    }
    else 
    {
      datamappointer = 0;
      mainstate.mqttready = true;
    }
  }
}

#ifdef FLASHBUTTON
void flashbutton_handle()
{
  static uint8_t flashbuttontimer = 0;
  static unsigned long buttonpresstimer = millis();
  if (millis() > buttonpresstimer)
  {
    buttonpresstimer += 1000;
    if ((digitalRead(FLASHBUTTON) == 0) && (flashbuttontimer < (2 ^ (sizeof(flashbuttontimer) * 8))))
    {
      flashbuttontimer++;
      DEBUG("Flash button is pressed, flashbuttontimer=%d\n", flashbuttontimer);
    }
    else
    {
      flashbuttontimer = 0;
    }

    if (flashbuttontimer == 3) // After 3 seconds clear passwords
    {
      flashbuttonstatus = 2;
      esp_password = "esplogin";
      DEBUG("Web Password defaulted to esplogin until reboot!\n");
      mainstate.defaultpassword = true;
    }

    if (flashbuttontimer == 6) // After 6 seconds start access point
    {
      flashbuttonstatus = 1;
      if (!WiFi.softAP(WiFi.hostname().c_str(), "esplogin", 6, 0)) DEBUG("Failed setting WiFi.softAP()");
      WiFi.mode(WIFI_AP_STA);
      DEBUG("Wifi Accesspoint started!\n");
      mainstate.accesspoint = true;
    }

    if (flashbuttontimer == 10) ESP.reset();
  }
}
#endif

uint8_t oldeeprom_read(String * data, byte eepromindex)
{
  DEBUG("%s", String("read_oldeeprom(" + String(eepromindex) + ");\n").c_str());
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

byte oldeeprom_read()
{
  byte eepromindex = 0;
  String data = "";
  while (oldeeprom_read(&data, eepromindex))
  {
    eepromMap->put(eepromindex, data);
    eepromindex++;
  }
  return eepromindex;
}

void eeprom_commit()
{
  DEBUG("commit_eeprom();\n");
  int eeprompointer = 0;
  for (int i = 0; i < eepromMap->size(); i++)
  {
    uint8_t checksum = 20;
    String eepromdata = eepromMap->get(i);
    DEBUG("EEPROM.write(%d,%d) (length)\n", eeprompointer, eepromdata.length() + 1);
    EEPROM.write(eeprompointer++, eepromdata.length() + 1);
    for (int pos = 0; pos < eepromdata.length(); pos++)
    {
      char chr = eepromdata.c_str()[pos];
      DEBUG("EEPROM.write(%d,%c) (data)\n", eeprompointer, chr);
      EEPROM.write(eeprompointer++, chr);
      checksum += chr;
    }
    DEBUG("EEPROM.write(%d,%d) (checksum)\n", eeprompointer, checksum);
    EEPROM.write(eeprompointer++, checksum);
  }
  EEPROM.write(eeprompointer++, 0);
  EEPROM.commit();
}

void eeprom_write(String value, int eepromindex)
{
  DEBUG("eeprom_write %d,%s\n", eepromindex, value.c_str());
  for (int i = 0; i < eepromindex; i++)
  {
    if (!eepromMap->has(i)) eepromMap->put(i, "");
  }
  eepromMap->put(eepromindex, value);
}

void eeprom_init ()
{
  if (oldeeprom_read())
  {
    eeprom_commit();
  }
  eeprom_read();
}

uint8_t eeprom_read()
{
  DEBUG("eeprom_read();\n");
  int eeprompointer = 0;
  int index = 0;
  while (eeprompointer < 512)
  {
    int datasize = EEPROM.read(eeprompointer++);
    DEBUG("datasize=%d\n", datasize);
    if (datasize == 0) return (0);
    byte checksum = 20;
    String data = "";
    for (int pos = 0; pos < datasize - 1; pos++)
    {
      data += String(char(EEPROM.read(eeprompointer)));
      checksum += char(EEPROM.read(eeprompointer++));
    }
    byte eepromchecksum = EEPROM.read(eeprompointer++);
    DEBUG("Read from eeprom %d=%s (%d=%d)\n", eeprompointer, data.c_str(), checksum, eepromchecksum);
    if (eepromchecksum != checksum) return 0;
    eepromMap->put(index++, data);
  }
  return 1;
}

uint8_t eeprom_read(String * data, byte eepromindex)
{
  DEBUG("eeprom_read(%d)\n", eepromindex);
  if (eepromindex >= eepromMap->size()) return 0;
  *data = eepromMap->getData(eepromindex);
  DEBUG("eeprom_read(%d)=%s\n", eepromindex, data->c_str());
  return 1;
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
  static uint8_t ledtimer = 0, ledontime = 1, ledofftime = 1;
  if (mainstate.accesspoint) {
    ledontime = 1;
    ledofftime = 0;
  }
  else if (mainstate.defaultpassword) {
    ledontime = 1;
    ledofftime = 4;
  }
  else if (mainstate.mqttconnected) {
    ledontime = 10;
    ledofftime = 10;
  }
  else if (mainstate.wificonnected) {
    ledontime = 19;
    ledofftime = 1;
  }

#ifdef ESPLED_TIMEOUT_OFF
  // set led off after period of time
  if (uptime > ESPLED_TIMEOUT_OFF) {
    ledontime = 0;
    ledofftime = 1;
  }
#endif

#ifdef ESPLEDINVERSE
  if ((ledtimer == ledontime) && (ledofftime > 0)) digitalWrite(ESPLED, 0);
  if (((ledtimer == 0) && (ledontime > 0)) || (ledofftime == 0)) digitalWrite(ESPLED, 1);
#else
  if ((ledtimer == ledontime) && (ledofftime > 0)) digitalWrite(ESPLED, 1);
  if (((ledtimer == 0) && (ledontime > 0)) || (ledofftime == 0)) digitalWrite(ESPLED, 0);
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
  if (!webserver.authenticate("admin", esp_password.c_str())) {
    return webserver.requestAuthentication();
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
    mqtt_ssl = 0;
    for (uint8_t i = 0; i < webserver.args(); i++)
    {
      if (webserver.argName(i) == "wifissid") postwifissid = webserver.arg(i);
      if (webserver.argName(i) == "wifikey") postwifikey = webserver.arg(i);
      if (webserver.argName(i) == "mqttserver") mqtt_server = webserver.arg(i);
      if (webserver.argName(i) == "mqttusername") mqtt_username = webserver.arg(i);
      if (webserver.argName(i) == "mqttpassword") mqtt_password = webserver.arg(i);
      if (webserver.argName(i) == "mqttmaintopic") mqtt_maintopic = webserver.arg(i);
      if (webserver.argName(i) == "mqttport") mqtt_port = String(webserver.arg(i)).toInt();
      if (webserver.argName(i) == "mqttssl") mqtt_ssl = 1;
      if (webserver.argName(i) == "webpassword") esp_password = webserver.arg(i);
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
    eeprom_write(mqtt_server, 0);
    eeprom_write(mqtt_username, 1);
    eeprom_write(mqtt_password, 2);
    eeprom_write(esp_password, 3);
    eeprom_write(esp_hostname, 4);
    eeprom_write(String(mqtt_port), 5);
    eeprom_write(String(mqtt_ssl), 6);
    eeprom_write(mqtt_maintopic, 7);
    eeprom_commit();


    ArduinoOTA.setPassword(esp_password.c_str());
    ArduinoOTA.setHostname(esp_hostname.c_str());
    Debug.setPassword(esp_password);
    mainstate.defaultpassword = false;

    mqttClient.disconnect(); // Disconnect mqtt server, it will auto reconnect...
    initMqtt();
    WiFi.mode(WIFI_STA); // After saving settings return to wifi client mode and disable AP
    mainstate.accesspoint = false;

    if ((postwifissid != WiFi.SSID()) || (postwifikey != WiFi.psk()) || (esp_hostname != WiFi.hostname()))
    {
      webserver.send(200, "text/html", "<HTML><BODY>Settings Saved.<BR>Please connect to proper wifi network and open the page of the saved hostname.</BODY></HTML>");
      delay(1000);
      WiFi.hostname(esp_hostname);
      WiFi.disconnect(true);
      WiFi.begin(postwifissid.c_str(), postwifikey.c_str()); // Save wifi ssid and key and also activate new hostname...
      flashbuttonstatus = 0;
      previouswifistatus = -1;
    }
    else webserver.send(200, "text/html", "<HTML><BODY>Settings Saved.<BR><A HREF=\"/\">Return to main page</A></BODY></HTML>");
    update_systeminfo(true);
  }
  else
  {
    String wifiselectoptions = "<option value=\"" + WiFi.SSID() + "\" selected>" + WiFi.SSID() + "</option>";
    uint8_t n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i) {
      if (WiFi.SSID(i) != WiFi.SSID()) wifiselectoptions += "<option value=\"" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</option>";
    }


    String webpage = "";
    webpage += "<HTML><HEAD><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></HEAD><BODY><CENTER><div align=\"left\" style=\"width:400px; margin:auto\">";
    webpage += String("<CENTER><H1>") + WiFi.hostname() + "</H1></CENTER><form action=\"/settings\" method=\"post\" autocomplete=\"off\"><TABLE style=\"width:400px; margin:auto\">";
    webpage += String("<TR><TD>Hostname</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"") + String(EEPROMSTRINGSIZE - 2) + "\" name=\"hostname\" value=\"" + WiFi.hostname() + "\"></TD></TR>";
    webpage += String("<TR><TD>WifiSSID</TD><TD><select style=\"width:200\" name=\"wifissid\">") + wifiselectoptions + "</select></TD></TR>";
    webpage += String("<TR><TD>WifiKEY</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"64\" name=\"wifikey\" value=\"") + String(WiFi.psk()) + "\"></TD></TR>";
    webpage += String("<TR><TD>MQTT Server</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"20\" name=\"mqttserver\" value=\"") + mqtt_server + "\"></TD></TR>";
    webpage += String("<TR><TD>MQTT Port</TD><TD><input style=\"width:200\" type=\"number\" maxlength=\"5\" name=\"mqttport\" value=\"") + String(mqtt_port) + "\"></TD></TR>";
    webpage += String("<TR><TD>MQTT Ssl</TD><TD ALIGN=\"left\"><input type=\"checkbox\" name=\"mqttssl\" ") + (mqtt_ssl ? "checked" : "") + "></TD></TR>";
    webpage += String("<TR><TD>MQTT Username</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"20\" name=\"mqttusername\" value=\"") + mqtt_username + "\"></TD></TR>";
    webpage += String("<TR><TD>MQTT Password</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"20\" name=\"mqttpassword\" value=\"") + mqtt_password + "\"></TD></TR>";
    webpage += String("<TR><TD>MQTT Maintopic</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"50\" name=\"mqttmaintopic\" value=\"") + mqtt_maintopic + "\"></TD></TR>";
    webpage += String("<TR><TD>ESP Password</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"20\" name=\"webpassword\" value=\"") + esp_password + "\"></TD></TR>";
#ifdef WATERMETER
    webpage += String("<TR><TD>Watermeter Liter</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"64\" name=\"watermeterliter\" value=\"") + getdatamap("water/liter") + "\"></TD></TR>";
#endif
    webpage += "</TABLE><BR><CENTER><input type=\"submit\" value=\"Save Settings\"></form><BR><BR><form action=\"/settings\" method=\"post\" autocomplete=\"off\"><input type=\"hidden\" name=\"rebootdevice\" value=\"1\"><input type=\"submit\" value=\"Reboot Device\"></form><BR><BR><A HREF=\"/\">Return to main page</A></CENTER></div></BODY></HTML>";
    webserver.send(200, "text/html", webpage);
  }
}

void handleJsonData() {
  String json = "{";
  for (int i = 0; i < dataMap->size(); i++)
  {
    json += "\"" + dataMap->getKey(i) + "\":\"" + dataMap->getData(i).payload + "\",";
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

void amgpelletstovecallback (String topic, String payload)
{
  putdatamap(topic, payload);
}

void logdebug (String message)
{
  DEBUG("%s", message.c_str());
}

void openthermcallback (String topic, String payload)
{
  putdatamap(topic, payload);
}

#ifdef GROWATT
void growattcallback (String topic, String payload)
{
  if (topic == "status")
  {
    pinMode(NODEMCULEDPIN, OUTPUT);
    if (payload == "ready") digitalWrite(NODEMCULEDPIN, 0);
    else digitalWrite(NODEMCULEDPIN, 1);
  }
  putdatamap(topic, payload);
}
#endif
void smartmetercallback (String topic, String payload)
{
  putdatamap(topic, payload);
}

void setup() {

  ESP.wdtDisable(); // Use hardware watchdog of 6 seconds to prevent auto reboot when function takes more time..
  EEPROM.begin(512);

  initSerial();

#ifdef SERIALLOG
  Debug.setSerialEnabled(true);
#else
  Debug.setSerialEnabled(false);
#endif

  Debug.begin("");
  DEBUG("\n\nInitializing ESP8266...\n\n");

  char buffer[25];
  snprintf(buffer, 25, "%08X", ESP.getChipId());
  chipid = buffer;

  sntp_set_timezone(1);
  sntp_setservername(0, "nl.pool.ntp.org");
  // Read settings from EEPROM
  DEBUG("Reading internal EEPROM...\n");
  eeprom_init();
  if (!eeprom_read(&mqtt_server, 0))
  {
    DEBUG("Error reading mqtt server from internal eeprom\n");
  }
  if (!eeprom_read(&mqtt_username, 1))
  {
    DEBUG("Error reading mqtt username from internal eeprom\n");
  }
  if (!eeprom_read(&mqtt_password, 2))
  {
    DEBUG("Error reading mqtt password from internal eeprom\n");
  }
  if (!eeprom_read(&esp_password, 3))
  {
    DEBUG("Error reading web password from internal eeprom\n");
  }
  if (!eeprom_read(&esp_hostname, 4))
  {
    DEBUG("Error reading hostname from internal eeprom\n");
    esp_hostname = ESPNAME;
  }

  String mqttportstr = "";
  if (!eeprom_read(&mqttportstr, 5))
  {
    DEBUG("Error reading mqttport from internal eeprom\n");
  }
  else
  {
    if ((mqttportstr != "") && (mqttportstr.toInt() > 0 ) && (mqttportstr.toInt() < 65536)) mqtt_port = mqttportstr.toInt();
  }

  String mqttsslstr = "";
  if (!eeprom_read(&mqttsslstr, 6))
  {
    DEBUG("Error reading hostname from internal eeprom\n");
  }
  else
  {
    if (mqttsslstr != "") mqtt_ssl = mqttsslstr == "1" ? 1 : 0;
  }

  if (!eeprom_read(&mqtt_maintopic, 7))
  {
    DEBUG("Error reading mqtt main topic from internal eeprom\n");
    mqtt_maintopic = "home/" + esp_hostname + "/";
  }

#ifdef SONOFFCH
  sonoff_init();
#endif
  initWifi();
  initMqtt();
  connectToWifi();

  ArduinoOTA.setHostname(esp_hostname.c_str());

  Debug.begin(esp_hostname.c_str());
  Debug.setPassword(esp_password);
  Debug.setResetCmdEnabled(true);
  Debug.setCallBackProjectCmds(&processCmdRemoteDebug);

  syslog.server(mqtt_server.c_str(), 514);
  syslog.deviceHostname(esp_hostname.c_str());
  syslog.appName(ESPNAME);
  syslog.defaultPriority(LOG_KERN);

  LOG("ESP8266 Started...");
  LOG("Hostname=" + WiFi.hostname());

  ArduinoOTA.setPassword(esp_password.c_str());

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
  i2cEeprom_init(I2C_SDA, I2C_SCL, I2C_EEPROM_ADDRESS, EE24LC512MAXBYTES);
  watermeter_init(WATERPULSEPIN, NODEMCULEDPIN, i2cEeprom_read());
#endif

#ifdef MAINPOWERMETER
  circuitspowermeter_init(ADS0_CS_PIN, ADS0_RDY_PIN, ADS1_CS_PIN, ADS1_RDY_PIN);
#endif

#ifdef ONEWIREPIN
#ifdef OPENTHERM
  if (!oneWireSensors.getAddress(onewire_dcwSupplyWaterThermometer, 0)) {
    DEBUG("Unable to find address for onewire_dcwSupplyWaterThermometer\n");
  }
  else onewire_dcwSupplyWaterEnabled = true;
  if (!oneWireSensors.getAddress(onewire_chReturnWaterThermometer, 1)) {
    DEBUG("Unable to find address for Device onewire_chReturnWaterThermometer\n");

  }
  else onewire_chReturnWaterEnabled = true;
#endif

#ifdef WEATHER
  if (!oneWireSensors.getAddress(onewire_OutsideAddress, 0)) {
    DEBUG("Unable to find address for onewire_outsidetemp\n");
  }
#endif
#endif

  webserver.on("/", handleWWWRoot);
  webserver.on("/data.json", handleJsonData);
  webserver.on("/settings", handleWWWSettings);
  webserver.begin();


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

#ifdef SONOFFBULB
  // MY9291 with 4 channels (like the AiThinker Ai-Light)
  _my92xx = new my92xx(MY92XX_MODEL, MY92XX_CHIPS, MY92XX_DI_PIN, MY92XX_DCKI_PIN, MY92XX_COMMAND_DEFAULT);
  _my92xx->setState(true);
#endif

#ifdef RAINMETERPIN
  pinMode(RAINMETERPIN, INPUT_PULLUP);
#endif

#ifdef NODEMCULEDPIN
  pinMode(NODEMCULEDPIN, OUTPUT);
  digitalWrite(NODEMCULEDPIN, 1);
#endif
}

void processCmdRemoteDebug()
{
  String lastCmd = Debug.getLastCommand();
  String lastArg = "";
  if (lastCmd.indexOf(" ") > 0)
  {
    lastArg = lastCmd.substring(lastCmd.indexOf(" ") + 1);
    lastCmd = lastCmd.substring(0, lastCmd.indexOf(" "));
  }
  DEBUG("[%s]  [%s]\n", lastCmd.c_str(), lastArg.c_str());
  if (lastCmd == "help")
  {
    DEBUG("Available Debug Commands:\n");
    DEBUG("  showdatamap\n");
    DEBUG("  ping [hostname]\n");
    DEBUG("  route\n");
    DEBUG("  mqttforceconnect\n");
    DEBUG("  showmainstate\n");
  }

  if (lastCmd == "ping")
  {
    if (Ping.ping(lastArg.c_str())) {
      DEBUG("Ping Success!!\n");
      } else {
      DEBUG("Ping Error!!\n");
    }
  }

  if (lastCmd == "route")
  {
    String gw = WiFi.gatewayIP().toString();
    DEBUG("%s\n", gw.c_str());
  }

  if (lastCmd == "mqttforceconnect")
  {
    connectToMqtt();
  }

  if (lastCmd == "showmainstate")
  {
    DEBUG("wificonnected=%d\n",mainstate.wificonnected);
    DEBUG("mqttconnected=%d\n",mainstate.mqttconnected);
    DEBUG("mqttready=%d\n",mainstate.mqttready);
    DEBUG("mqttsenddatamap=%d\n",mainstate.mqttsenddatamap);
    DEBUG("defaultpassword=%d\n",mainstate.defaultpassword);
    DEBUG("accesspoint=%d\n",mainstate.accesspoint);
  }

  if (lastCmd == "showdatamap") showdatamap();
#ifdef WATERMETER
  if (lastCmd == "help") DEBUG("  watermeterreadeeprom\n  watermeterwriteeeprom\n");
  if (lastCmd == "watermeterreadeeprom")
  {
    LOG("i2cEeprom read liters=" + String(i2cEeprom_read()));
    watermeter_setliters(i2cEeprom_read());
  }
  if (lastCmd == "watermeterwriteeeprom")
  {
    LOG("i2cEeprom write liters=" + String(watermeter_getliters()));
    i2cEeprom_write(watermeter_getliters());
  }
#endif
}
