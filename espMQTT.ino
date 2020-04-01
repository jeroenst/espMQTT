/*
   Version of esp8266 library: 2.6.2

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
*/

#define DEBUGLEVEL Debug.DEBUG
//#define SYSLOGDEBUG

#ifndef ESPMQTT_BUILDSCRIPT // Only use defines when when firmware is not compiled from the build script...
/* SETTINGS */
//#define SERIALLOG

/* ESP8266 */
// #define ESPMQTT_WEATHER
// #define ESPMQTT_AMGPELLETSTOVE
// #define ESPMQTT_BATHROOM
// #define ESPMQTT_BEDROOM2
// #define ESPMQTT_OPENTHERM
// #define ESPMQTT_SMARTMETER
// #define ESPMQTT_GROWATT
// #define ESPMQTT_SDM120
// #define ESPMQTT_WATERMETER
// #define ESPMQTT_DDNS
#define ESPMQTT_GENERIC8266
// #define ESPMQTT_MAINPOWERMETER
// #define ESPMQTT_OBD2
// #define ESPMQTT_NOISE
// #define ESPMQTT_SOIL
// #define ESPMQTT_DIMMER

/* ESP8285 */
// #define ESPMQTT_ZMAI90
// #define ESPMQTT_DUCOBOX
// #define ESPMQTT_SONOFFS20 // coffeelamp & sonoffs20_00X
// #define ESPMQTT_SONOFFBULB
// #define ESPMQTT_GARDEN //ESP8285 TUIN & MARIANNE & LUIFEL
// #define ESPMQTT_SONOFF_FLOORHEATING
// #define SPMQTT_IRRIGATION
// #define ESPMQTT_BLITZWOLF
// #define ESPMQTT_QSWIFIDIMMERD01
// #define ESPMQTT_QSWIFIDIMMERD02
// #define ESPMQTT_SONOFF4CH //ESP8285
// #define ESPMQTT_SONOFFDUAL
// #define ESPMQTT_SONOFFS20_PRINTER
// #define ESPMQTT_SONOFFPOW
// #define ESPMQTT_SONOFFPOWR2 // tv&washingmachine&server&dishwasher

#define ESPMQTT_VERSION "TEST"
#else
#include "espMQTT_buildscript.h"
#endif


#ifdef  ESPMQTT_SONOFFS20_PRINTER
#define FIRMWARE_TARGET "SONOFFS20_PRINTER"
#define  ESPMQTT_SONOFFS20
#define SONOFFCH_TIMEOUT 1800
uint32_t sonoffch_starttime[1];
static bool sonoffch_timeout_enabled[1] = {1};
#endif

#ifdef  ESPMQTT_SONOFF_FLOORHEATING
#define FIRMWARE_TARGET "SONOFF_FLOORHEATING"
#define  ESPMQTT_SONOFFS20
// Use RX pin for onewire
#define ONEWIREPIN 3
#undef SERIALLOG
#define SONOFF_FLOORHEATING_TEMPMAX 45
#endif

#ifdef  ESPMQTT_DIMMER
#define FIRMWARE_TARGET "DIMMER"
#define TRIAC_PIN D1
#define ZEROCROSS_PIN D2
#define FLASHBUTTON D3
#define ESPLED D4
#include "dimmer.h"
#endif

#ifdef  ESPMQTT_NOISE
#define FIRMWARE_TARGET "NOISE"
#define MICOFFSET 0
#define MICGAIN 1
#define FLASHBUTTON D3
#define ESPLED D4
#endif

#ifdef  ESPMQTT_SDM120
#define FIRMWARE_TARGET "SDM120"
#define FLASHBUTTON D3
#define ESPLED D4
#define NODEMCULEDPIN D0
#undef SERIALLOG
HardwareSerial serSDM(0);
#include <SDM.h>
SDM sdm(serSDM, 2400);
#endif

#ifdef  ESPMQTT_QSWIFIDIMMERD01
#define FIRMWARE_TARGET "QSWIFIDIMMERD01"
#define APONBOOT
#define QSWIFIDIMMERCHANNELS 1
#include "qswifidimmer.h"
#endif

#ifdef  ESPMQTT_QSWIFIDIMMERD02
#define FIRMWARE_TARGET "QSWIFIDIMMERD02"
#define APONBOOT
#define QSWIFIDIMMERCHANNELS 2
#include "qswifidimmer.h"
#endif


#ifdef  ESPMQTT_GENERIC8266
#define FIRMWARE_TARGET "GENERIC8266"
#define FLASHBUTTON D3
#define ESPLED D4
#define SERIALLOG
#endif

#ifdef  ESPMQTT_OBD2
#define FIRMWARE_TARGET "OBD2"
#define FLASHBUTTON D3
#define ESPLED D4
#undef SERIALLOG
#endif

#ifdef  ESPMQTT_AMGPELLETSTOVE
#define FIRMWARE_TARGET "AMGPELLETSTOVE"
#define NODEMCULEDPIN D0
#define FLASHBUTTON D3
#define ESPLED D4
#include "amgpelletstove.h"
#undef SERIALLOG
#endif

#ifdef  ESPMQTT_WEATHER
#define FIRMWARE_TARGET "WEATHER"
#define NODEMCULEDPIN D0
#define FLASHBUTTON D3
#define ESPLED D4
#define ONEWIREPIN D5
#define RAINMETERPIN D1
#define RAINMETERPULSEMM 0.3636
#undef SERIALLOG
#endif

#ifdef  ESPMQTT_GROWATT
#define FIRMWARE_TARGET "GROWATT"
#define NODEMCULEDPIN D0
#define FLASHBUTTON D3
#define ESPLED D4
#define FANPIN D1
#include "growatt.h"
#undef SERIALLOG
#endif

#ifdef  ESPMQTT_SOIL
#define FIRMWARE_TARGET "SOIL"
#define FLASHBUTTON D3
#define ESPLED D4
#endif

#ifdef  ESPMQTT_DUCOBOX
#define FIRMWARE_TARGET "DUCOBOX"
#define  ESPMQTT_SONOFFDUAL
#define FLASHBUTTON 10
#define ESPLED 13
#include "ducobox.h"
#undef SERIALLOG
#endif

#ifdef  ESPMQTT_GARDEN
#define FIRMWARE_TARGET "GARDEN"
#ifndef ARDUINO_ESP8266_ESP01
#error "Wrong board selected! Select Generic ESP8285 module"
#endif
#define  ESPMQTT_SONOFF4CH
#endif

#ifdef  ESPMQTT_IRRIGATION
#define FIRMWARE_TARGET "IRRIGATION"
uint32_t sonoffch_starttime[4];
static bool sonoffch_timeout_enabled[4] = {1, 1, 1, 0};
#define SONOFFCH_TIMEOUT 1800
#ifndef ARDUINO_ESP8266_ESP01
#error "Wrong board selected! Select Generic ESP8285 module"
#endif
#define  ESPMQTT_SONOFF4CH
#endif

#ifdef  ESPMQTT_SONOFF4CH
#ifndef FIRMWARE_TARGET
#define FIRMWARE_TARGET "SONOFF4CH"
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

#ifdef  ESPMQTT_SONOFFDUAL
#ifndef FIRMWARE_TARGET
#define FIRMWARE_TARGET "SONOFFDUAL"
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

#ifdef  ESPMQTT_BLITZWOLF
#define FIRMWARE_TARGET "BLITZWOLF"
#ifndef ARDUINO_ESP8266_ESP01
#error "Wrong board selected! Select Generic ESP8285 module"
#endif
#define FLASHBUTTON 13
#define ESPLED 2
#define SONOFFCH 1

const byte sonoff_relays[1] = {15};
#define SONOFF_LEDS
#define SONOFF_LEDS_INVERSE
const byte sonoff_leds[1] = {0};
const byte sonoff_buttons[1] = {13};
static bool sonoff_oldbuttons[1] = {1};
const bool sonoff_ledinverse = 1;

// HLW8012
#define HLW8012_SEL_PIN 12
#define HLW8012_CF1_PIN 14
#define HLW8012_CF_PIN 5
#define HLW8012_CURRENT_MODE LOW
#define HLW8012_CF_INTERRUPT_EDGE FALLING
#define HLW8012_CF1_INTERRUPT_EDGE FALLING
#define HLW8012_CURRENT_MULTIPLIER 20500 //25740
#define HLW8012_VOLTAGE_MULTIPLIER 327800 //313400
#define HLW8012_POWER_MULTIPLIER 3160000
#endif

#ifdef  ESPMQTT_SONOFFPOW
#define FIRMWARE_TARGET "SONOFFPOW"
#ifndef ARDUINO_ESP8266_ESP01
#error "Wrong board selected! Select Generic ESP8285 module"
#endif
#define FLASHBUTTON 0
#define ESPLED 15
#define SONOFFCH 1

const byte sonoff_relays[1] = {12};
const byte sonoff_buttons[1] = {0};
static bool sonoff_oldbuttons[1] = {1};

// HLW8012
#define HLW8012_SEL_PIN 5
#define HLW8012_CF1_PIN 13
#define HLW8012_CF_PIN 14
#define HLW8012_CURRENT_MODE HIGH
#define HLW8012_CF_INTERRUPT_EDGE FALLING
#define HLW8012_CF1_INTERRUPT_EDGE CHANGE
#define HLW8012_CURRENT_RESISTOR 0.001
#define HLW8012_VOLTAGE_RESISTOR_UPSTREAM ( 5 * 470000 ) // Real: 2280k
#define HLW8012_VOLTAGE_RESISTOR_DOWNSTREAM ( 1000 ) // Real 1.009k
#define HLW8012_CURRENT_MULTIPLIER 16400
#define HLW8012_VOLTAGE_MULTIPLIER 467566.54
#define HLW8012_POWER_MULTIPLIER 24783293
#endif


#ifdef  ESPMQTT_SONOFFPOWR2
#undef SERIALLOG
#define FIRMWARE_TARGET "SONOFFPOWR2"
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

#ifdef  ESPMQTT_SONOFFBULB
// Remember: board: generic esp8266 module, flashmode=dio
#ifndef FIRMWARE_TARGET
#define FIRMWARE_TARGET "SONOFFBULB"
#define APONBOOT 1
#include "my92xx.h"
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


#ifdef  ESPMQTT_SONOFFS20
// Remember: board: generic esp8266 module, flashmode=dio
#ifndef FIRMWARE_TARGET
#define FIRMWARE_TARGET "SONOFFS20"
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

#ifdef  ESPMQTT_MAINPOWERMETER
#define FIRMWARE_TARGET "MAINPOWERMETER"
#define FLASHBUTTON D3
#define ESPLED D4
#define ADS0_RDY_PIN    D0 //ADS1256 data ready
#define ADS0_CS_PIN    D1 //ADS1256 chip select
#define ADS1_RDY_PIN    D2 //ADS1256 data ready
#define ADS1_CS_PIN    D8 //ADS1256 chip select
#include "circuitspowermeter.h"
#endif

#ifdef  ESPMQTT_OPENTHERM
#define FIRMWARE_TARGET "OPENTHERM"
#define FLASHBUTTON D3
#define ESPLED D4
#define ONEWIREPIN D6
#include "opentherm.h"
#undef SERIALLOG
#endif

#ifdef  ESPMQTT_WATERMETER
#define FIRMWARE_TARGET "WATERMETER"
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

#ifdef  ESPMQTT_GARDEN2
#define FIRMWARE_TARGET "GARDEN2"
#define FLASHBUTTON D3
#define ESPLED D4
#define DHTPIN D5
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
#define ONEWIREPIN D2
#endif

#ifdef OLDBATHROOM
#define FIRMWARE_TARGET "OLDBATHROOM"
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

#ifdef  ESPMQTT_BATHROOM
#define FIRMWARE_TARGET "BATHROOM"
#define ESPLED_TIMEOUT_OFF 0 // In sleepingroom we want the led to go off after some time
#define FLASHBUTTON D3
#define ESPLED D4
#define DHTPIN D5
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
#define DHTTEMPOFFSET -2
#define NEOPIXELPIN D6
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel neopixelleds = Adafruit_NeoPixel(2, NEOPIXELPIN, NEO_RGB + NEO_KHZ400);
#endif

#ifdef  ESPMQTT_BEDROOM2
#define FIRMWARE_TARGET "BEDROOM2"
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
#undef SERIALLOG
#endif

#ifdef ESPMQTT_SMARTMETER
#define FIRMWARE_TARGET "SMARTMETER"
#define FLASHBUTTON D3
#define ESPLED D4
#include "smartmeter.h"
#endif

#ifdef  ESPMQTT_DDNS
#include<EasyDDNS.h>
#define FIRMWARE_TARGET "DDNS"
#define NODEMCULEDPIN D0
#define FLASHBUTTON D3
#define ESPLED D4
#endif


#define EEPROMSTRINGSIZE 40 // 2 positions are used, one for 0 character and one for checksum

// ################################################################################################################# END OF DEFINES ####################################################################################################################

#include <time.h>
#include <sys/time.h>                   // struct timeval
#include <coredecls.h>                  // settimeofday_cb()
#include "sntp.h"

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>        // Include the mDNS library
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoJson.h>

#include <EEPROM.h>
#include "espMQTT.h"
#include <AsyncMqttClient.h>
AsyncMqttClient mqttClient;
#include <ArduinoOTA.h>
//#include <user_interface.h>
#include "SimpleMap.h"
#include <Syslog.h>
#define syslogD(fmt, ...) if (WiFi.status() == WL_CONNECTED) syslog.logf(LOG_DEBUG,"(%s) " fmt, __func__, ##__VA_ARGS__)
#define syslogI(fmt, ...) if (WiFi.status() == WL_CONNECTED) syslog.logf(LOG_INFO,"(%s) " fmt, __func__, ##__VA_ARGS__)
#define syslogN(fmt, ...) if (WiFi.status() == WL_CONNECTED) syslog.logf(LOG_NOTICE,"(%s) " fmt, __func__, ##__VA_ARGS__)
#define syslogW(fmt, ...) if (WiFi.status() == WL_CONNECTED) syslog.logf(LOG_WARNING,"(%s) " fmt, __func__, ##__VA_ARGS__)
#define syslogE(fmt, ...) if (WiFi.status() == WL_CONNECTED) syslog.logf(LOG_ERR,"(%s) " fmt, __func__, ##__VA_ARGS__)
#define syslogC(fmt, ...) if (WiFi.status() == WL_CONNECTED) syslog.logf(LOG_CRIT,"(%s) " fmt, __func__, ##__VA_ARGS__)
#define syslogA(fmt, ...) if (WiFi.status() == WL_CONNECTED) syslog.logf(LOG_ALERT,"(%s) " fmt, __func__, ##__VA_ARGS__)
#define syslogEM(fmt, ...) if (WiFi.status() == WL_CONNECTED) syslog.logf(LOG_EMERG,"(%s) " fmt, __func__, ##__VA_ARGS__)

#include <ESP8266Ping.h>

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udpClientSyslog;
WiFiUDP udpClient;

// Create a new empty syslog instance
Syslog syslog(udpClientSyslog, SYSLOG_PROTO_IETF);


#ifdef HLW8012_CF_PIN
#include <HLW8012.h>
#endif

struct Triggers {
  bool wificonnected = false;
  bool wifidisconnected = false;
  bool mqttconnected = false;
  bool mqttdisconnected = false;
  bool wifiscanready = false;
  bool mqttpublished = false;
  bool mqttpublishall = false;
  String firmwareupgrade = "";
} triggers;

struct Mainstate {
  bool wificonnected = false;
  bool mqttconnected = false;
  bool mqttconnectedtrigger = false;
  bool mqttready = false;
  bool mqttsenddatamap = false;
  bool defaultpassword = false;
  bool accesspoint = false;
} mainstate;

struct dataMapStruct {
  String payload = "";
  bool send = true;
  bool onair = false;
};

int wifichannel = 0;
int wifinetworksfound = 0;
uint16_t mqttlastpublishedpacketid = 0;


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


bool updatemqtt = 0;

static const char webpage_P[] PROGMEM = "<!DOCTYPE html><html><meta charset=\"UTF-8\"><meta name=\"google\" content=\"notranslate\"><meta http-equiv=\"Content-Language\" content=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><style>table{width: 400px; margin: auto;}</style></head><body><CENTER><div align='center' style='width:400px; margin:auto'><CENTER><H1><p id='header'></p></H1></CENTER><p id='table'></p><A HREF='settings'>Settings</A></div></CENTER><script>function refreshsite(){var obj,dbParam,xmlhttp,myObj,x,txt ='';xmlhttp=new XMLHttpRequest();xmlhttp.onreadystatechange=function(){if(this.readyState==4&&this.status==200){myObj=JSON.parse(this.responseText);txt+='<TABLE>';for (x in myObj){if(x=='hostname')document.getElementById('header').innerHTML=myObj[x].toUpperCase();txt+='<tr><td>'+x.split('/').join(' ')+'</td><td>'+myObj[x]+'</td></tr>';}txt+='</table>';document.getElementById('table').innerHTML = txt;}};xmlhttp.open('POST','data.json',true);xmlhttp.setRequestHeader('Content-type','application/x-www-form-urlencoded');xmlhttp.send();}refreshsite();window.setInterval(refreshsite, 5000);</script></body></html>";

extern "C" {
  //#include "user_interface.h"
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

#ifdef  ESPMQTT_OPENTHERM
DeviceAddress onewire_chReturnWaterThermometer, onewire_dcwSupplyWaterThermometer;
float onewire_chReturnWaterTemperature = -127, onewire_dcwSupplyWaterTemperature = -127;
float oldonewire_chReturnWaterTemperature = -127, oldonewire_dcwSupplyWaterTemperature = -127;
bool onewire_chReturnWaterEnabled = false, onewire_dcwSupplyWaterEnabled = false;
#endif

#ifdef  ESPMQTT_WEATHER
DeviceAddress onewire_OutsideAddress;
float onewire_chOutsideTemperature = -127;
float oldonewire_chOutsideTemperature = -127;
#endif

#ifdef  ESPMQTT_SONOFF_FLOORHEATING
DeviceAddress onewire_floorWaterAddress;
float onewire_floorWaterTemperature = -127;
bool floorheating_valveon = 0;
#endif
#endif

//WiFiClientSecure wifiClientSecure;
//WiFiClient wifiClient;
static uint8_t wifiTimer = 0;
ESP8266WebServer webserver(80);
#include <WiFiUdp.h>

String chipid;
uint32_t uptime = 0;
bool timertick = 1;
bool timersectick = 1;
String mqtt_server = "";
String mqtt_username = "";
String mqtt_password = "";
String mqtt_topicprefix = "";
int mqtt_port = 1883;
bool mqtt_ssl = 0;
String esp_password = "esplogin";
String esp_hostname = "";
String esp_orig_hostname = "";
//#include "esp8266_peri.h"
RemoteDebug Debug;
String postwifissid = "";
String postwifikey = "";

#include <Ticker.h>
Ticker mqttReconnectTimer;
Ticker wifiReconnectTimer;
Ticker systemTimer;
uint32_t reboottimeout = 0;
uint32_t wifichangesettingstimeout = 0;

#ifdef HLW8012_CF_PIN
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


void publishdatamap(int32_t packetId = -1, bool publishall = false, bool init = false);
void updateexternalip();

String getdatamap(String topic)
{
  return dataMap->get(topic).payload;
}

void showdatamap()
{
  for (int i = 0; i < dataMap->size(); i++)
  {
    DEBUG("%s=%s (send=%d, onair=%d)\n", dataMap->getKey(i).c_str(), dataMap->getData(i).payload.c_str(), dataMap->getData(i).send, dataMap->getData(i).onair);
    yield();
  }
}

void putdatamap(String topic, String value, bool sendupdate = true, bool forcesend = false)
{
  dataMapStruct datamapstruct = dataMap->get(topic);

  if ((datamapstruct.payload != value) || forcesend)
  {
    // Do not output debug for uptime
    if (topic != "system/uptime") DEBUG_D ("DATAMAP %s=%s (sendupdate=%d, oldval=%s oldsend=%d forcesend=%d)\n", topic.c_str(), value.c_str(), sendupdate, datamapstruct.payload.c_str(), datamapstruct.send, forcesend);
    if (topic == "status")
    {
      if (datamapstruct.payload == "upgrading")
      {
        // When upgrading only accept upgradefailed or upgradedone as value
        if ((value != "upgradefailed") && (value != "upgradedone") && (value != "upgradesameversion")) return;
      }
    }
    datamapstruct.onair = false;
    datamapstruct.send = sendupdate;
    datamapstruct.payload = value;
    dataMap->put(topic, datamapstruct);
  }
}

void obd2_writeserial (String data)
{
  DEBUG ("Writing to ODB2:\"%s\"\n", data.c_str());
  data += "\r";
  Serial.write(data.c_str());
}

#ifdef ESPMQTT_OBD2
void obd2_handle()
{
  static unsigned long nextupdatetime = millis() + 10000; // Wait 10 second after startup before sending first cmd...
  static uint8_t obdcmd = 0;
  static unsigned long nextsleeptime = millis() + 60000; // wait 60 seconds after statup before entering sleep mode

  if (nextsleeptime < millis())
  {
    DEBUG("Entering sleep mode\n");
    delay(1000);
    wifi_station_disconnect();
    wifi_set_opmode(NULL_MODE); // set WiFi mode to null mode.
    wifi_fpm_open(); // enable force sleep
    wifi_fpm_set_sleep_type(LIGHT_SLEEP_T); // light sleep
    //wifi_fpm_set_wakeup_cb(fpm_wakup_cb_func1); // Set wakeup callback
    wifi_fpm_do_sleep(60000 * 1000);

    nextsleeptime = millis() + 5000;
  }

  byte serdata = 0;
  if (Serial.available() > 0)
  {
    static String serialstring = "";
    serdata = Serial.read();
    if (((32 <= serdata) && (126 >= serdata)) || (serdata == 13)) // filter bogus data
    {
      //  DEBUG ("%c (%d)\n", serdata, serdata);
      if ('\r' == serdata) // If \r is received it means new data available (/r is ignored for serialstring;
      {
        DEBUG ("Received from OBD2:\"%s\" (obdcmd=%d)\n", serialstring.c_str(), obdcmd);
        String value = "-";
        if (serialstring == "UNABLE TO CONNECT")
        {
          putdatamap ("status", "commerror");
        }
        switch (obdcmd)
        {
          case 0:
            if (serialstring.indexOf("ELM327") == 0)
            {
              serialstring.remove(0, serialstring.indexOf(" v") + 2);
              serialstring.remove(serialstring.indexOf(13));
              DEBUG ("\n\n\nOBD2 Connected! Version=%s\n\n\n", serialstring.c_str());
              putdatamap ("obd/version", serialstring);
              obdcmd = 1;
            }
            break;
          case 1:
            if (serialstring.indexOf("OK") == 0)
            {
              obdcmd = 2;
            }
            break;
          default:
            if (serialstring.indexOf("41 00") == 0)
            {
              int A = strtol(serialstring.substring(6, 8).c_str(), NULL, 16);
              int B = strtol(serialstring.substring(9, 11).c_str(), NULL, 16);
              int C = strtol(serialstring.substring(12, 14).c_str(), NULL, 16);
              int D = strtol(serialstring.substring(15, 17).c_str(), NULL, 16);
              //value = String (A << 24 || B << 16 || C << 8 || D);
              DEBUG ("Supported PIDS= %d %d %d %d", A, B, C, D);
              obdcmd++;
            }

            if (serialstring.indexOf("41 05") == 0)
            {
              value = String(strtol(serialstring.substring(6, 8).c_str(), NULL, 16) - 40);
              putdatamap ("obd/temperature/coolant", value);
              putdatamap ("status", "connected");
              obdcmd++;
            }

            if (serialstring.indexOf("41 0D") == 0)
            {
              value = String(strtol(serialstring.substring(6, 8).c_str(), NULL, 16));
              putdatamap ("obd/speed", value);
              putdatamap ("status", "connected");
              nextsleeptime = millis() + 30000;
              obdcmd++;
            }

            if (serialstring.indexOf("41 0C") == 0)
            {
              int A = strtol(serialstring.substring(6, 8).c_str(), NULL, 16);
              int B = strtol(serialstring.substring(9, 11).c_str(), NULL, 16);
              value = String(((A * 256) + B) / 4);
              putdatamap ("obd/enginerpm", value);
              putdatamap ("status", "connected");
              obdcmd++;
            }

            if (serialstring.indexOf("41 11") == 0)
            {
              value = String((strtol(serialstring.substring(6, 8).c_str(), NULL, 16) * 100) / 255);
              putdatamap ("obd/throttleposition", value);
              putdatamap ("status", "connected");
              obdcmd++;
            }

            if (serialstring.indexOf("41 0F") == 0)
            {
              value = String(strtol(serialstring.substring(6, 8).c_str(), NULL, 16) - 40);
              putdatamap ("obd/temperature/intake", value);
              putdatamap ("status", "connected");
              obdcmd++;
            }

            if (serialstring.indexOf("41 46") == 0)
            {
              value = String(strtol(serialstring.substring(6, 8).c_str(), NULL, 16) - 40);
              putdatamap ("obd/temperature/ambient", value);
              putdatamap ("status", "connected" );
              obdcmd++;
            }
            break;
        }
        serialstring.clear();
      }
      else serialstring += (char)serdata; // filter non ascii bogus and \r
    }
  }

  if (('>' == serdata) || (millis() > nextupdatetime)) // If > is received it means odb is ready for new command
  {
    if (millis() > nextupdatetime) obdcmd = 0;
    nextupdatetime = millis() + 10000;
    switch (obdcmd)
    {
      case 0:
        obd2_writeserial("ATZ");
        break;
      case 1:
        obd2_writeserial("AT SP 0");
        break;
      case 2:
      case 4:
      case 6:
      case 8:
      case 10:
      case 12:
        obd2_writeserial("01 0D"); // Request speed in km/h often because it's important to update fast for cruise control
        break;
      case 3:
        obd2_writeserial("01 00"); // Get supported PID's
        break;
      case 5:
        obd2_writeserial("01 05"); // Request coolant temperature
        break;
      case 7:
        obd2_writeserial("01 0C"); // Request engine RPM
        break;
      case 9:
        obd2_writeserial("01 11"); // Request throttle position
        break;
      case 11:
        obd2_writeserial("01 0F"); // Request air intake temperature
        obdcmd = 2; // next round start with case 2
        break;
      default:
        obd2_writeserial("ATZ");
        obdcmd = 0;
        break;
    }
  }
  serdata = 0;
}
#endif

void update_systeminfo(bool writestaticvalues = false, bool sendupdate = true)
{
  char uptimestr[20];
  sprintf(uptimestr, "%d:%02d:%02d:%02d", uptime / 86400, (uptime / 3600) % 24, (uptime / 60) % 60, uptime % 60);
  if (writestaticvalues)
  {
    putdatamap("hostname", WiFi.hostname(), sendupdate);
    String firmwarename = __FILE__;
    firmwarename = firmwarename.substring(firmwarename.lastIndexOf("/") + 1);
    firmwarename = firmwarename.substring(firmwarename.lastIndexOf("\\") + 1);
    firmwarename = firmwarename.substring(0, firmwarename.lastIndexOf("."));
    putdatamap("firmware/name", firmwarename, sendupdate);
    putdatamap("firmware/target", FIRMWARE_TARGET, sendupdate);
    putdatamap("firmware/sourcefile", String(__FILE__).substring(String(__FILE__).lastIndexOf('/') + 1), sendupdate);
    putdatamap("firmware/version", ESPMQTT_VERSION, sendupdate);
    putdatamap("firmware/compiletime", String(__DATE__) + " " + __TIME__, sendupdate);
    putdatamap("firmware/upgradekey", getRandomString(10));
    putdatamap("status", "online", sendupdate);
    putdatamap("flash/id", String(ESP.getFlashChipId()), sendupdate);
    putdatamap("flash/size/real", String(ESP.getFlashChipRealSize()), sendupdate);
    putdatamap("flash/size/ide", String(ESP.getFlashChipSize()), sendupdate);
    FlashMode_t ideMode = ESP.getFlashChipMode();
    putdatamap("flash/mode", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"), sendupdate);
    putdatamap("flash/speed", String(ESP.getFlashChipSpeed()), sendupdate);
    putdatamap("system/chipid", String(chipid), sendupdate);
  }
  putdatamap("system/uptime", String(uptimestr), uptime % 60 == 0);
  putdatamap("system/freeram", String(system_get_free_heap_size()), uptime % 60 == 0);
  putdatamap("wifi/state", WiFi.status() == WL_CONNECTED ? "connected" : "disconnected", sendupdate);
  putdatamap("wifi/localip", WiFi.localIP().toString(), sendupdate);
  putdatamap("wifi/mac", String(WiFi.macAddress()), sendupdate);
  putdatamap("wifi/ssid", String(WiFi.SSID()), sendupdate);
  putdatamap("wifi/bssid", String(WiFi.BSSIDstr()), sendupdate);
  putdatamap("wifi/rssi", String(WiFi.RSSI()), (((abs(getdatamap("wifi/rssi").toInt() - WiFi.RSSI()) > 5) && (uptime % 10 == 0)) || (uptime % 60 == 0)));
  putdatamap("wifi/channel", String(wifichannel), sendupdate);
  putdatamap("mqtt/server", String(mqtt_server), sendupdate);
  putdatamap("mqtt/port", String(mqtt_port), sendupdate);
  putdatamap("mqtt/ssl", String(mqtt_ssl), sendupdate);
  putdatamap("mqtt/state", mqttClient.connected() ? "connected" : "disconnected", sendupdate);
  putdatamap("mqtt/clientid", String(mqttClient.getClientId()), sendupdate);
}


void initWifi()
{
  WiFi.setAutoReconnect(false); // We handle reconnect our self
  WiFi.setSleepMode(WIFI_NONE_SLEEP); // When sleep is on regular disconnects occur https://github.com/esp8266/Arduino/issues/5083
  WiFi.mode(WIFI_STA);
  WiFi.setOutputPower(20);        // 10dBm == 10mW, 14dBm = 25mW, 17dBm = 50mW, 20dBm = 100mW
  WiFi.hostname(esp_hostname);
  DEBUG_I("Wifi Initialized: Hostname=%s\n", esp_hostname.c_str());
}

void connectToWifi()
{
  if (!mainstate.wificonnected && !mainstate.accesspoint)
  {
    String wifissid = WiFi.SSID();
    String wifipsk =  WiFi.psk();
    DEBUG_I("Connecting to Wi-Fi: SSID=\"%s\"...\n", wifissid.c_str());
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.begin(wifissid.c_str(), wifipsk.c_str(), 0, 0); // First connect to any available AP, later scan for stronger AP
  }
}

void initMqtt()
{
  // Because mqtt lib uses a pointer to const char[], we have to make a static variables
  static String willtopic = String(mqtt_topicprefix) + "status";

  mqttClient.onMessage(onMqttMessage);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setCredentials(mqtt_username.c_str(), mqtt_password.c_str());
  mqttClient.setServer(mqtt_server.c_str(), mqtt_port);
  mqttClient.setWill(willtopic.c_str(), 0, 1, "offline");
  mqttClient.setSecure(mqtt_ssl);
  //#define MQTT_SERVER_FINGERPRINT {0x3F, 0x80, 0xCF, 0x16, 0xD9, 0x43, 0x3B, 0x92, 0xB6, 0x3A, 0x0A, 0x02, 0xFE, 0x27, 0x0B, 0x60, 0xC1, 0x9A, 0x8B, 0xB1}
  //mqttClient.addServerFingerprint((const uint8_t[])MQTT_SERVER_FINGERPRINT);
}

void connectToMqtt()
{
  DEBUG_I("Connecting to Mqtt\n");
  if (!mainstate.mqttconnected)
  {
    mqttClient.disconnect();
    mqttClient.connect();
    mqttReconnectTimer.once(30, connectToMqtt); // retry over 30 seconds if connection can not be established
  }
}

void disconnectMqtt()
{
  DEBUG_W("Disconnecting Mqtt\n");
  mqttReconnectTimer.detach();
  mqttClient.disconnect();
  mainstate.mqttconnected = false;
  mainstate.mqttready = false;
  triggers.mqttdisconnected = true;
}

void onMqttConnect(bool sessionPresent) {
  DEBUG_I("Connected to MQTT sessionPresent=%d\n", sessionPresent);
  mqttReconnectTimer.detach();
  mainstate.mqttconnected = true;
  triggers.mqttconnected = true;
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
  DEBUG_W("Disconnected from MQTT reason:%d\n", reason);

  if (reason == AsyncMqttClientDisconnectReason::TLS_BAD_FINGERPRINT) {
    DEBUG_W("Bad server fingerprint.\n");
  }

  mainstate.mqttconnected = false;
  mainstate.mqttready = false;
  triggers.mqttdisconnected = true;
}

void onMqttUnsubscribe(uint16_t packetId) {
  DEBUG_I ("Unsubscribe acknowledged. packetid=%d\n" + packetId);
}


void mqttdosubscriptions(int32_t packetId = -1)
{
  DEBUG_D("mqttdosubscriptions (%d)\n", packetId);
  static int32_t nextpacketid = -1;
  static uint16_t nextsubscribe = 0;
  static String subscribetopic = ""; // We need this static variable because mqttclient.subscribe uses a pointer

  if (packetId == -1) nextsubscribe = 0;
  if (packetId > 0) nextsubscribe++;
  nextpacketid = -1;
  subscribetopic = "";
  while ((subscribetopic == "") && (nextsubscribe <= 21))
  {
    //DEBUG("mqttdosubscriptions while nextsubscribe=%d\n", nextsubscribe);
    switch (nextsubscribe)
    {
#ifdef ESPMQTT_OPENTHERM
      case 0: subscribetopic = mqtt_topicprefix + "setthermostattemporary"; break;
      case 1: subscribetopic = mqtt_topicprefix + "setthermostatcontinue"; break;
      case 2: subscribetopic = mqtt_topicprefix + "setchwatertemperature"; break;
#endif
#ifdef ESPMQTT_DUCOBOX
      case 3: subscribetopic = mqtt_topicprefix + "setfan"; break;
#endif
#ifdef ESPMQTT_DIMMER
      case 4: subscribetopic = mqtt_topicprefix + "setdimvalue"; break;
      case 5: subscribetopic = mqtt_topicprefix + "setdimstate"; break;
#endif
#ifdef ESPMQTT_SONOFFBULB
      case 6: subscribetopic = mqtt_topicprefix + "setcolor"; break;
#endif
#ifdef ESPMQTT_AMGPELLETSTOVE
      case 7: subscribetopic = mqtt_topicprefix + "setonoff"; break;
      case 8: subscribetopic = mqtt_topicprefix + "setpower"; break;
      case 9: subscribetopic = mqtt_topicprefix + "settemperature"; break;
#endif
#ifdef SONOFFCH
      case 10:  if (0 < SONOFFCH) subscribetopic = mqtt_topicprefix + "setrelay/0"; break;
      case 11:  if (1 < SONOFFCH) subscribetopic = mqtt_topicprefix + "setrelay/1"; break;
      case 12:  if (2 < SONOFFCH) subscribetopic = mqtt_topicprefix + "setrelay/2"; break;
      case 13:  if (3 < SONOFFCH) subscribetopic = mqtt_topicprefix + "setrelay/3"; break;
#endif
#ifdef  ESPMQTT_SONOFF_FLOORHEATING
      case 14: subscribetopic = mqtt_topicprefix + "setvalve"; break;
#endif
#ifdef ESPMQTT_QSWIFIDIMMERD01
      case 15:  subscribetopic = mqtt_topicprefix + "setdimvalue"; break;
      case 16:  subscribetopic = mqtt_topicprefix + "setdimstate"; break;
#endif
#ifdef ESPMQTT_QSWIFIDIMMERD02
      case 17:  subscribetopic = mqtt_topicprefix + "setdimvalue/0"; break;
      case 18:  subscribetopic = mqtt_topicprefix + "setdimvalue/1"; break;
      case 19:  subscribetopic = mqtt_topicprefix + "setdimstate/0"; break;
      case 20:  subscribetopic = mqtt_topicprefix + "setdimstate/1"; break;
#endif
      case 21:  subscribetopic = mqtt_topicprefix + "startfirmwareupgrade"; break;
    }
    if (subscribetopic == "") nextsubscribe++;
  }

  if (subscribetopic == "") triggers.mqttpublishall = true; // When subscibtion has finished start publishing of datamap
  else
  {
    DEBUG("MQTT Subscribing to: %s\n", subscribetopic.c_str());
    nextpacketid = mqttClient.subscribe(subscribetopic.c_str() , 1);
  }
  DEBUG_V("mqttdosubscriptions end nextpacketid=%d\n", nextpacketid);
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
  DEBUG_V ("Subscribe acknowledged packetid=%d qos=%d\n", packetId, qos);
  mqttdosubscriptions(packetId);
}


void onMqttPublish(uint16_t packetId)
{
  triggers.mqttpublished = true;
  mqttlastpublishedpacketid = packetId;
}

void initSerial()
{
  Serial.setRxBufferSize(2048);
#if defined(MH_Z19) || defined( ESPMQTT_OPENTHERM) || defined( ESPMQTT_GROWATT)
  Serial.setDebugOutput(false);
  Serial.begin(9600);  //Init serial 9600 baud
#elif defined ( ESPMQTT_SONOFFPOWR2)
  Serial.setDebugOutput(false);
  Serial.begin(4800, SERIAL_8E1);
#elif defined ( ESPMQTT_AMGPELLETSTOVE)
  Serial.setDebugOutput(false);
#elif defined ( ESPMQTT_SDM120)
  Serial.setDebugOutput(false);
  sdm.begin();
#elif defined ( ESPMQTT_OBD2)
  Serial.setDebugOutput(false);
  Serial.begin(38400, SERIAL_8N1);
#elif defined (ESPMQTT_SMARTMETER) || defined (QSWIFIDIMMERCHANNELS)
  // do nothing, smartmeter initializes serial in init function.
#else
  Serial.begin(115200); //Init serial 115200 baud
#endif

#if defined(MH_Z19)
  //  Serial.swap();
  putdatamap ("mhz19/co2", "-");
  putdatamap ("mhz19/temperature", "-");
#endif
}
#ifdef DHTPIN
void update_dht()
{
  static uint8 errors = 4;
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
    DEBUG_E("Reading DHT22 failed!\n");
    if (errors < 255) errors++;
  }
  else errors = 0;

  DEBUG_I("%s", ("DHT22 temp=" + String(temp, 1) + " rh=" + String(rh, 1) + " hi=" + String(hi, 1) + "\n").c_str());

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
  else if (rh < 90) neopixelleds.setPixelColor(1, neopixelleds.Color(30, 15, 0));
  else neopixelleds.setPixelColor(1, neopixelleds.Color(50, 0, 0));
  neopixelleds.show();
#endif
}
#endif

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties, size_t len, size_t, size_t) {
  String payloadstring = "";
  for (unsigned int i = 0; i < len; i++)
  {
    payloadstring += char(payload[i]);
  }
  String topicstring = topic;
  DEBUG_D("MQTT RECEIVED topic=%s payload=%s\n", topicstring.c_str(), payloadstring.c_str());

#ifdef SONOFFCH
  for (byte i = 0; i < SONOFFCH; i++)
  {
    if (String(topic) == String(mqtt_topicprefix + "setrelay/" + i))
    {
      bool inverse = false;
#ifdef SONOFFCHINVERSE
      inverse = true;
#endif
      if (payloadstring == "0")
      {
        digitalWrite(sonoff_relays[i], inverse ? 1 : 0);
#ifdef SONOFF_LEDS
        digitalWrite(sonoff_leds[i], sonoff_ledinverse ? 1 : 0);
#endif
#ifdef SONOFFCH_TIMEOUT
        sonoffch_starttime[i] = 0;
#endif
      }
      if (payloadstring == "1")
      {
        digitalWrite(sonoff_relays[i], inverse ? 0 : 1);
#ifdef SONOFF_LEDS
        digitalWrite(sonoff_leds[i], sonoff_ledinverse ? 0 : 1);
#endif
#ifdef SONOFFCH_TIMEOUT
        sonoffch_starttime[i] = uptime;
#endif
      }
      updatemqtt = 1;
    }
  }
#endif

#ifdef  ESPMQTT_OPENTHERM
  if (String(topic) == String(mqtt_topicprefix + "setthermostattemporary"))
  {
    DEBUG_I("RECEIVED SETTHERMOSTATTEMPORARY %s\n", payloadstring.c_str());
    opentherm_setthermosttattemporary(payloadstring.toFloat());
  }
  if (String(topic) == String(mqtt_topicprefix + "setthermostatcontinue"))
  {
    DEBUG_I("RECEIVED SETTHERMOSTATCONTINUE %s\n", payloadstring.c_str());
    opentherm_setthermosttatcontinue(payloadstring.toFloat());
  }
  if (String(topic) == String(mqtt_topicprefix + "setchwatertemperature"))
  {
    DEBUG_I("RECEIVED SETCHWATERTEMPERATURE %s\n", payloadstring.c_str());
    opentherm_setchwatertemperature(payloadstring.toFloat());
  }
#endif

#ifdef  ESPMQTT_SONOFF_FLOORHEATING
  if (String(topic) == String(mqtt_topicprefix + "setvalve")) floorheating_valveon = payloadstring == "1" ? true : false;
#endif


#ifdef  ESPMQTT_DUCOBOX
  if (String(topic) == String(mqtt_topicprefix + "setfan")) ducobox_setfan(payloadstring.toInt());
#endif

#ifdef  ESPMQTT_DIMMER
  if (String(topic) == String(mqtt_topicprefix + "setdimvalue"))
  {
    dimmer_setdimvalue(payloadstring.toInt());
    putdatamap ("dimvalue", String(dimmer_getdimvalue()));
  }
#endif

#ifdef  ESPMQTT_SONOFFBULB
  if (String(topic) == String(mqtt_topicprefix + "setcolor"))
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

#ifdef  ESPMQTT_AMGPELLETSTOVE
  amgpelletstove_receivemqtt(topicstring, payloadstring);
#endif

#ifdef  ESPMQTT_QSWIFIDIMMERD01
  if (topicstring == mqtt_topicprefix + "setdimvalue") qswifidimmer_setdimvalue(payloadstring.toInt());
  if (topicstring == mqtt_topicprefix + "setdimstate") qswifidimmer_setdimstate(payloadstring.toInt() ? 1 : 0);
#endif
#ifdef  ESPMQTT_QSWIFIDIMMERD02
  if (topicstring == mqtt_topicprefix + "setdimvalue/0") qswifidimmer_setdimvalue(payloadstring.toInt(), 0);
  if (topicstring == mqtt_topicprefix + "setdimvalue/1") qswifidimmer_setdimvalue(payloadstring.toInt(), 1);
  if (topicstring == mqtt_topicprefix + "setdimstate/0") qswifidimmer_setdimstate(payloadstring.toInt(), 0);
  if (topicstring == mqtt_topicprefix + "setdimstate/1") qswifidimmer_setdimstate(payloadstring.toInt(), 1);
#endif
  if (topicstring == mqtt_topicprefix + "startfirmwareupgrade")
  {
    triggers.firmwareupgrade = payloadstring;
  }
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
#ifdef OLED_ADDRESS
  int lcdline = -9;
  DEBUG_V ("Writing OLED display\n");
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

#ifdef  ESPMQTT_NOISE
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
      DEBUG_D("Analog read=%d, Analog mean=%.0f\n", analogread, analogmean);
      putdatamap ("noise", String(analogmean, 0));
      analogmeanready = 0;
    }
  }
}
#endif


void wifiScanReady(int networksFound)
{
  triggers.wifiscanready = true;
  wifinetworksfound = networksFound;
}

static byte flashbuttonstatus = 0;
static int8_t previouswifistatus = -1;

String doubletostring (double value, int precision)
{
  if (isnan(value)) return "-";
  else return String(value, precision);
}

void loop()
{
  ESP.wdtFeed(); // Prevent HW WD to kick in...
  ArduinoOTA.handle();
  Debug.handle();
  yield();

  if (WiFi.status() == WL_CONNECTED)
  {
    wifichannel = WiFi.channel(); // We can't rely on wifi.channel because while scanning the channel is changed.
    if (!mainstate.wificonnected) triggers.wificonnected = true;
    mainstate.wificonnected = true;
  }
  else
  {
    wifichannel = 0;
    if (mainstate.wificonnected) triggers.wifidisconnected = true;
    mainstate.wificonnected = false;
  }


#ifdef QSWIFIDIMMERCHANNELS
  qswifidimmer_handle();
  yield();
#endif

#ifdef  ESPMQTT_DDNS
  EasyDDNS.update(10000);
#endif

  if ((0 != reboottimeout) && (uptime > reboottimeout))
  {
    ESP.restart();
    delay(1000);
  }

  if ((0 != wifichangesettingstimeout) && (uptime > wifichangesettingstimeout))
  {
    mainstate.accesspoint = false;
    WiFi.mode(WIFI_STA); // After saving settings return to wifi client mode and disable AP
    if ((postwifissid != ""))
    {
      DEBUG_I ("Connecting wifi to %s\n", postwifissid.c_str());
      WiFi.disconnect();
      WiFi.hostname(esp_hostname);
      WiFi.begin(postwifissid.c_str(), postwifikey.c_str()); // Save wifi ssid and key and also activate new hostname...
    }
    wifichangesettingstimeout = 0;
  }


  if (triggers.wificonnected)
  {
    triggers.wificonnected = false;
    DEBUG_I("Connected to WiFi SSID=%s RSSI=%d\n", WiFi.SSID().c_str(), WiFi.RSSI());
    syslogN("Connected to WiFi SSID=%s RSSI=%d\n", WiFi.SSID().c_str(), WiFi.RSSI());
    initMqtt();
    connectToMqtt();
#ifdef SYSLOGDEBUG
    for (int i = 0; i < dataMap->size(); i++)
    {
      syslogD("%s=%s (%d)\n", dataMap->getKey(i).c_str(), dataMap->getData(i).payload.c_str(), dataMap->getData(i).send);
      yield();
    }
#endif
    MDNS.begin(esp_hostname.c_str());
  }

  if (triggers.wifidisconnected)
  {
    triggers.wifidisconnected = false;
    DEBUG_W("Disconnected from Wi-Fi.\n");
    disconnectMqtt();
    if (!mainstate.accesspoint) wifiReconnectTimer.once(2, connectToWifi); // trying to connect to wifi can cause AP to fail
  }

  if (triggers.mqttconnected)
  {
    triggers.mqttconnected = false;
    DEBUG_I("Connected to MQTT Server=%s\n", mqtt_server.c_str());
    syslogN("Connected to MQTT Server=%s\n", mqtt_server.c_str());
    yield(); // Prevent crash because of to many debug data to send
    update_systeminfo(true);
    mqttdosubscriptions();
    updateexternalip();
  }

  if (triggers.mqttdisconnected)
  {
    triggers.mqttdisconnected = false;
    DEBUG_W("Disconnected from MQTT Server=%s\n", mqtt_server.c_str());
    syslogN("Disconnected from MQTT Server=%s\n", mqtt_server.c_str());
    yield(); // Prevent crash because of to many debug data to send
    if (WiFi.isConnected()) {
      mqttReconnectTimer.once(5, connectToMqtt);
    }
  }

  if (triggers.mqttpublished)
  {
    triggers.mqttpublished = false;
    DEBUG_V ("Publish acknowledged packetid=%d\n", mqttlastpublishedpacketid);
    publishdatamap(mqttlastpublishedpacketid);

  }

  if (triggers.mqttpublishall)
  {
    triggers.mqttpublishall = false;
    publishdatamap(-1, true, true);
  }

  if (mainstate.mqttready) publishdatamap();

  if (triggers.firmwareupgrade != "")
  {
    // wait 5 seconds before upgrading
    static uint64_t waitbeforeupgrade = 0;
    if (waitbeforeupgrade == 0)
    {
      DEBUG_I ("Received startfirmwareupgrade, upgrade pending...\n");
      putdatamap("status", "upgrading");
      waitbeforeupgrade = uptime + 5;
    }
    else if (waitbeforeupgrade < uptime)
    {
      waitbeforeupgrade = 0;

      StaticJsonDocument<300> JSONdoc;
      auto error = deserializeJson(JSONdoc, triggers.firmwareupgrade.c_str());
      triggers.firmwareupgrade = "";

      if (!error)
      {
        // The upgradekey is mainly to prevent upgrading over and over when a retained upgrade message is retained in mqtt
        String upgradekey = JSONdoc["key"];
        String upgradeurl = JSONdoc["url"];
        String upgradeversion = JSONdoc["version"];

        DEBUG_I ("Starting firmware upgrade\n upgradeversion=%s\n upgradeurl=%s\n upgradekey=%s\n", upgradeversion.c_str(), upgradeurl.c_str(), upgradekey.c_str());

        if (upgradeversion == ESPMQTT_VERSION)
        {
          DEBUG_I ("Upgrade canceled, version is the same\n");
          putdatamap("status", "upgradesameversion");
        }
        else if (getdatamap("firmware/upgradekey") != upgradekey)
        {
          DEBUG_I ("Upgrade canceled, upgradekey is incorrect\n");
          putdatamap("status", "upgradefailed");
        }
        else
        {
          DEBUG_I ("Starting upgrade from:%s\n", upgradeurl.c_str());
          WiFiClient upgradeclient;
          t_httpUpdate_return ret = ESPhttpUpdate.update(upgradeclient, upgradeurl, ESPMQTT_VERSION);
          switch (ret)
          {
            case HTTP_UPDATE_FAILED:
              DEBUG_E("Firmware upgrade failed: %s.\n", ESPhttpUpdate.getLastErrorString().c_str());
              putdatamap("status", "upgradefailed");
              break;
            case HTTP_UPDATE_NO_UPDATES:
              DEBUG_E("Firmware upgrade check finished, no new version available.");
              putdatamap("status", "upgradefailed");
              break;
            case HTTP_UPDATE_OK:
              DEBUG_E("Firmware upgrade done!\n"); // may not be called since we reboot the ESP
              putdatamap("status", "upgradedone");
              break;
          }
        }
      }
    }
  }

  if (triggers.wifiscanready)
  {
    triggers.wifiscanready = false;
    int strongestwifiid = -1;
    int strongestwifirssi = -1000;
    int currentwifirssi = -1000;
    int currentwifiid = -1;

    for (int i = 0; i < wifinetworksfound; i++)
    {
      if (WiFi.SSID(i) == WiFi.SSID())
      {
        if (strongestwifirssi < WiFi.RSSI(i))
        {
          strongestwifiid = i;
          strongestwifirssi = WiFi.RSSI(i);
        }
        if (WiFi.BSSIDstr(i) == WiFi.BSSIDstr())
        {
          currentwifirssi = WiFi.RSSI(i);
          currentwifiid = i;
        }
      }

      String enctype = "None";
      switch (WiFi.encryptionType(i)) {
        case ENC_TYPE_WEP:
          enctype = "WEP";
          break;
        case ENC_TYPE_TKIP:
          enctype = "WPA";
          break;
        case ENC_TYPE_CCMP:
          enctype = "WPA2";
          break;
        case ENC_TYPE_NONE:
          enctype = "None";
          break;
        case ENC_TYPE_AUTO:
          enctype = "Auto";
          break;
      }


      DEBUG_V(" %d: %s, %s, Ch:%d (%ddBm) %s\n", i, WiFi.SSID(i).c_str(), WiFi.BSSIDstr(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), enctype.c_str());
      yield(); // Prevent crash because of to many debug data to send
    }

    DEBUG_D("CurrentAp ID=%d SSID=%s BSSID=%s RSSI=%d(%d), Strongest AP ID=%d SSID=%s, BSSID=%s RSSI=%d(%d)\n", currentwifiid, WiFi.SSID().c_str(), WiFi.BSSIDstr().c_str(), currentwifirssi, WiFi.RSSI(), strongestwifiid, WiFi.SSID(strongestwifiid).c_str(), WiFi.BSSIDstr(strongestwifiid).c_str(), WiFi.RSSI(strongestwifiid), strongestwifirssi);
    yield(); // Prevent crash because of to many debug data to send

    if (!mainstate.accesspoint)
    {
      if ((strongestwifiid >= 0) && ((WiFi.RSSI() >= 0) || (currentwifiid == -1) || ((currentwifiid != strongestwifiid) && (currentwifirssi + 10 < strongestwifirssi))))
      {
        DEBUG_I ("Switching to stronger AP %d (%s, %s, %s)\n", strongestwifiid, WiFi.SSID().c_str(), WiFi.psk().c_str(), WiFi.BSSIDstr(strongestwifiid).c_str());
        String wifissid = WiFi.SSID();
        String wifipsk =  WiFi.psk();
        WiFi.disconnect();
        WiFi.begin(wifissid.c_str(), wifipsk.c_str(), WiFi.channel(strongestwifiid), WiFi.BSSID(strongestwifiid));
      }
    }
  }

  webserver.handleClient();

#ifdef SONOFFCH
  sonoff_handle();
  yield();
#endif

#ifdef  ESPMQTT_NOISE
  handle_noise();
  yield();
#endif

#ifdef  ESPMQTT_DUCOBOX
  ducobox_handle();
  yield();
#endif

#ifdef  ESPMQTT_DIMMER
  dimmer_handle();
  yield();
#endif

#ifdef  ESPMQTT_GENERIC8266
#endif

#ifdef RAINMETERPIN
  static uint32_t rainpinmillis = 0;
  static uint32_t rainpulses = 0;
  static uint32_t rainpulsesminute = 0;
  static uint32_t rainpulseshour = 0;
  bool rainpinstate = 0;
  static bool count = 0;
  rainpinstate = digitalRead(RAINMETERPIN);
  if ((rainpinstate == 1) && (millis() - 50 > rainpinmillis) && count) // Pulse has to settle for 50ms before counting
  {
    rainpulses++;
    rainpulseshour++;
    rainpulsesminute++;
    String mqtttopic = String(mqtt_topicprefix + "rain/pulse");
    mqttClient.publish(mqtttopic.c_str(), 0, false, "1");
    count = 0;
  }
  if (rainpinstate == 0)
  {
    rainpinmillis = millis();
    count = 1;
  }
  putdatamap ("rain/pulses", String(rainpulses));
  putdatamap ("rain/mm", String((double(rainpulses)*RAINMETERPULSEMM), 1));

  static bool hourreg = false;
  static bool firsthourreg = true;
  if ((uptime % 3600) || (firsthourreg))
  {
    putdatamap ("rain/hour/pulses", String(rainpulseshour));
    putdatamap ("rain/hour/mm", String((double(rainpulseshour)*RAINMETERPULSEMM), 1));
    firsthourreg = false;
    if (!hourreg)
    {
      putdatamap ("rain/lasthour/pulses", String(rainpulseshour));
      putdatamap ("rain/lasthour/mm", String((double(rainpulseshour)*RAINMETERPULSEMM), 1));
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
    putdatamap ("rain/minute/mm", String((double(rainpulsesminute)*RAINMETERPULSEMM), 1));
    firstminreg = false;
    if (!minreg)
    {
      putdatamap ("rain/lastminute/pulses", String(rainpulsesminute));
      putdatamap ("rain/lastminute/mm", String((double(rainpulsesminute)*RAINMETERPULSEMM), 1));
      rainpulsesminute = 0;
      minreg = true;
    }
  }
  else minreg = false;

  digitalWrite(NODEMCULEDPIN, rainpinstate);
  yield();
#endif


#ifdef  ESPMQTT_WATERMETER
  static uint32_t watermeter_liters = watermeter_getliters();
  if (watermeter_handle())
  {
    putdatamap("water/lmin", String(watermeter_getflow(), 1));
    putdatamap("water/m3h", String(watermeter_getflow() * 0.06, 3));

    if (watermeter_liters != watermeter_getliters())
    {
      watermeter_liters = watermeter_getliters();
      i2cEeprom_write(watermeter_liters);
      syslogN("Watermeter Liters Changed=%d\n", watermeter_liters);
      putdatamap("water/liter", String(watermeter_liters));
      putdatamap("water/m3", String((double(watermeter_liters) / 1000), 3));
    }
  }
  yield();
#endif

#ifdef  ESPMQTT_OPENTHERM
  opentherm_handle();
  yield();
#endif

#ifdef  ESPMQTT_SMARTMETER
  smartmeter_handle();
  yield();
#endif

#ifdef  ESPMQTT_GROWATT
  growatt_handle();
  yield();
#endif

#ifdef FLASHBUTTON
  flashbutton_handle();
  yield();
#endif

#ifdef  ESPMQTT_AMGPELLETSTOVE
  amgpelletstove_handle();
  yield();
#endif

#ifdef ESPMQTT_OBD2
  obd2_handle();
  yield();
#endif

#ifdef  ESPMQTT_SONOFFPOWR2
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
  yield();
#endif

#ifdef MH_Z19
  static int mhz19ppm = 0;
  static int mhz19temp = 0;
  switch (MHZ19_handle(&mhz19ppm, &mhz19temp))
  {
    case 1:
      putdatamap ("mhz19/co2", String(mhz19ppm));
      putdatamap ("mhz19/temperature", String(mhz19temp));
      break;
    case -1:
      putdatamap ("mhz19/co2", "-");
      putdatamap ("mhz19/temperature", "-");
      break;
    default:
      break;
  }
  yield();
#endif

  if (timertick == 1) // Every 0.1 second read next SDM120 register
  {
    //    Serial.print(".");
    timertick = 0;
#ifdef  ESPMQTT_SDM120
    static uint8_t sdmreadcounter = 1;
    double value = NAN;

    switch (sdmreadcounter)
    {
      case 1:
        putdatamap("status", "querying");
        putdatamap("voltage", String(sdm.readVal(SDM120CT_VOLTAGE), 2));
        break;
      case 2:
        putdatamap("current", String(sdm.readVal(SDM120CT_CURRENT), 2));
        break;
      case 3:
        putdatamap("power", String(sdm.readVal(SDM120CT_POWER), 2));
        break;
      case 4:
        putdatamap("power/apparant", String(sdm.readVal(SDM120CT_APPARENT_POWER), 2));
        break;
      case 5:
        putdatamap("power/reactive", String(sdm.readVal(SDM120CT_REACTIVE_POWER), 2));
        break;
      case 6:
        putdatamap("frequency", String(sdm.readVal(SDM120CT_FREQUENCY), 2));
        break;
      case 7:
        putdatamap("powerfactor", String(sdm.readVal(SDM120CT_POWER_FACTOR), 2));
        break;
      case 8:
        putdatamap("energy/active/import", String(sdm.readVal(SDM120CT_IMPORT_ACTIVE_ENERGY), 3));
        break;
      case 9:
        putdatamap("energy/active/export", String(sdm.readVal(SDM120CT_EXPORT_ACTIVE_ENERGY), 3));
        break;
      case 10:
        putdatamap("energy/active", String(sdm.readVal(SDM120CT_TOTAL_ACTIVE_ENERGY), 3));
        break;
      case 11:
        putdatamap("energy/reactive/import", String(sdm.readVal(SDM120CT_IMPORT_REACTIVE_ENERGY), 3));
        break;
      case 12:
        putdatamap("energy/reactive/export", String(sdm.readVal(SDM120CT_EXPORT_REACTIVE_ENERGY), 3));
        break;
      case 13:
        value = sdm.readVal(SDM120CT_TOTAL_REACTIVE_ENERGY);
        putdatamap("energy/reactive", doubletostring(value, 3));
        if (isnan(value)) putdatamap("status", "commerror");
        else putdatamap("status", "ready");
        break;
      case 14:
        if (uptime % 10) sdmreadcounter = 0;
        break;
    }
    if (sdmreadcounter < 14)
    {
      digitalWrite(NODEMCULEDPIN, isnan(value) ? 1 : 0);
      sdmreadcounter++;
    }
    yield();
#endif
  }

  if (timersectick == 1) // Every 1 second check sensors and update display (it would be insane to do it more often right?)
  {
    timersectick = 0;
    updatemqtt = 1;

    // scan for stronger wifi network: every 10 minutes, directly when wifi is not connected or every 30 seconds if signal is bad
    // try to do this as less as posisble because during scan the esp is unreachable for about a second.
    if ((!mainstate.accesspoint))
    {
      if (((uptime % 600) == 0) || ((WiFi.status() != WL_CONNECTED) && (uptime % 30)) || (((uptime % 30) == 0) && WiFi.RSSI() < -70))
      {
        static uint32_t wifilastscan = 0;
        // prevent scanning more than once per 30 seconds and wait 2 seconds before first scan
        if (((uptime > 10) && (wifilastscan == 0)) || ((wifilastscan + 30 < uptime)))
        {
          DEBUG_D("Starting Wifi Scan...\n");
          WiFi.scanNetworksAsync(wifiScanReady);
          wifilastscan = uptime;
        }
      }
    }

    if ((uptime % 600) == 0)
    {
      updateexternalip();
    }

    if ((uptime % 60) == 0)
    {
      char uptimestr[20];
      sprintf(uptimestr, "%d:%02d:%02d:%02d", uptime / 86400, (uptime / 3600) % 24, (uptime / 60) % 60, uptime % 60);
      time_t now;
      time(&now);
      String strtime = ctime(&now);
      strtime.replace("\n", "");
      if (mainstate.wificonnected)
      {
        syslogI("Uptime=%s DateTime=%s\n", uptimestr, strtime.c_str());
      }
      DEBUG_I("Uptime=%s DateTime=%s\n", uptimestr, strtime.c_str());
      yield();
    }


#ifdef  ESPMQTT_SONOFFPOWR2
    if ((uptime % 5) == 0) // Every 5 seconds send update about power usage
    {
      putdatamap("voltage", String(voltval, 1));
      putdatamap("current", String(currentval, 3));
      putdatamap("power", String(powerval, 1));
    }
#endif

#ifdef APONBOOT
    if ((uptime == 30) && (WiFi.status() != WL_CONNECTED))
    {
      DEBUG_W("Connection to wifi failed, starting accesspoint\n");
      mainstate.accesspoint = true;
      if (!WiFi.softAP(WiFi.hostname().c_str(), "esplogin", 6, 0)) DEBUG_E("Failed setting WiFi.softAP()");
      static uint8 routermode = 0;
      wifi_softap_set_dhcps_offer_option(OFFER_ROUTER, &routermode);
      WiFi.mode(WIFI_AP);
      esp_password = "esplogin";
    }
    if (uptime == 630)
    {
      DEBUG_I("Stopping accesspoint");
      WiFi.mode(WIFI_STA);
      mainstate.accesspoint = false;
    }
#endif

    // Every 10 seconds update system info
    if ((uptime % 10) == 0) update_systeminfo();

#ifdef  ESPMQTT_MAINPOWERMETER
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
    if (uptime % 5 == 0) update_dht();
#endif

#ifdef ONEWIREPIN
    if (uptime % 10 == 0)
    {
      DEBUG_V("Requesting DS18B20 temperatures...\n");
      oneWireSensors.requestTemperatures();
      float temperature;
#ifdef  ESPMQTT_OPENTHERM
      temperature = oneWireSensors.getTempC(onewire_chReturnWaterThermometer);
      DEBUG_I("chreturnwatertemp=%f\n", temperature);
      if ((onewire_chReturnWaterEnabled) && (temperature != -127)) putdatamap("ow/ch/returnwatertemperature", String(temperature, 1));
      else putdatamap("ow/ch/returnwatertemperature", "-");
      temperature = oneWireSensors.getTempC(onewire_dcwSupplyWaterThermometer);
      DEBUG_I("dcwsupplywatertemp=%f\n", temperature);
      if ((onewire_dcwSupplyWaterEnabled) && (temperature != -127)) putdatamap("ow/dcw/temperature", String(temperature, 1));
      else putdatamap("ow/dcw/temperature", "-");
      yield();
#endif
#ifdef  ESPMQTT_WEATHER
      temperature = oneWireSensors.getTempC(onewire_OutsideAddress);
      DEBUG_I("Outside Temperature=%f\n", temperature);
      if (temperature != -127) putdatamap("temperature", String(temperature, 1));
      else putdatamap("temperature", "-");
      yield();
#endif
#ifdef  ESPMQTT_SONOFF_FLOORHEATING
      temperature = oneWireSensors.getTempC(onewire_floorWaterAddress);
      DEBUG_I("Floor Water Temperature=%f\n", temperature);
      bool floorheatingrelayon = false;
      // When temperature exeeds maximum prevent pump from switching on
      if (temperature < SONOFF_FLOORHEATING_TEMPMAX)
      {
        // Every 24 hours run the pump for 1 minute to prevent locking
        if ((int)(uptime / 60) % 1440 == 0)
        {
          floorheatingrelayon = true;
        }
        // Only if floorheating temperature as measured correctly put valve on if requested
        else if (temperature != -127)
        {
          putdatamap("temperature", String(temperature, 1));
          floorheatingrelayon = floorheating_valveon;
        }
        else putdatamap("temperature", "-");
      }
#ifdef SONOFFCHINVERSE
      digitalWrite(sonoff_relays[0], floorheatingrelayon ? false : true); // Set floorheating
#else
      digitalWrite(sonoff_relays[0], floorheatingrelayon); // Set floorheating
#endif
#ifdef SONOFF_LEDS
      digitalWrite(sonoff_leds[0], sonoff_ledinverse == floorheatingrelayon ? 0 : 1);
#endif
      yield();
#endif
    }
#endif

    write_oled_display();

    if (WiFi.status() == WL_CONNECTED)
    {
      wifiTimer = 0;
      if (WiFi.status() != previouswifistatus)
      {
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
#ifdef SONOFF_LEDS
    digitalWrite(sonoff_leds[i], sonoff_ledinverse ? 1 : 0);
    pinMode(sonoff_leds[i], OUTPUT);
#endif
    pinMode(sonoff_buttons[i], INPUT_PULLUP);
  }

#ifdef SONOFFCH_TIMEOUT
  for (uint8_t i = 0; i < SONOFFCH; i++) sonoffch_starttime[i] = 0;
#endif

#ifdef HLW8012_CF_PIN


  // Initialize HLW8012
  // void begin(unsigned char cf_pin, unsigned char cf1_pin, unsigned char sel_pin, unsigned char currentWhen = HIGH, bool use_interrupts = false, unsigned long pulse_timeout = PULSE_TIMEOUT);
  // * cf_pin, cf1_pin and sel_pin are GPIOs to the HLW8012 IC
  // * currentWhen is the value in sel_pin to select current sampling
  // * set use_interrupts to true to use interrupts to monitor pulse widths
  // * leave pulse_timeout to the default value, recommended when using interrupts
  hlw8012.begin(HLW8012_CF_PIN, HLW8012_CF1_PIN, HLW8012_SEL_PIN, HLW8012_CURRENT_MODE, true);

  // These values are used to calculate current, voltage and power factors as per datasheet formula
  // These are the nominal values for the Sonoff POW resistors:
  // * The CURRENT_RESISTOR is the 1milliOhm copper-manganese resistor in series with the main line
  // * The VOLTAGE_RESISTOR_UPSTREAM are the 5 470kOhm resistors in the voltage divider that feeds the V2P pin in the HLW8012
  // * The VOLTAGE_RESISTOR_DOWNSTREAM is the 1kOhm resistor in the voltage divider that feeds the V2P pin in the HLW8012
#ifdef HLW8012_CURRENT_RESISTOR
  hlw8012.setResistors(HLW8012_CURRENT_RESISTOR, HLW8012_VOLTAGE_RESISTOR_UPSTREAM, HLW8012_VOLTAGE_RESISTOR_DOWNSTREAM);
#endif

#ifdef HLW8012_CURRENT_MULTIPLIER
  hlw8012.setCurrentMultiplier(HLW8012_CURRENT_MULTIPLIER);
#endif
#ifdef HLW8012_VOLTAGE_MULTIPLIER
  hlw8012.setVoltageMultiplier(HLW8012_VOLTAGE_MULTIPLIER);
#endif
#ifdef HLW8012_POWER_MULTIPLIER
  hlw8012.setPowerMultiplier(HLW8012_POWER_MULTIPLIER);
#endif

  attachInterrupt(HLW8012_CF1_PIN, hlw8012_cf1_interrupt, HLW8012_CF1_INTERRUPT_EDGE);
  attachInterrupt(HLW8012_CF_PIN, hlw8012_cf_interrupt, HLW8012_CF_INTERRUPT_EDGE);
#endif

}

void sonoff_handle()
{
#ifdef SONOFFCH_TIMEOUT
  for (uint8_t i = 0; i < SONOFFCH; i++)
  {
    if ((sonoffch_starttime[i] > 0) && (sonoffch_starttime[i] + SONOFFCH_TIMEOUT < uptime) && (sonoffch_timeout_enabled[i]))
    {
      sonoffch_starttime[i] = 0;
      bool inverse = false;
#ifdef SONOFFCHINVERSE
      inverse = true;
#endif
      digitalWrite(sonoff_relays[i], inverse ? 1 : 0);
#ifdef SONOFF_LEDS
      digitalWrite(sonoff_leds[i], sonoff_ledinverse ? 1 : 0);
#endif
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
          DEBUG_I ("SONOFF BUTTON %d PRESSED\n", i);
          digitalWrite(sonoff_relays[i], digitalRead(sonoff_relays[i]) ? 0 : 1);
#ifdef SONOFF_LEDS
          digitalWrite(sonoff_leds[i], (digitalRead(sonoff_relays[i]) ? 1 : 0) == sonoff_ledinverse ? 0 : 1);
#endif
          bool inverse = false;
#ifdef SONOFFCHINVERSE
          inverse = true;
#endif
#ifdef SONOFFCH_TIMEOUT
          if (digitalRead(sonoff_relays[i]) != inverse) sonoffch_starttime[i] = uptime;
          else sonoffch_starttime[i] = 0;
#endif
#ifdef  ESPMQTT_SONOFF_FLOORHEATING
          if (i == 0) floorheating_valveon = digitalRead(sonoff_relays[i]) != inverse ? 1 : 0;
#endif
          DEBUG_I ("SONOFF RELAY IS %s\n", digitalRead(sonoff_relays[i]) != inverse ? "ON" : "OFF");
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

#ifdef HLW8012_CF_PIN
  static unsigned long nextupdatetime = 0;
  if (millis() > nextupdatetime)
  {
    putdatamap("voltage", String(hlw8012.getVoltage()));
    putdatamap("voltage/multiplier", String(hlw8012.getVoltageMultiplier()));
    putdatamap("current", String(hlw8012.getCurrent(), 3));
    putdatamap("current/multiplier", String(hlw8012.getCurrentMultiplier()));
    putdatamap("power", String(hlw8012.getActivePower()));
    putdatamap("power/apparent", String(hlw8012.getApparentPower()));
    putdatamap("power/reactive", String(hlw8012.getReactivePower()));
    putdatamap("power/factor", String(hlw8012.getPowerFactor(), 2));
    putdatamap("power/multiplier", String(hlw8012.getPowerMultiplier()));
    putdatamap("energy/ws", String(hlw8012.getEnergy()));
    putdatamap("energy/kwh", String(hlw8012.getEnergy() / 3600000, 3));
    nextupdatetime = millis() + 5000;
  }
#endif
}
#endif


// Publish datamap publishes the datamap one by one to mqtt broker to prevent buffer overflow
void publishdatamap(int32_t packetId, bool publishall, bool init)
{
  static uint16_t datamappointer = 0;
  static int32_t nextpacketId = -1;
  static bool waitingforack = false;

  if (init)
  {
    waitingforack = false;
    datamappointer = 0;
    nextpacketId = -1;
  }

  if ((packetId != -1) || publishall) DEBUG_V("Publishdatamap packetId=%d publishall=%d datamappointer=%d datamapsize=%d nextpacketid=%d waitingforack=%d\n", packetId, publishall, datamappointer, dataMap->size(), nextpacketId, waitingforack);

  if (publishall)
  {
    int32_t publishallpointer = 0;
    while (publishallpointer < dataMap->size())
    {
      String topic = dataMap->getKey(publishallpointer);
      dataMapStruct data = dataMap->getData(publishallpointer);
      data.onair = false;
      data.send = true;
      dataMap->put(topic, data);
      publishallpointer++;
      //DEBUG("publishallpointer=%d datamapsize=%d\n",publishallpointer, dataMap->size());
    }
    datamappointer = 0;
  }

  if (mqttClient.connected())
  {

    if (waitingforack)
    {
      if (packetId == 0)
      {
        // If packetId == 0 resend because packet was not acked
        waitingforack = true;
        String topic = dataMap->getKey(datamappointer);
        String sendtopic = String("home/" + esp_hostname + "/" + topic);
        dataMapStruct data = dataMap->getData(datamappointer);
        String payload = data.payload;
        nextpacketId = mqttClient.publish(sendtopic.c_str(), 1, true, payload.c_str());
        if (nextpacketId == 0) waitingforack = false;
        else
        {
          data.onair = true;
          dataMap->put(topic, data);
        }
      }
      if (packetId == nextpacketId)
      {
        // Packet succesfull delivered proceed to next item
        String topic = dataMap->getKey(datamappointer);
        dataMapStruct data = dataMap->getData(datamappointer);
        if (data.onair)
        {
          data.send = false;
          data.onair = false;
          dataMap->put(topic, data);
        }
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
            String sendtopic = String(mqtt_topicprefix + topic);
            nextpacketId = mqttClient.publish(sendtopic.c_str(), 1, true, data.payload.c_str());
            if (nextpacketId > 0)
            {
              waitingforack = true;
              data.onair = true;
              dataMap->put(topic, data);
            }
            DEBUG_D ("MQTT PUBLISHING DATAMAP %s=%s (nextpacketId=%d)\n", topic.c_str(), data.payload.c_str(), nextpacketId);
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
      DEBUG_I("Flash button is pressed, flashbuttontimer=%d\n", flashbuttontimer);
    }
    else
    {
      flashbuttontimer = 0;
    }

    if (flashbuttontimer == 3) // After 3 seconds clear passwords
    {
      flashbuttonstatus = 2;
      esp_password = "esplogin";
      DEBUG_I("Web Password defaulted to esplogin until reboot!\n");
      mainstate.defaultpassword = true;
    }

    if (flashbuttontimer == 6) // After 6 seconds start access point
    {
      flashbuttonstatus = 1;
      if (!WiFi.softAP(WiFi.hostname().c_str(), "esplogin", 6, 0)) DEBUG_E("Failed setting WiFi.softAP()");
      WiFi.mode(WIFI_AP);
      DEBUG_I("Wifi Accesspoint started!\n");
      mainstate.accesspoint = true;
    }

    if (flashbuttontimer == 10) ESP.reset();
  }
}
#endif

uint8_t oldeeprom_read(String * data, byte eepromindex)
{
  DEBUG_D("%s", String("read_oldeeprom(" + String(eepromindex) + ");\n").c_str());
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
  DEBUG_D("commit_eeprom();\n");
  int eeprompointer = 0;
  for (int i = 0; i < eepromMap->size(); i++)
  {
    uint8_t checksum = 20;
    String eepromdata = eepromMap->get(i);
    DEBUG_D("EEPROM.write(%d,%d) (length)\n", eeprompointer, eepromdata.length() + 1);
    EEPROM.write(eeprompointer++, eepromdata.length() + 1);
    for (unsigned int pos = 0; pos < eepromdata.length(); pos++)
    {
      char chr = eepromdata.c_str()[pos];
      DEBUG_D("EEPROM.write(%d,%c) (data)\n", eeprompointer, chr);
      EEPROM.write(eeprompointer++, chr);
      checksum += chr;
    }
    DEBUG_D("EEPROM.write(%d,%d) (checksum)\n", eeprompointer, checksum);
    EEPROM.write(eeprompointer++, checksum);
  }
  EEPROM.write(eeprompointer++, 0);
  EEPROM.commit();
}

void eeprom_write(String value, int eepromindex)
{
  DEBUG_D("eeprom_write %d,%s\n", eepromindex, value.c_str());
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
  DEBUG_D("eeprom_read();\n");
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
    DEBUG_D("Read from eeprom %d=%s (%d=%d)\n", eeprompointer, data.c_str(), checksum, eepromchecksum);
    if (eepromchecksum != checksum)
    {
      return 0;
      DEBUG_E("Error reading eeprom index %d (wrong checksum)!", index);
    }
    eepromMap->put(index++, data);
  }
  return 1;
}

void eeprom_erase()
{
  DEBUG_W("eeprom_erase();\n");
  EEPROM.write(512, 0);
  EEPROM.commit();
  EEPROM.end();
}

uint8_t eeprom_read(String * data, byte eepromindex)
{
  DEBUG_V("eeprom_read(%d)\n", eepromindex);
  if (eepromindex >= eepromMap->size()) return 0;
  *data = eepromMap->getData(eepromindex);
  DEBUG_D("eeprom_read(%d)=%s\n", eepromindex, data->c_str());
  return 1;
}

void systemTimerCallback()
{
  static uint8_t ms = 0;
  timertick = 1;
  ms++;
  if (ms >= 10)
  {
    uptime++;
    timersectick = 1;
    ms = 0;
  }
#ifdef ESPLED
  static uint8_t ledtimer = 0, ledontime, ledofftime;
  ledontime = 1;
  ledofftime = 1;
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
int8_t MHZ19_handle(int *ppm, int *temp)
{
  static unsigned long requesttime = 0;
  static unsigned long readtime = 0;
  if (requesttime < millis())
  {
    MHZ19_send_request_cmd();
    requesttime = millis() + 5000;
    readtime = millis() + 100;
  }
  if ((readtime > 0) && (readtime < millis()))
  {
    readtime = 0;
    if (MHZ19_read(ppm, temp))
    {
      DEBUG_I("MHZ19 ppm=%d temp=%d\n", *ppm, *temp);
      return 1;
    }
    else
    {
      DEBUG_E("MHZ19 read error!\n");
      return -1;
    }
  }
  return false;
}

void MHZ19_send_request_cmd()
{
  uint8_t cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  // command to ask for data

  DEBUG_V("Sending MHZ19 request packet...\n");

  Serial.flush();
  Serial.write(cmd, 9); //request PPM CO2
}

bool MHZ19_read(int *ppm, int *temp)
{
  static uint8_t errorcounter = 0;
  uint8_t response[9]; // for answer

  DEBUG_D("Receiving MHZ19 data...\n");

  // The serial stream can get out of sync. The response starts with 0xff, try to resync.
  uint8_t maxcounter = 0;
  while (Serial.available() > 0 && (uint8_t)Serial.peek() != 0xFF && maxcounter++ < 10) {
    Serial.read();
  }

  memset(response, 0, 9);
  Serial.readBytes(response, 9);
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
      *ppm = (256 * responseHigh) + responseLow;
      *temp = response[4] - 40;
      errorcounter = 0;
    }
  }
  else
  {
    DEBUG_E("Invalid response from MHZ_19 CO2 sensor!\n");
    if (errorcounter < 255) errorcounter++;
    if (errorcounter == 5)
    {
      return false;
    }
  }
  return true;
}


#endif


void handleWWWSettings()
{
  if (!webserver.authenticate("admin", esp_password.c_str())) {
    return webserver.requestAuthentication();
  }


  if (webserver.method() == HTTP_POST)
  {
    for (uint8_t i = 0; i < webserver.args(); i++)
    {
      if (webserver.argName(i) == "rebootdevice")
      {
        webserver.send(200, "text/html", "<HTML><HEAD><meta http-equiv=\"refresh\" content=\"5;url=\\\" /></HEAD><BODY>Device Rebooting, Please Wait...</BODY></HTML>");
        reboottimeout = uptime + 4;
        return;
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
      if (webserver.argName(i) == "mqtttopicprefix") mqtt_topicprefix = webserver.arg(i);
      if (webserver.argName(i) == "mqttport") mqtt_port = String(webserver.arg(i)).toInt();
      if (webserver.argName(i) == "mqttssl") mqtt_ssl = 1;
      if (webserver.argName(i) == "webpassword") esp_password = webserver.arg(i);
      if (webserver.argName(i) == "hostname") esp_hostname = webserver.arg(i);
#ifdef  ESPMQTT_WATERMETER
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
#ifdef  ESPMQTT_QSWIFIDIMMERD01
      if (webserver.argName(i) == "qswifidimoffset") qswifidimmer_setdimoffset((webserver.arg(i)).toInt());
#endif
#ifdef  ESPMQTT_QSWIFIDIMMERD02
      if (webserver.argName(i) == "qswifidimoffset0") qswifidimmer_setdimoffset((webserver.arg(i)).toInt(), 0);
      if (webserver.argName(i) == "qswifidimoffset1") qswifidimmer_setdimoffset((webserver.arg(i)).toInt(), 1);
#endif
    }
    eeprom_write(mqtt_server, 0);
    eeprom_write(mqtt_username, 1);
    eeprom_write(mqtt_password, 2);
    eeprom_write(esp_password, 3);
    eeprom_write(esp_hostname, 4);
    eeprom_write(String(mqtt_port), 5);
    eeprom_write(String(mqtt_ssl), 6);
    eeprom_write(mqtt_topicprefix, 7);
#ifdef  ESPMQTT_QSWIFIDIMMERD01
    eeprom_write(String(qswifidimmer_getdimoffset()), 8);
    putdatamap("dimoffset", String(qswifidimmer_getdimoffset()));
#endif
#ifdef  ESPMQTT_QSWIFIDIMMERD02
    eeprom_write(String(qswifidimmer_getdimoffset(0)), 8);
    putdatamap("dimoffset/0", String(qswifidimmer_getdimoffset(0)));
    eeprom_write(String(qswifidimmer_getdimoffset(1)), 9);
    putdatamap("dimoffset/1", String(qswifidimmer_getdimoffset(1)));
#endif
    eeprom_commit();
    ArduinoOTA.setHostname(esp_hostname.c_str());
    ArduinoOTA.setPassword(esp_password.c_str());
    MDNS.begin(esp_hostname.c_str());
    MDNS.notifyAPChange();
    Debug.setPassword(esp_password);
    mainstate.defaultpassword = false;

    disconnectMqtt(); // Disconnect mqtt server

    if (esp_hostname != WiFi.hostname())
    {
      WiFi.hostname(esp_hostname);
      wifichangesettingstimeout = uptime + 4;
    }

    if ((postwifissid != "") && (postwifikey != "") && (esp_hostname != "") && ((postwifissid != WiFi.SSID()) || (postwifikey != WiFi.psk())))
    {
      webserver.send(200, "text/html", "<HTML><BODY>Settings Saved.<BR>Please wait a moment and connect to proper wifi network and open the page of the saved hostname.</BODY></HTML>");
      flashbuttonstatus = 0;
      previouswifistatus = -1;
      wifichangesettingstimeout = uptime + 4;
      return;
    }
    else
    {
      wifichangesettingstimeout = uptime + 4;
      webserver.send(200, "text/html", "<HTML><HEAD><meta http-equiv=\"refresh\" content=\"5;url=\\\" /></HEAD><BODY>Settings Saved, Please Wait...</BODY></HTML>");
    }

    initMqtt();
    connectToMqtt();
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
    webpage += String("<TR><TD>MQTT Topic Prefix</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"50\" name=\"mqtttopicprefix\" value=\"") + mqtt_topicprefix + "\"></TD></TR>";
    webpage += String("<TR><TD>ESP Password</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"20\" name=\"webpassword\" value=\"") + esp_password + "\"></TD></TR>";
#ifdef  ESPMQTT_WATERMETER
    webpage += String("<TR><TD>Watermeter Liter</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"64\" name=\"watermeterliter\" value=\"") + getdatamap("water/liter") + "\"></TD></TR>";
#endif
#ifdef  ESPMQTT_QSWIFIDIMMERD01
    webpage += String("<TR><TD>Dimmer Offset</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"3\" name=\"qswifidimoffset\" value=\"") + getdatamap("dimoffset") + "\"></TD></TR>";
#endif
#ifdef  ESPMQTT_QSWIFIDIMMERD02
    webpage += String("<TR><TD>Dimmer Offset 0</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"3\" name=\"qswifidimoffset0\" value=\"") + getdatamap("dimoffset/0") + "\"></TD></TR>";
    webpage += String("<TR><TD>Dimmer Offset 1</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"3\" name=\"qswifidimoffset1\" value=\"") + getdatamap("dimoffset/1") + "\"></TD></TR>";
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
  DEBUG_I("New webclient connected...\n");
  webserver.send_P(200, "text/html", webpage_P);
}


#if (QSWIFIDIMMERCHANNELS == 1)
void qswifidimmer_callback (uint8_t , uint8_t dimvalue, bool dimstate)
{
  putdatamap ("dimvalue", String(dimvalue));
  putdatamap ("dimstate", String(dimstate));
}
#endif
#if (QSWIFIDIMMERCHANNELS == 2)
void qswifidimmer_callback (uint8_t dimchannel, uint8_t dimvalue, bool dimstate)
{
  putdatamap ("dimvalue/" + String(dimchannel), String(dimvalue));
  putdatamap ("dimstate/" + String(dimchannel), String(dimstate));
}
#endif

#if (QSWIFIDIMMERCHANNELS == 1)
void qswifidimmer_switchcallback(uint8_t , bool switchstate)
{
  putdatamap ("switchstate", String(switchstate));
}
#endif
#if (QSWIFIDIMMERCHANNELS == 2)
void qswifidimmer_switchcallback(uint8_t dimchannel, bool switchstate)
{
  putdatamap ("switchstate/" + String(dimchannel), String(switchstate));
}
#endif

void ducoboxcallback (String topic, String payload)
{
  putdatamap(topic, payload);
}

void amgpelletstovecallback (String topic, String payload)
{
  putdatamap(topic, payload);
}

void openthermcallback (String topic, String payload)
{
  putdatamap(topic, payload);
}

#ifdef  ESPMQTT_GROWATT
void growattcallback (String topic, String payload)
{
  if (topic == "status")
  {
    if (payload == "ready") digitalWrite(NODEMCULEDPIN, 0);
    else digitalWrite(NODEMCULEDPIN, 1);
  }
  putdatamap(topic, payload);
}
#endif
#ifdef  ESPMQTT_SMARTMETER
void smartmetercallback (String topic, String payload)
{
  static uint32 nextupdatetime = 0;
  static bool sendupdate = 0;

  if ((nextupdatetime < uptime) && (topic == "status") && (payload == "receiving")) // wait for start of new packet from smartmeter
  {
    sendupdate = 1;
    nextupdatetime = uptime + 8; // wait 8 seconds before accepting new values from smartmeter (some smartmeters keep sending out data)
  }

  if (sendupdate) putdatamap(topic, payload);

  if ((topic == "status") && (payload == "ready") && sendupdate) // stop processing when a complete packet was pushed to datamap
  {
    sendupdate = 0;
  }
}
#endif

String getRandomString(int len) {
  static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";

  String returnstring = "";

  for (int i = 0; i < len; ++i) {
    returnstring += alphanum[random(sizeof(alphanum) - 1)];
  }

  return returnstring;
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
  DEBUG_I("\n\nInitializing ESP8266 %s %s...\n\n", FIRMWARE_TARGET, ESPMQTT_VERSION);

  char buffer[25];
  snprintf(buffer, 25, "%08X", ESP.getChipId());
  chipid = buffer;

  sntp_set_timezone(1);
  sntp_setservername(0, "nl.pool.ntp.org");
  // Read settings from EEPROM
  DEBUG_D("Reading internal EEPROM...\n");
  eeprom_init();
  if (!eeprom_read(&mqtt_server, 0))
  {
    DEBUG_E("Error reading mqtt server from internal eeprom\n");
  }
  if (!eeprom_read(&mqtt_username, 1))
  {
    DEBUG_E("Error reading mqtt username from internal eeprom\n");
  }
  DEBUG_D("mqtt username=%s\n", mqtt_username.c_str());

  if (!eeprom_read(&mqtt_password, 2))
  {
    DEBUG_E("Error reading mqtt password from internal eeprom\n");
  }
  DEBUG_D("mqtt password=%s\n", mqtt_password.c_str());

  if (!eeprom_read(&esp_password, 3))
  {
    DEBUG_E("Error reading esp password from internal eeprom\n");
  }
  DEBUG_D("esp password=%s\n", esp_password.c_str());

  if (!eeprom_read(&esp_hostname, 4))
  {
    DEBUG_E("Error reading hostname from internal eeprom\n");
    esp_hostname = String(FIRMWARE_TARGET) + "-" + String(chipid);
  }
  DEBUG_I("Hostname=%s\n", esp_hostname.c_str());

  String mqttportstr = "";
  if (!eeprom_read(&mqttportstr, 5))
  {
    DEBUG_E("Error reading mqttport from internal eeprom\n");
  }
  else
  {
    if ((mqttportstr != "") && (mqttportstr.toInt() > 0 ) && (mqttportstr.toInt() < 65536)) mqtt_port = mqttportstr.toInt();
  }
  DEBUG_D("mqtt port=%d\n", mqtt_port);

  String mqttsslstr = "";
  if (!eeprom_read(&mqttsslstr, 6))
  {
    DEBUG_E("Error reading mqtt ssl from internal eeprom\n");
  }
  else
  {
    if (mqttsslstr != "") mqtt_ssl = mqttsslstr == "1" ? 1 : 0;
  }
  DEBUG_D("mqtt ssl=%d\n", mqtt_ssl);

  if (!eeprom_read(&mqtt_topicprefix, 7))
  {
    DEBUG_E("Error reading mqtt main topic from internal eeprom\n");
    mqtt_topicprefix = "home/" + esp_hostname + "/";
  }
  DEBUG_D("mqtt topicprefix=%s\n", mqtt_topicprefix.c_str());


#ifdef  ESPMQTT_DDNS
  EasyDDNS.service("duckdns");
  EasyDDNS.client("renegusta.duckdns.org", "c4f55de5-0d15-477d-a938-f39c19c67b33");
#endif


#ifdef SONOFFCH
  sonoff_init();
#endif

  initWifi();
  connectToWifi();

  ArduinoOTA.setHostname(esp_hostname.c_str());

  Debug.begin(esp_hostname.c_str(), DEBUGLEVEL);
  Debug.setPassword(esp_password);
  Debug.setResetCmdEnabled(true);
  Debug.setCallBackProjectCmds(&processCmdRemoteDebug);

  syslog.server(mqtt_server.c_str(), 514);
  syslog.deviceHostname(esp_hostname.c_str());
  syslog.appName(FIRMWARE_TARGET);
  syslog.defaultPriority(LOG_KERN);

  DEBUG_I("ESP8266 Started...\n");
  DEBUG_I("Hostname=%s\n", WiFi.hostname().c_str());

  ArduinoOTA.setPassword(esp_password.c_str());

  ArduinoOTA.onStart([]() {
    Serial.end();
#ifdef  ESPMQTT_DIMMER
    dimmer_stop();
#endif
  });

  ArduinoOTA.onEnd([]() {
    initSerial();
#ifdef  ESPMQTT_DIMMER
    dimmer_init(ZEROCROSS_PIN, TRIAC_PIN);
#endif
  });

  ArduinoOTA.onProgress([](unsigned int, unsigned int) {
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

#ifdef  ESPMQTT_WATERMETER
  i2cEeprom_init(I2C_SDA, I2C_SCL, I2C_EEPROM_ADDRESS, (uint32_t)EE24LC512MAXBYTES);
  uint32_t watermeter_liters = i2cEeprom_read();
  watermeter_init(WATERPULSEPIN, NODEMCULEDPIN, watermeter_liters);
  putdatamap("water/lmin", "0");
  putdatamap("water/m3h", "0");
  putdatamap("water/liter", String(watermeter_liters));
  putdatamap("water/m3", String(double(watermeter_liters) / 1000, 3));
#endif

#ifdef  ESPMQTT_MAINPOWERMETER
  circuitspowermeter_init(ADS0_CS_PIN, ADS0_RDY_PIN, ADS1_CS_PIN, ADS1_RDY_PIN);
#endif

#ifdef  ESPMQTT_SONOFF_FLOORHEATING
  pinMode(3, FUNCTION_3);
#endif

#ifdef ONEWIREPIN
  oneWireSensors.setWaitForConversion(false);
  oneWireSensors.setResolution(12);

#ifdef  ESPMQTT_OPENTHERM
  if (!oneWireSensors.getAddress(onewire_dcwSupplyWaterThermometer, 0)) {
    DEBUG_E("Unable to find address for onewire_dcwSupplyWaterThermometer\n");
  }
  else onewire_dcwSupplyWaterEnabled = true;
  if (!oneWireSensors.getAddress(onewire_chReturnWaterThermometer, 1)) {
    DEBUG_E("Unable to find address for Device onewire_chReturnWaterThermometer\n");

  }
  else onewire_chReturnWaterEnabled = true;
#endif

#ifdef  ESPMQTT_WEATHER
  if (!oneWireSensors.getAddress(onewire_OutsideAddress, 0)) {
    DEBUG_E("Unable to find address for onewire_outsidetemp\n");
  }
#endif

#ifdef  ESPMQTT_SONOFF_FLOORHEATING
  if (!oneWireSensors.getAddress(onewire_floorWaterAddress, 0)) {
    DEBUG_E("Unable to find address for onewire_floorwater\n");
  }
#endif
#endif

  webserver.on("/", handleWWWRoot);
  webserver.on("/data.json", handleJsonData);
  webserver.on("/settings", handleWWWSettings);
  webserver.begin();


  MDNS.begin(esp_hostname.c_str());
  MDNS.addService("http", "tcp", 80);

  systemTimer.attach_ms(100, systemTimerCallback);

#ifdef ESPLED
  pinMode(ESPLED, OUTPUT);
#ifdef ESPLEDINVERSE
  digitalWrite(ESPLED, 1);
#else
  digitalWrite(ESPLED, 0);
#endif
#endif

#ifdef NODEMCULEDPIN
  pinMode(NODEMCULEDPIN, OUTPUT);
  digitalWrite(NODEMCULEDPIN, 1);
#endif


#ifdef FLASHBUTTON
  pinMode(FLASHBUTTON, INPUT_PULLUP);
#endif

#ifdef  ESPMQTT_SMARTMETER
  smartmeter_init(smartmetercallback);
#endif

#ifdef  ESPMQTT_OPENTHERM
  opentherm_init(openthermcallback);
#endif

#ifdef  ESPMQTT_GROWATT
  growatt_init(growattcallback, FANPIN);
#endif

#ifdef  ESPMQTT_AMGPELLETSTOVE
  amgpelletstove_init(amgpelletstovecallback);
#endif

#ifdef NEOPIXELPIN
  neopixelleds.begin();               // init van de strip
  neopixelleds.setPixelColor(0, neopixelleds.Color(0, 0, 0));
  neopixelleds.setPixelColor(1, neopixelleds.Color(0, 0, 0));
  neopixelleds.show();
#endif

#ifdef  ESPMQTT_DIMMER
  dimmer_init(ZEROCROSS_PIN, TRIAC_PIN);
#endif

#ifdef  ESPMQTT_DUCOBOX
  ducobox_init(sonoff_relays[0], sonoff_relays[1], 10, ducoboxcallback);
#endif

#ifdef  ESPMQTT_SONOFFBULB
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

#ifdef DHTPIN
  dht.begin();
#endif

#ifdef QSWIFIDIMMERCHANNELS
  qswifidimmer_init(QSWIFIDIMMERCHANNELS, qswifidimmer_callback);
  qswifidimmer_setswitchcallback(qswifidimmer_switchcallback);
#ifdef ESPMQTT_QSWIFIDIMMERD01
  String dimoffset = "0";
  if (!eeprom_read(&dimoffset, 8))
  {
    DEBUG_E("Error reading dimmer offset from internal eeprom\n");
    dimoffset = "20";
  }
  qswifidimmer_setdimoffset(dimoffset.toInt());
  putdatamap("dimoffset", String(qswifidimmer_getdimoffset()));
#endif
#ifdef ESPMQTT_QSWIFIDIMMERD02
  String dimoffset = "0";
  if (!eeprom_read(&dimoffset, 8))
  {
    DEBUG_E("Error reading dimmer offset 0 from internal eeprom\n");
    dimoffset = "20";
  }
  qswifidimmer_setdimoffset(dimoffset.toInt(), 0);
  dimoffset = "0";
  if (!eeprom_read(&dimoffset, 9))
  {
    DEBUG_E("Error reading dimmer offset 1 from internal eeprom\n");
    dimoffset = "20";
  }
  qswifidimmer_setdimoffset(dimoffset.toInt(), 1);
  putdatamap("dimoffset/0", String(qswifidimmer_getdimoffset(0)));
  putdatamap("dimoffset/1", String(qswifidimmer_getdimoffset(1)));
#endif
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
    DEBUG("  factoryreset\n");
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
    DEBUG("wificonnected=%d\n", mainstate.wificonnected);
    DEBUG("mqttconnected=%d\n", mainstate.mqttconnected);
    DEBUG("mqttready=%d\n", mainstate.mqttready);
    DEBUG("mqttsenddatamap=%d\n", mainstate.mqttsenddatamap);
    DEBUG("defaultpassword=%d\n", mainstate.defaultpassword);
    DEBUG("accesspoint=%d\n", mainstate.accesspoint);
  }

  if (lastCmd == "showdatamap") showdatamap();

  if (lastCmd == "factoryreset")
  {
    WiFi.disconnect();
    eeprom_erase();
    delay(1000);
    ESP.restart();
    delay(1000);
  }

#ifdef  ESPMQTT_WATERMETER
  if (lastCmd == "help") DEBUG("  watermeterreadeeprom\n  watermeterwriteeeprom\n");
  if (lastCmd == "watermeterreadeeprom")
  {
    DEBUG("i2cEeprom read liters=%d\n", i2cEeprom_read());
    watermeter_setliters(i2cEeprom_read());
  }
  if (lastCmd == "watermeterwriteeeprom")
  {
    DEBUG("i2cEeprom write liters=%d\n", watermeter_getliters());
    i2cEeprom_write(watermeter_getliters());
  }
#endif
}

static void handleDataExternalIpServer(void*, AsyncClient* client, void *data, size_t len) {
  char *chardata = (char *)data;
  String datastring = "";
  for (unsigned int i = 0; i < len; i++)
  {
    if ((chardata[i] == '\n') || (chardata[i] == '\r')) break;
    if (((chardata[i] >= 48) && (chardata[i] <= 58)) || chardata[i] == 46) datastring += chardata[i]; // Only accept when character is a number a . or a :
    else
    {
      datastring = "";
      break;
    }

  }
  putdatamap("wifi/externalip", datastring);
  client->close();
}

void onConnectExternalIpServer(void*, AsyncClient* client) {
  client->add("GET /extip.php\r\n", 16);
  client->send();
}

void updateexternalip()
{
  static  AsyncClient* client = new AsyncClient;
  client->close();
  client->onData(&handleDataExternalIpServer, client);
  client->onConnect(&onConnectExternalIpServer, client);
  client->connect("j4m.nl", 80);
}
