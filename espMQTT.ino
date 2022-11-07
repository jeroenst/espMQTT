/*
   Version of esp8266 library: 2.6.2

   Needed libraries:
    https://github.com/jeroenst/RemoteDebug
    https://github.com/jeroenst/ESPAsyncTCP
    https://github.com/jeroenst/async-mqtt-client
    https://github.com/jeroenst/syslog

   Optional libraries depending on defines:
    (Sonoff POW) https://github.com/jeroenst/hlw8012
    (RGB LED) https://github.com/jeroenst/my92xx
    (DS18B20) https://github.com/jeroenst/OneWire
    (DS18B20) https://github.com/jeroenst/Arduino-Temperature-Control-Library
    (I2C OLED DISPLAY) https://github.com/jeroenst/esp8266-oled-ssd1306

   Libraries via Arduino Library Manager:
    (I2C) Wire
*/

#define DEFAULT_PASSWORD "esplogin"
#define CPUSLEEP 50

#ifdef ESPMQTT_BUILDSCRIPT
#define DEBUGLEVEL Debug.INFO
#else
// Only use defines when when firmware is not compiled from the build script...
/* SETTINGS */
#define SERIALLOG
#define MYTZ TZ_Europe_Amsterdam
#define DEBUGLEVEL Debug.VERBOSE

/* ESP8266 */
// #define ESPMQTT_WEATHER
// #define ESPMQTT_AMGPELLETSTOVE
// #define ESPMQTT_BATHROOM
// #define ESPMQTT_BEDROOM2
// #define ESPMQTT_OPENTHERM
#define ESPMQTT_SMARTMETER
// #define ESPMQTT_GROWATT
// #define ESPMQTT_GROWATT_MODBUS
// #define ESPMQTT_SDM120
// #define ESPMQTT_DDM18SD
// #define ESPMQTT_WATERMETER
// #define ESPMQTT_WATERMETER2
// #define ESPMQTT_DDNS
// #define ESPMQTT_GENERIC8266
// #define ESPMQTT_GENERIC8266_NEO
// #define ESPMQTT_OBD2
// #define ESPMQTT_NOISE
// #define ESPMQTT_SOIL
// #define ESPMQTT_DIMMER
// #define ESPMQTT_RELAY
// #define ESPMQTT_LIVINGROOM
// #define ESPMQTT_BBQTEMP
// #define ESPMQTT_GOODWE

/* ESP8285 */
// #define ESPMQTT_ZMAI90
// #define ESPMQTT_DUCOBOX
// #define ESPMQTT_SONOFFS20 // coffeelamp & sonoffs20_00X
// #define ESPMQTT_SONOFFBULB
// #define ESPMQTT_GARDEN //ESP8285 TUIN & MARIANNE & LUIFEL
// #define ESPMQTT_SONOFF_FLOORHEATING
// #define ESPMQTT_IRRIGATION
// #define ESPMQTT_BLITZWOLF
// #define ESPMQTT_QSWIFIDIMMERD01
// #define ESPMQTT_QSWIFIDIMMERD02
// #define ESPMQTT_SONOFF4CH //ESP8285
// #define ESPMQTT_SONOFFDUAL
// #define ESPMQTT_SONOFFS20_PRINTER
// #define ESPMQTT_SONOFFPOW
// #define ESPMQTT_SONOFFPOWR2 // tv&washingmachine&server&dishwasher
// #define ESPMQTT_SONOFFTH
// #define ESPMQTT_GENERIC8255
// #define ESPMQTT_BHT002
// #define ESPMQTT_TUYA_2GANGDIMMERV2
// #define ESPMQTT_QSWIFISWITCH1C
// #define ESPMQTT_QSWIFISWITCH2C

#define ESPMQTT_VERSION "TEST"
#endif

#define APONBOOT

#ifdef ESPMQTT_ZMAI90
#define FIRMWARE_TARGET "ZMAI90"
#define FLASHBUTTON 13
#define ZMAI90RELAY 12
#define ESPLED 4
#undef SERIALLOG
uint8_t zmai90pointer = 255;
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
#define SONOFF_FLOORHEATING_TEMPMAX 450
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

#ifdef  ESPMQTT_DDM18SD
#define FIRMWARE_TARGET "DDM18SD"
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
#undef CPUSLEEP
#define CPUSLEEP 5
#endif

#ifdef  ESPMQTT_QSWIFIDIMMERD02
#define FIRMWARE_TARGET "QSWIFIDIMMERD02"
#define APONBOOT
#define QSWIFIDIMMERCHANNELS 2
#include "qswifidimmer.h"
#undef CPUSLEEP
#define CPUSLEEP 5
#endif

#ifdef  ESPMQTT_QSWIFISWITCH1C
#define FIRMWARE_TARGET "QSWIFISWITCH1C"
#define APONBOOT
#define QSWIFISWITCHCHANNELS 1
#include "qswifiswitch.h"
QsWifiSwitch qswifiswitch(QSWIFISWITCHCHANNELS);
#endif

#ifdef  ESPMQTT_QSWIFISWITCH2C
#define FIRMWARE_TARGET "QSWIFISWITCH2C"
#define APONBOOT
#define QSWIFISWITCHCHANNELS 2
#include "qswifiswitch.h"
QsWifiSwitch qswifiswitch(QSWIFISWITCHCHANNELS);
#endif

#ifdef  ESPMQTT_BHT002
#define FIRMWARE_TARGET "BHT002"
#define APONBOOT
#include "bht002.h"
#undef SERIALLOG
#endif

#ifdef ESPMQTT_TUYA_2GANGDIMMERV2
#define FIRMWARE_TARGET "TUYA_2GANGDIMMERV2"
#define APONBOOT
#define ESPMQTT_NOSERIAL_DEBUG
#include "tuya.h"
#undef SERIALLOG
#endif

#ifdef  ESPMQTT_GENERIC8255
#define FIRMWARE_TARGET "GENERIC8255"
#define SERIALLOG
#define APONBOOT
#define ESPLED D4
#endif

#ifdef  ESPMQTT_BBQTEMP
#define FIRMWARE_TARGET "BBQTEMP"
#define SERIALLOG
#define APONBOOT
//#define ESPLED D4
#include <SPI.h>
#define ESPMQTT_BBQTEMP_CS0 2
#define ESPMQTT_BBQTEMP_CS1 16

#define SMALLOLED
#define I2C_SDA D2
#define I2C_SCL D1
#define OLED_ADDRESS 0x3c
#define OLEDX 32
#define OLEDSMALL
#endif

#ifdef  ESPMQTT_GENERIC8266
#define FIRMWARE_TARGET "GENERIC8266"
#define FLASHBUTTON D3
#define ESPLED D4
#define SERIALLOG
#define APONBOOT
#endif

#ifdef  ESPMQTT_GENERIC8266_NEO
#define FIRMWARE_TARGET "GENERIC8266"
#define FLASHBUTTON D3
#define NEOPIXELPIN D8
#define ESPLED D4
#define SERIALLOG
#define APONBOOT
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
struct
{
  int16_t temperature = INT16_MAX; //In 0.1 degree celcius
  uint16_t rain_pulses = 0;
  uint16_t rain_hour_pulses = 0;
  uint16_t rain_lasthour_pulses = 0;
  uint16_t rain_minute_pulses = 0;
  uint16_t rain_lastminute_pulses = 0;
} weather;
#define DATAMAPLENGTH_ADD 11
#undef SERIALLOG
#undef CPUSLEEP
#define CPUSLEEP 5
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

#ifdef  ESPMQTT_GOODWE
#define FIRMWARE_TARGET "GOODWE"
#define FLASHBUTTON 0 //d3
#define ESPLED 2 // d4
#define NODEMCULEDPIN 16 //d0
#include "goodwe.h"
#undef SERIALLOG
#endif

#ifdef  ESPMQTT_GROWATT_MODBUS
#define FIRMWARE_TARGET "GROWATT_MODBUS"
#define NODEMCULEDPIN D0
#define FLASHBUTTON D3
#define ESPLED D4
#include "growattmodbus.h"
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
#define DATAMAPLENGTH_ADD 6
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

#ifdef  ESPMQTT_RELAY
#ifndef FIRMWARE_TARGET
#define FIRMWARE_TARGET "RELAY"
#endif
#define ESPLED 2
#define SONOFFCH 1
#define APONBOOT
const byte sonoff_relays[2] = {15};
const byte sonoff_buttons[2] = {0};
static bool sonoff_oldbuttons[2] = {1};
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
double voltval = -1;
double currentval = -1;
double powerval = -1;
#endif

#ifdef  ESPMQTT_SONOFFTH
#define FIRMWARE_TARGET "SONOFFTH"
#ifndef ARDUINO_ESP8266_ESP01
#error "Wrong board selected! Select Generic ESP8285 module"
#endif

#define FLASHBUTTON 0
#define ESPLED 13
#define SONOFFCH 1
#define ONEWIREPIN 14

const byte sonoff_relays[1] = {12};
const byte sonoff_buttons[1] = {0};
static bool sonoff_oldbuttons[1] = {1};
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
#define DATAMAPLENGTH_ADD 4
#endif

#ifdef  ESPMQTT_WATERMETER2
#define FIRMWARE_TARGET "WATERMETER2"
#define NODEMCULEDPIN D0
#define FLASHBUTTON D3
#define ESPLED D4
#define NEOPIXELPIN D8
#define WATERPULSEPIN D5
#include "watermeter.h"
#define DATAMAPLENGTH_ADD 4
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
//#define ESPLED D4 // Disable led
//#define ESPLED_TIMEOUT_OFF 60 // In bedroom we want the led to go off after some time
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


#ifdef  ESPMQTT_LIVINGROOM
#define FIRMWARE_TARGET "LIVINGROOM"
#define FLASHBUTTON D3
#define ESPLED D4
#define DHTPIN D5
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
#endif


#ifdef ESPMQTT_SMARTMETER
#define FIRMWARE_TARGET "SMARTMETER"
#define FLASHBUTTON D3
#define ESPLED D4
#define NODEMCULEDPIN D0
#include "smartmeter.h"
#undef SERIALLOG
#undef CPUSLEEP
#endif

#ifdef  ESPMQTT_DDNS
#include<EasyDDNS.h>
#define FIRMWARE_TARGET "DDNS"
#define NODEMCULEDPIN D0
#define FLASHBUTTON D3
#define ESPLED D4
#endif

#ifdef SONOFFCH
static bool sonoff_relaystate[SONOFFCH];
#endif

#ifdef NEOPIXELPIN
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel neopixelleds = Adafruit_NeoPixel(2, NEOPIXELPIN, NEO_RGB + NEO_KHZ400);
#endif

#ifdef MH_Z19
struct
{
  int8_t temperature = INT8_MAX; //In 1 degree celcius
  uint16_t co2 = UINT16_MAX; //In 1 ppm
  struct 
  {
      uint8_t temperature : 1;
      uint8_t co2 : 1;
  } changed;
} mhz19;
#endif

// ################################################################################################################# END OF DEFINES ####################################################################################################################

#include <time.h>
#include <sys/time.h>                   // struct timeval
#include <coredecls.h>                  // settimeofday_cb()
#include "sntp.h"


#include "espMQTT.h"
#include <TZ.h>

#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>        // Include the mDNS library
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <ArduinoJson.h>

#include <EEPROM.h>
#include <AsyncMqttClient.h>
AsyncMqttClient mqttClient;
#include <ArduinoOTA.h>
//#include <user_interface.h>
#include "SimpleMap.h"
//#include <//syslog.h>
//#define //syslogD(fmt, ...) if (WiFi.status() == WL_CONNECTED) //syslog.logf(LOG_DEBUG,"(%s) " fmt, __func__, ##__VA_ARGS__)
//#define //syslogI(fmt, ...) if (WiFi.status() == WL_CONNECTED) //syslog.logf(LOG_INFO,"(%s) " fmt, __func__, ##__VA_ARGS__)
//#define //syslogN(fmt, ...) if (WiFi.status() == WL_CONNECTED) //syslog.logf(LOG_NOTICE,"(%s) " fmt, __func__, ##__VA_ARGS__)
//#define //syslogW(fmt, ...) if (WiFi.status() == WL_CONNECTED) //syslog.logf(LOG_WARNING,"(%s) " fmt, __func__, ##__VA_ARGS__)
//#define //syslogE(fmt, ...) if (WiFi.status() == WL_CONNECTED) //syslog.logf(LOG_ERR,"(%s) " fmt, __func__, ##__VA_ARGS__)
//#define //syslogC(fmt, ...) if (WiFi.status() == WL_CONNECTED) //syslog.logf(LOG_CRIT,"(%s) " fmt, __func__, ##__VA_ARGS__)
//#define //syslogA(fmt, ...) if (WiFi.status() == WL_CONNECTED) //syslog.logf(LOG_ALERT,"(%s) " fmt, __func__, ##__VA_ARGS__)
//#define //syslogEM(fmt, ...) if (WiFi.status() == WL_CONNECTED) //syslog.logf(LOG_EMERG,"(%s) " fmt, __func__, ##__VA_ARGS__)

#include <ESP8266Ping.h>

// A UDP instance to let us send and receive packets over UDP
//WiFiUDP udpClient//syslog;
//WiFiUDP udpClient;

// Create a new empty //syslog instance
////syslog //syslog(udpClient//syslog, //syslog_PROTO_IETF);


#ifdef HLW8012_CF_PIN
#include <HLW8012.h>
#endif

char _gFmtBuf[100 + 1];

struct {
  bool wificonnected = false;
  bool wifidisconnected = false;
  bool mqttconnected = false;
  bool mqttdisconnected = false;
  bool wifiscanready = false;
  bool mqttsubscribed = false;
  String firmwareupgrade = "";
} triggers;

struct {
  bool wificonnected = false;
  bool wificonnectedonce = false;
  bool mqttconnected = false;
  bool mqttsubscribed = false;
  bool mqttconnectedtrigger = false;
  bool mqttready = false;
  bool mqttsenddatamap = false;
  uint16_t mqttlastsubscribedpacketid = 0;
  uint16_t mqttlastpublishedpacketid = 0;
  bool defaultpassword = true;
  bool accesspoint = false;
  bool statusok = true;
} mainstate;


int wifichannel = 0;
int wifinetworksfound = 0;

bool updatemqtt = 0;

static const char webpage_P[] PROGMEM = "<!DOCTYPE html><html><meta charset=\"UTF-8\"><meta name=\"google\" content=\"notranslate\"><meta http-equiv=\"Content-Language\" content=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><style>table{width: 400px; margin: auto;}</style></head><body><CENTER><div align='center' style='width:400px; margin:auto'><CENTER><H1><p id='header'></p></H1></CENTER><p id='table'></p><A HREF='settings'>Settings</A></div></CENTER><script>function refreshsite(){var obj,dbParam,xmlhttp,myObj,x,txt ='';xmlhttp=new XMLHttpRequest();xmlhttp.onreadystatechange=function(){if(this.readyState==4&&this.status==200){myObj=JSON.parse(this.responseText);txt+='<TABLE>';for (x in myObj){if(x=='hostname')document.getElementById('header').innerHTML=myObj[x].toUpperCase();txt+='<tr><td>'+x.split('/').join(' ')+'</td><td>'+myObj[x]+'</td></tr>';}txt+='</table>';document.getElementById('table').innerHTML = txt;}};xmlhttp.open('GET','data.json',true);xmlhttp.setRequestHeader('Content-type','application/x-www-form-urlencoded');xmlhttp.send();}refreshsite();window.setInterval(refreshsite, 5000);</script></body></html>";

extern "C" {
  //#include "user_interface.h"
}

#ifdef DHTPIN
#include "DHT.h"
DHT dhtdevice(DHTPIN, DHTTYPE);
struct
{
  int16_t temperature = INT16_MAX; //In 0.1 degree celcius
  uint16_t humidity = UINT16_MAX; //In 0.1 percent
  int16_t heatindex = INT16_MAX; //In 0.1 degree celcius
  struct 
  {
      uint8_t temperature : 1;
      uint8_t humidity : 1;
      uint8_t heatindex : 1;
  } changed;
} dht;
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
#endif

#ifdef  ESPMQTT_WEATHER
DeviceAddress onewire_OutsideAddress;
float onewire_chOutsideTemperature = -127;
float oldonewire_chOutsideTemperature = -127;
#endif

#ifdef  ESPMQTT_SONOFFTH
DeviceAddress onewire_address;
#endif


#ifdef  ESPMQTT_SONOFF_FLOORHEATING
DeviceAddress onewire_floorWaterAddress;
float onewire_floorWaterTemperature = -127;
bool floorheating_valveon = 0;
#endif
#endif

//WiFiClientSecure wifiClientSecure;
//WiFiClient wifiClient;
ESP8266WebServer webserver(80);
#include <WiFiUdp.h>

String chipid;
uint32_t uptime = 0;
bool timertick = 1;
bool timersectick = 1;
#define MQTT_SERVER_SIZE 30
char mqtt_server[MQTT_SERVER_SIZE] = "";
#define MQTT_USERNAME_SIZE 20
char mqtt_username[MQTT_USERNAME_SIZE] = "";
#define MQTT_PASSWORD_SIZE 20
char mqtt_password[MQTT_PASSWORD_SIZE] = "";
#define MQTT_TOPICPREFIX_SIZE 30
char mqtt_topicprefix[MQTT_TOPICPREFIX_SIZE] = "";
int mqtt_port = 1883;
bool mqtt_ssl = 0;
#define ESP_PASSWORD_SIZE 10
char esp_password[ESP_PASSWORD_SIZE] = DEFAULT_PASSWORD;
#define ESP_HOSTNAME_SIZE 20
char esp_hostname[ESP_HOSTNAME_SIZE] = "";
RemoteDebug Debug;

#define WIFISSID_SIZE 20
char wifissid[WIFISSID_SIZE] = "";
#define WIFIPSK_SIZE 20
char wifipsk[WIFIPSK_SIZE] = "";

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


int replacechar(char *str, const char orig, const char rep) {
  char *ix = str;
  int n = 0;
  while ((ix = strchr(ix, orig)) != NULL) {
    *ix++ = rep;
    n++;
  }
  return n;
}


size_t strlcpy(char* dst, const char* src, size_t bufsize)
{
  size_t srclen = strlen(src);
  size_t result = srclen; /* Result is always the length of the src string */
  if (bufsize > 0)
  {
    if (srclen >= bufsize)
      srclen = bufsize - 1;
    if (srclen > 0)
      memcpy(dst, src, srclen);
    dst[srclen] = '\0';
  }
  return result;
}

#ifdef NEOPIXELPIN
void update_neoled_status()
{
  static uint8_t ledtimer = 0;
  static uint8_t ledtimeout = 0;
  if (ledtimer++ == ledtimeout)
  {
    ledtimer = 0;
    neopixelleds.setPixelColor(0, neopixelleds.Color(0, 0, 0));
  }

  if (mainstate.accesspoint) {
    neopixelleds.setPixelColor(0, neopixelleds.Color(50, 0, 15));
  }
  else if (mainstate.defaultpassword) {
    neopixelleds.setPixelColor(0, neopixelleds.Color(50, 40, 20));
  }
  else if ((mainstate.statusok) && (mainstate.mqttconnected)) {
    if (ledtimer == 19) neopixelleds.setPixelColor(0, neopixelleds.Color(0, 50, 0));
    ledtimeout = 20;
  }
  else if (mainstate.mqttconnected) {
    if (ledtimer == 5) neopixelleds.setPixelColor(0, neopixelleds.Color(0, 0, 50));
    ledtimeout = 10;
  }
  else if (mainstate.wificonnected) {
    if (ledtimer == 5) neopixelleds.setPixelColor(0, neopixelleds.Color(50, 20, 0));
    ledtimeout = 10;
  }
  else
  {
    if (ledtimer == 5) neopixelleds.setPixelColor(0, neopixelleds.Color(50, 0, 0));
    ledtimeout = 10;
  }

  neopixelleds.show();
}
#endif

void publishdatamap(int32_t packetId = -1, bool publishall = false, bool init = false, bool publishregular = false);

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

void updateexternalip();

#define DATAMAP_BASELENGTH 25

enum class DataMapStatus {initializing, wifierror, mqtterror, commerror, online, receiving, querying, ready, rebooting, upgrading};
enum class DataMapStatusUpgrade {notchecked, uptodate, httperror, upgrading, upgradedone, incorrectupgradekey };

struct
{
  uint64_t sendIds[2] = {0, 0}; // Ids that are queued to be send to mqtt broker
  uint64_t publishRegularIds[2] = {0, 0}; // Ids that will be send to mqtt broker regulary
  int8_t length = DATAMAP_BASELENGTH; // Length of datamap
  char firmware_upgradekey[11] = ""; // Key that has to be provided by mqtt upgrade command before upgrading to prevent upgrade loop
  DataMapStatus status = DataMapStatus::initializing; // The main status of this esp
  bool sendStatusAfterPublishCompleted = false; // When true, the status will be send to mqtt broker when all other other data has been published (usually for a ready message after sending out new values)
  DataMapStatusUpgrade status_upgrade = DataMapStatusUpgrade::notchecked; // Upgrade status
  char externalip[17] = ""; // For storing the external ip discovered.
} DataMap;



bool getdatamap_checkandfill(char *key, char *outputvalue, int8_t id, uint8_t idCounter, const char *searchkey, const char *inputvalue, const bool dataValid = true)
{
  if (((strcmp (key, searchkey) == 0) || (id == idCounter)) && (id < DataMap.length))
  {
    if (dataValid) strcpy (outputvalue, inputvalue);
    else strcpy (outputvalue, "-");
    if (id >= 0) strcpy (key, searchkey);
    return true;
  }
  return false;
}

uint8_t getDataMapLength()
{
  return DataMap.length;
}

bool getDataMapPublishRegular(uint8_t id)
{
  if (id < 64) return ((DataMap.publishRegularIds[0] & (uint64_t(1) << id)) > 0);
  else return ((DataMap.publishRegularIds[1] & (uint64_t(1) << (id - 64))) > 0);
}

void setDataMapPublishRegular(uint8_t id, bool value = true)
{
  if (id < 64)
  {
    if (value) DataMap.publishRegularIds[0] |= 1 << id;
    else DataMap.publishRegularIds[0] &= DataMap.publishRegularIds[0] - (1 << id);
  }
  else
  {
    if (value) DataMap.publishRegularIds[1] |= 1 << (id - 64);
    else DataMap.publishRegularIds[1] &= DataMap.publishRegularIds[1] - (1 << (id - 64));
  }
}

bool getDataMapSendStatus(uint8_t id)
{
  if (DataMap.sendStatusAfterPublishCompleted && DataMap.sendIds[0] == 0 && DataMap.sendIds[1] == 0 && id == 1) return 1;
  if (id < 64) return ((DataMap.sendIds[0] & (uint64_t(1) << id)) > 0);
  else return ((DataMap.sendIds[1] & (uint64_t(1) << (id - 64))) > 0);
}

String uint64_to_binstr (uint64_t n)
{
  String output = "";
  for (size_t i = 0; i < 64; i++)
  {
    bool one = n & (uint64_t(1) << i);
    output += (one ? "1" : "0");
  }
  return output;
}

bool setDataMapSendStatus(uint8_t id, bool sendstatus = true)
{
  if (id == 1) DataMap.sendStatusAfterPublishCompleted = false;
  if (id < 64)
  {
    if (sendstatus) DataMap.sendIds[0] |= (uint64_t(1) << id);
    else DataMap.sendIds[0] &= ~(uint64_t(1) << id);
  }
  else if (id < 128)
  {
    if (sendstatus) DataMap.sendIds[1] |= (uint64_t(1) << (id - 64));
    else DataMap.sendIds[1] &= ~(uint64_t(1) << (id - 64));
  }
  else return true;
  //DEBUG("id=%d, sendstatus=%d sendids[0]=%s sendids[1]=%s\n", id, sendstatus, uint64_to_binstr(DataMap.sendIds[0]).c_str(), uint64_to_binstr(DataMap.sendIds[1]).c_str());
  return false;
}

int16_t getDataMap(char *key, char *value, int8_t id = -1)
{
  uint8_t idCounter = 0;
  char valuestring[30] = "";

  if (getdatamap_checkandfill(key, value, id, idCounter++, "hostname", WiFi.hostname().c_str())) return --idCounter;

  // Return Status Items
  if (getdatamap_checkandfill(key, value, id, idCounter++, "status",
                              (DataMap.status == DataMapStatus::initializing) ? cF("initializing") :
                              (DataMap.status == DataMapStatus::online) ? cF("online") :
                              (DataMap.status == DataMapStatus::commerror) ? cF("commerror") :
                              (DataMap.status == DataMapStatus::receiving) ? cF("receiving") :
                              (DataMap.status == DataMapStatus::querying) ? cF("querying") :
                              (DataMap.status == DataMapStatus::ready) ? cF("ready") :
                              (DataMap.status == DataMapStatus::rebooting) ? cF("rebooting") :
                              (DataMap.status == DataMapStatus::upgrading) ? cF("upgrading") :
                              "")) return --idCounter;
  
  if (getdatamap_checkandfill(key, value, id, idCounter++, "status/upgrade",
                              (DataMap.status_upgrade == DataMapStatusUpgrade::notchecked) ? cF("not checked") :
                              (DataMap.status_upgrade == DataMapStatusUpgrade::uptodate) ? cF("up to date") :
                              (DataMap.status_upgrade == DataMapStatusUpgrade::httperror) ? cF("http error") :
                              (DataMap.status_upgrade == DataMapStatusUpgrade::upgrading) ? cF("upgrading") :
                              (DataMap.status_upgrade == DataMapStatusUpgrade::incorrectupgradekey) ? cF("incorrect upgradekey") :
                              (DataMap.status_upgrade == DataMapStatusUpgrade::upgradedone) ? cF("upgrade done") :
                              "")) return --idCounter;

  // Return Firmware Items
  String firmwarename = __FILE__;
  firmwarename = firmwarename.substring(firmwarename.lastIndexOf("/") + 1);
  firmwarename = firmwarename.substring(firmwarename.lastIndexOf("\\") + 1);
  firmwarename = firmwarename.substring(0, firmwarename.lastIndexOf("."));
  if (getdatamap_checkandfill(key, value, id, idCounter++, "firmware/name", firmwarename.c_str())) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "firmware/target", FIRMWARE_TARGET)) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "firmware/sourcefile", String(__FILE__).substring(String(__FILE__).lastIndexOf('/') + 1).c_str())) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "firmware/version", ESPMQTT_VERSION)) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "firmware/compiletime", String(String(__DATE__) + " " + __TIME__).c_str())) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "firmware/upgradekey", DataMap.firmware_upgradekey)) return --idCounter;

  // Return System Items
  if (getdatamap_checkandfill(key, value, id, idCounter++, "system/chipid", chipid.c_str())) return --idCounter;
  snprintf(valuestring, 30, cF("%d:%02d:%02d:%02d"), uptime / 86400, (uptime / 3600) % 24, (uptime / 60) % 60, uptime % 60);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "system/uptime", valuestring)) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "system/freeram", String(system_get_free_heap_size()).c_str())) return --idCounter;

  // Return Wifi items
  if (getdatamap_checkandfill(key, value, id, idCounter++, "wifi/state", WiFi.status() == WL_CONNECTED ? "connected" : "disconnected")) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "wifi/localip", WiFi.localIP().toString().c_str())) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "wifi/ssid", WiFi.SSID().c_str())) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "wifi/bssid", WiFi.BSSIDstr().c_str())) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "wifi/rssi", String(WiFi.RSSI()).c_str())) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "wifi/channel", String(WiFi.channel()).c_str())) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "wifi/externalip",  DataMap.externalip)) return --idCounter;

  // Return MQTT items
  if (getdatamap_checkandfill(key, value, id, idCounter++, "mqtt/server", mqtt_server)) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "mqtt/port", String(mqtt_port).c_str())) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "mqtt/ssl", String(mqtt_ssl).c_str())) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "mqtt/state", mqttClient.connected() ? "connected" : "disconnected")) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "mqtt/clientid", mqttClient.getClientId())) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "mqtt/user",  mqtt_username)) return --idCounter;

#ifdef ESPMQTT_SMARTMETER
  if (getdatamap_checkandfill(key, value, id, idCounter++, "electricity/watt", String(smartmeter_DataMap.electricity.watt).c_str())) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "electricity/watt_using", String(smartmeter_DataMap.electricity.watt_using).c_str())) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "electricity/watt_providing", String(smartmeter_DataMap.electricity.watt_providing).c_str())) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%03u"), smartmeter_DataMap.electricity.wh / 1000, smartmeter_DataMap.electricity.wh % 1000);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "electricity/kwh_total", valuestring))return --idCounter;

  snprintf (valuestring, 30, cF("%u.%03u"), smartmeter_DataMap.electricity.wh_used1 / 1000, smartmeter_DataMap.electricity.wh_used1 % 1000);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "electricity/kwh_used1", valuestring)) return --idCounter;
  snprintf (valuestring, 30, cF("%u.%03u"), smartmeter_DataMap.electricity.wh_used2 / 1000, smartmeter_DataMap.electricity.wh_used2 % 1000);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "electricity/kwh_used2", valuestring)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%03u"), smartmeter_DataMap.electricity.wh_provided1 / 1000, smartmeter_DataMap.electricity.wh_provided1 % 1000);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "electricity/kwh_provided1", valuestring)) return --idCounter;
  snprintf (valuestring, 30, cF("%u.%03u"), smartmeter_DataMap.electricity.wh_provided2 / 1000, smartmeter_DataMap.electricity.wh_provided2 % 1000);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "electricity/kwh_provided2", valuestring)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%03u"), smartmeter_DataMap.gas.liter / 1000, smartmeter_DataMap.gas.liter % 1000);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "gas/m3", valuestring)) return --idCounter;
  snprintf (valuestring, 30, cF("%u.%03u"), smartmeter_DataMap.gas.lh / 1000000, smartmeter_DataMap.gas.lh / 1000);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "gas/m3h", valuestring)) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "gas/datetime", smartmeter_DataMap.gas.datetime)) return --idCounter;
#endif

#ifdef ESPMQTT_GROWATT
  if (getdatamap_checkandfill(key, value, id, idCounter++, "inverter/status/value", String(growatt_DataMap.inverter_status_value).c_str(), growatt_DataMap.dataReady)) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "inverter/status", growatt_DataMap.inverter_status, growatt_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growatt_DataMap.pv_1_voltage / 10, growatt_DataMap.pv_1_voltage % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "pv/1/volt", valuestring, growatt_DataMap.dataReady)) return --idCounter;
  snprintf (valuestring, 30, cF("%u.%01u"), growatt_DataMap.pv_2_voltage / 10, growatt_DataMap.pv_2_voltage % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "pv/2/volt", valuestring, growatt_DataMap.dataReady)) return --idCounter;
  snprintf (valuestring, 30, cF("%u.%01u"), growatt_DataMap.pv_power / 10, growatt_DataMap.pv_power % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "pv/watt", valuestring, growatt_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growatt_DataMap.grid_voltage / 10, growatt_DataMap.grid_voltage % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "grid/volt", valuestring, growatt_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growatt_DataMap.grid_current / 10, growatt_DataMap.grid_current % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "grid/amp", valuestring, growatt_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%02u"), growatt_DataMap.grid_frequency / 100, growatt_DataMap.grid_frequency % 100);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "grid/frequency", valuestring, growatt_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growatt_DataMap.grid_power / 10, growatt_DataMap.grid_power % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "grid/watt", valuestring, growatt_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growatt_DataMap.fault_temperature / 10, growatt_DataMap.fault_temperature % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "fault/temperature", valuestring, growatt_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u"), growatt_DataMap.fault_type);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "fault/type", valuestring, growatt_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growatt_DataMap.temperature / 10, growatt_DataMap.temperature % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "temperature", valuestring, growatt_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growatt_DataMap.grid_today_energy / 10, growatt_DataMap.grid_today_energy % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "grid/today/kwh", valuestring, growatt_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growatt_DataMap.grid_total_energy / 10, growatt_DataMap.grid_total_energy % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "grid/total/kwh", valuestring, growatt_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u"), growatt_DataMap.inverter_time);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "inverter/hours", valuestring, growatt_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u"), growatt_DataMap.fanspeed);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "fanspeed", valuestring, growatt_DataMap.dataReady)) return --idCounter;
#endif

#ifdef ESPMQTT_GROWATT_MODBUS
  if (getdatamap_checkandfill(key, value, id, idCounter++, "inverter/status/value", String(growattModbus_DataMap.inverter_status_value).c_str(), growattModbus_DataMap.dataReady)) return --idCounter;
  if (getdatamap_checkandfill(key, value, id, idCounter++, "inverter/status", growattModbus_DataMap.inverter_status, growattModbus_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growattModbus_DataMap.pv_1_voltage / 10, growattModbus_DataMap.pv_1_voltage % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "pv/1/volt", valuestring, growattModbus_DataMap.dataReady)) return --idCounter;
  snprintf (valuestring, 30, cF("%u.%01u"), growattModbus_DataMap.pv_1_current / 10, growattModbus_DataMap.pv_1_current % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "pv/1/amp", valuestring, growattModbus_DataMap.dataReady)) return --idCounter;
  snprintf (valuestring, 30, cF("%u.%01u"), growattModbus_DataMap.pv_1_power / 10, growattModbus_DataMap.pv_1_power % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "pv/1/watt", valuestring, growattModbus_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growattModbus_DataMap.pv_2_voltage / 10, growattModbus_DataMap.pv_2_voltage % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "pv/2/volt", valuestring, growattModbus_DataMap.dataReady)) return --idCounter;
  snprintf (valuestring, 30, cF("%u.%01u"), growattModbus_DataMap.pv_2_current / 10, growattModbus_DataMap.pv_2_current % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "pv/2/amp", valuestring, growattModbus_DataMap.dataReady)) return --idCounter;
  snprintf (valuestring, 30, cF("%u.%01u"), growattModbus_DataMap.pv_2_power / 10, growattModbus_DataMap.pv_2_power % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "pv/2/watt", valuestring, growattModbus_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growattModbus_DataMap.pv_power / 10, growattModbus_DataMap.pv_power % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "pv/watt", valuestring, growattModbus_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growattModbus_DataMap.grid_voltage / 10, growattModbus_DataMap.grid_voltage % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "grid/volt", valuestring, growattModbus_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growattModbus_DataMap.grid_current / 10, growattModbus_DataMap.grid_current % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "grid/amp", valuestring, growattModbus_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%02u"), growattModbus_DataMap.grid_frequency / 100, growattModbus_DataMap.grid_frequency % 100);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "grid/frequency", valuestring, growattModbus_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growattModbus_DataMap.grid_power / 10, growattModbus_DataMap.grid_power % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "grid/watt", valuestring, growattModbus_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growattModbus_DataMap.fault_temperature / 10, growattModbus_DataMap.fault_temperature % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "fault/temperature", valuestring, growattModbus_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u"), growattModbus_DataMap.fault_type);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "fault/type", valuestring, growattModbus_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growattModbus_DataMap.temperature / 10, growattModbus_DataMap.temperature % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "temperature", valuestring, growattModbus_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growattModbus_DataMap.grid_today_energy / 10, growattModbus_DataMap.grid_today_energy % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "grid/today/kwh", valuestring, growattModbus_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u.%01u"), growattModbus_DataMap.grid_total_energy / 10, growattModbus_DataMap.grid_total_energy % 10);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "grid/total/kwh", valuestring, growattModbus_DataMap.dataReady)) return --idCounter;

  snprintf (valuestring, 30, cF("%u"), growattModbus_DataMap.inverter_time / 2);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "inverter/seconds", valuestring, growattModbus_DataMap.dataReady)) return --idCounter;
#endif

#if (defined ESPMQTT_WATERMETER) || (defined ESPMQTT_WATERMETER2)
  snprintf (valuestring, 30, cF("%.1f"), watermeter_getflow());
  if (getdatamap_checkandfill(key, value, id, idCounter++, "water/lmin", valuestring)) return --idCounter;
  snprintf (valuestring, 30, cF("%.3f"), watermeter_getflow() * 0.06);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "water/m3h", valuestring)) return --idCounter;
  snprintf (valuestring, 30, cF("%u"), watermeter_getliters());
  if (getdatamap_checkandfill(key, value, id, idCounter++, "water/liter", valuestring)) return --idCounter;
  snprintf (valuestring, 30, cF("%u.%03u"), watermeter_getliters() / 1000, watermeter_getliters() % 1000);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "water/m3", valuestring)) return --idCounter;
#endif

#ifdef ESPMQTT_WEATHER
  if (weather.temperature != INT16_MAX) snprintf (valuestring, 30, cF("%u.%01u"), weather.temperature / 10, weather.temperature % 10);
  else snprintf (valuestring, 30, "-");
  if (getdatamap_checkandfill(key, value, id, idCounter++, "temperature", valuestring)) return --idCounter;

  snprintf (valuestring, 30, cF("%d"), weather.rain_pulses);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "rain/pulses", valuestring)) return --idCounter;

  snprintf (valuestring, 30, cF("%.1f"), float(weather.rain_pulses) * RAINMETERPULSEMM);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "rain/mm", valuestring)) return --idCounter;

  snprintf (valuestring, 30, cF("%d"), weather.rain_hour_pulses);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "rain/hour/pulses", valuestring)) return --idCounter;

  snprintf (valuestring, 30, cF("%.1f"), float(weather.rain_hour_pulses) * RAINMETERPULSEMM);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "rain/hour/mm", valuestring)) return --idCounter;

  snprintf (valuestring, 30, cF("%d"), weather.rain_lasthour_pulses);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "rain/lasthour/pulses", valuestring)) return --idCounter;

  snprintf (valuestring, 30, cF("%.1f"), float(weather.rain_lasthour_pulses) * RAINMETERPULSEMM);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "rain/lasthour/mm", valuestring)) return --idCounter;

  snprintf (valuestring, 30, cF("%d"), weather.rain_minute_pulses);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "rain/minute/pulses", valuestring)) return --idCounter;

  snprintf (valuestring, 30, cF("%.1f"), float(weather.rain_minute_pulses) * RAINMETERPULSEMM);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "rain/minute/mm", valuestring)) return --idCounter;

  snprintf (valuestring, 30, cF("%d"), weather.rain_lastminute_pulses);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "rain/lastminute/pulses", valuestring)) return --idCounter;

  snprintf (valuestring, 30, cF("%.1f"), float(weather.rain_lastminute_pulses) * RAINMETERPULSEMM);
  if (getdatamap_checkandfill(key, value, id, idCounter++, "rain/lastminute/mm", valuestring)) return --idCounter;
#endif

#ifdef ESPMQTT_DUCOBOX
  if (ducobox_DataMap.fanspeed != UINT16_MAX) snprintf (valuestring, 30, cF("%d"), ducobox_DataMap.fanspeed);
  else snprintf (valuestring, 30, "-");
  if (getdatamap_checkandfill(key, value, id, idCounter++, "1/fanspeed", valuestring)) return --idCounter;

  if (ducobox_DataMap.minfanspeed != UINT16_MAX) snprintf (valuestring, 30, cF("%d"), ducobox_DataMap.minfanspeed);
  else snprintf (valuestring, 30, "-");
  if (getdatamap_checkandfill(key, value, id, idCounter++, "1/minfanspeed", valuestring)) return --idCounter;

  if (ducobox_DataMap.node_4_temperature != INT16_MAX) snprintf (valuestring, 30, cF("%u.%01u"), ducobox_DataMap.node_4_temperature / 10, ducobox_DataMap.node_4_temperature % 10);
  else snprintf (valuestring, 30, "-");
  if (getdatamap_checkandfill(key, value, id, idCounter++, "4/temperature", valuestring)) return --idCounter;

  if (ducobox_DataMap.node_4_co2 != UINT16_MAX) snprintf (valuestring, 30, cF("%d"), ducobox_DataMap.node_4_co2);
  else snprintf (valuestring, 30, "-");
  if (getdatamap_checkandfill(key, value, id, idCounter++, "4/co2", valuestring)) return --idCounter;

  if (ducobox_DataMap.node_26_temperature != INT16_MAX) snprintf (valuestring, 30, cF("%u.%01u"), ducobox_DataMap.node_26_temperature / 10, ducobox_DataMap.node_26_temperature % 10);
  else snprintf (valuestring, 30, "-");
  if (getdatamap_checkandfill(key, value, id, idCounter++, "26/temperature", valuestring)) return --idCounter;

  if (ducobox_DataMap.node_26_humidity != UINT16_MAX) snprintf (valuestring, 30, cF("%d"), ducobox_DataMap.node_26_humidity);
  else snprintf (valuestring, 30, "-");
  if (getdatamap_checkandfill(key, value, id, idCounter++, "26/humidity", valuestring)) return --idCounter;
#endif

#ifdef SONOFFCH
  for (uint8_t i = 0; i < SONOFFCH; i++)
  {
    char topicstring [10];
    snprintf (valuestring, 30, cF("%d"), sonoff_relaystate[i]);
    snprintf(topicstring, 10, "relay/%u", i);
    if (getdatamap_checkandfill(key, value, id, idCounter++, topicstring, valuestring)) return --idCounter;
  }
#endif

#ifdef MH_Z19
  if (mhz19.temperature != INT8_MAX) snprintf (valuestring, 30, cF("%d"), mhz19.temperature);
  else snprintf (valuestring, 30, "-");
  if (mhz19.changed.temperature) 
  {
    mhz19.changed.temperature = false;
    setDataMapSendStatus(idCounter, true);
  }
  if (getdatamap_checkandfill(key, value, id, idCounter++, "mhz19/temperature", valuestring)) return --idCounter;

  if (mhz19.co2 != UINT16_MAX) snprintf (valuestring, 30, cF("%d"), mhz19.co2);
  else snprintf (valuestring, 30, "-");
  if (mhz19.changed.co2) 
  {
    mhz19.changed.co2 = false;
    setDataMapSendStatus(idCounter, true);
  }
  if (getdatamap_checkandfill(key, value, id, idCounter++, "mhz19/co2", valuestring)) return --idCounter;
#endif

#ifdef DHTPIN
  if (dht.temperature != INT16_MAX)
  {
    snprintf (valuestring, 30, cF("%.1f"), (float)dht.temperature/10);
    DataMap.status = DataMapStatus::online;
    setDataMapSendStatus(0);
  }
  else 
  {
    snprintf (valuestring, 30, "-");
    DataMap.status = DataMapStatus::commerror;
    setDataMapSendStatus(0);
  }
  if (dht.changed.temperature) 
  {
    dht.changed.temperature = false;
    setDataMapSendStatus(idCounter, true);
  }
  if (getdatamap_checkandfill(key, value, id, idCounter++, "dht/temperature", valuestring)) return --idCounter;

  if (dht.humidity != INT16_MAX) snprintf (valuestring, 30, cF("%.1f"), (float)dht.humidity/10);
  else snprintf (valuestring, 30, "-");
  if (dht.changed.humidity) 
  {
    dht.changed.humidity = false;
    setDataMapSendStatus(idCounter, true);
  }
  if (getdatamap_checkandfill(key, value, id, idCounter++, "dht/humidity", valuestring)) return --idCounter;

  if (dht.heatindex != INT16_MAX) snprintf (valuestring, 30, cF("%.1f"), (float)dht.heatindex/10);
  else snprintf (valuestring, 30, "-");
  if (dht.changed.heatindex) 
  {
    dht.changed.heatindex = false;
    setDataMapSendStatus(idCounter, true);
  }
  if (getdatamap_checkandfill(key, value, id, idCounter++, "dht/heatindex", valuestring)) return --idCounter;
#endif

return -1;
  yield();
}

int16_t getDatamapId(String key)
{
  char value[30] = "";
  char keyarray[30] = "";
  for (uint8_t id = 0; id < getDataMapLength(); id++)
  {
    strcpy (keyarray, "");
    getDataMap(keyarray, value, id);
    if (strcmp(keyarray, key.c_str()) == 0) return id;
  }
  return -1;
}

String getDataMapKey(uint8_t id)
{
  char value[30] = "";
  char keyarray[30] = "";
  getDataMap(keyarray, value, id);
  return String(keyarray);
}

String getDataMapValue(uint8_t id)
{
  char value[30] = "";
  char keyarray[30] = "";
  getDataMap(keyarray, value, id);
  return String(value);
}


String getDataMapValue(const char *key)
{
  char value[30] = "";
  char keyarray[30] = "";
  strcpy(keyarray, key);
  getDataMap(keyarray, value);
  return String(value);
}

String getDataMap(const char *key)
{
  return getDataMapValue(key);
}


int16_t getDataMapId(const char *key)
{
  char value[30] = "";
  char keyarray[30] = "";
  strcpy(keyarray, key);
  DEBUG("getDataMapId key=%s, keyarray=%s\n", key, keyarray);
  return getDataMap(keyarray, value);
}

bool setDataMapSendStatus(const char *key, bool sendstatus = true)
{
  int16_t id = getDataMapId(key);
  DEBUG("setDataMapSendStatus key=%s, id=%d\n", key, id);
  if (id < 0) return 1;
  return setDataMapSendStatus(id, sendstatus);
}

String getdatamap(String key)
{
  return getDataMapValue(key.c_str());
}

void showdatamap()
{
  char key[30];
  char value[30];
  uint8_t id = 0;
  while (getDataMap(key, value, id++) >= 0)
  {
    DEBUG("%d %s=%s\n", id, key, value);
    strcpy (key, "");
    yield();
  }

  char buf[256];
  snprintf(buf, 255, "snprintf-s: %s", (char *)F("PROGMEM-STRING"));
  DEBUG("%s\n", buf);
}

#ifdef  ESPMQTT_BBQTEMP
double MAX6675_readCelsius(uint8_t cs)
{
  uint16_t v;
  SPI.beginTransaction(SPISettings(100, MSBFIRST, SPI_MODE0));
  digitalWrite(cs, LOW);
  delay(10);
  v = SPI.transfer(0x00);
  v <<= 8;
  v |= SPI.transfer(0x00);
  digitalWrite(cs, HIGH);
  delay(10);
  SPI.endTransaction();

  if (v & 0x4) {
    // uh oh, no thermocouple attached!
    return NAN;
  }

  v >>= 3;

  return v * 0.25;
}

#endif



#ifdef ESPMQTT_OBD2
void obd2_writeserial (String data)
{
  DEBUG ("Writing to ODB2:\"%s\"\n", data.c_str());
  data += "\r";
  Serial.write(data.c_str());
}

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
  while (Serial.available() > 0)
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


void startWifiAP()
{
  DEBUG_E("Starting WiFi Accesspoint\n");
  bool apok = 0;
  if ((apok = WiFi.softAP(WiFi.hostname().c_str(), DEFAULT_PASSWORD, 6, 0)))
  {
    mainstate.accesspoint = true;
    DEBUG_E("WiFi Accesspoint Activated\n");
    static uint8 routermode = 0;
    wifi_softap_set_dhcps_offer_option(OFFER_ROUTER, &routermode);
    WiFi.mode(WIFI_AP);
    snprintf(esp_password, ESP_PASSWORD_SIZE, "%s", DEFAULT_PASSWORD);
    mainstate.defaultpassword = true;
    ArduinoOTA.setPassword(esp_password);
    Debug.setPassword(esp_password);
#ifdef ESPMQTT_TUYA_2GANGDIMMERV2
    //tuya_apmode();
#endif
  }
  else DEBUG_E("Starting WiFi Accesspoint failed.");
}

void stopWifiAP()
{
  DEBUG_I("Stopping accesspoint, connecting to accesspoint.\n");
  mainstate.accesspoint = false;
  connectToWifi();
}

void connectToWifi()
{
  WiFi.hostname(esp_hostname);
  ArduinoOTA.setHostname(esp_hostname);
  ArduinoOTA.setPassword(esp_password);
  Debug.setPassword(esp_password);

  // If no ssid was configured start accesspoint
  if (0 != wifissid[0])
  {
    if (!mainstate.wificonnected && !mainstate.accesspoint)
    {
      DEBUG_I("Connecting to Wi-Fi: SSID='%s' HOSTNAME='%s'...\n", wifissid, esp_hostname);

      WiFi.persistent(true); // Save wifi credentials for other firmware
      WiFi.setAutoReconnect(false); // We handle reconnect our self
      WiFi.setSleepMode(WIFI_MODEM_SLEEP);
      //WiFi.setSleepMode(WIFI_NONE_SLEEP); // When sleep is on regular disconnects occur https://github.com/esp8266/Arduino/issues/5083
      WiFi.setOutputPower(20);        // 10dBm == 10mW, 14dBm = 25mW, 17dBm = 50mW, 20dBm = 100mW
      WiFi.mode(WIFI_STA);

      mainstate.accesspoint = false;
      mainstate.defaultpassword = false;

      WiFi.scanNetworksAsync(wifiScanReady); // Search for strongest accesspoint and connect
    }
  }
  else
  {
    startWifiAP();
  }
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
  const uint8_t nr_of_subsribe_topics = 30;

  static int32_t nextpacketid = 1;
  static uint16_t nextsubscribe = 0;
  const char *subscribetopic = "";
  DEBUG_V("mqttdosubscriptions (packetId=%d, nextpacketId=%d, nextsubscribe=%d)\n", packetId, nextpacketid, nextsubscribe);

  if (packetId == -1)
  {
    nextpacketid = -1;
    nextsubscribe = 0;
  }
  else if (packetId == nextpacketid)
  {
    nextsubscribe++;
    nextpacketid = -1;
  }
  else return;

  while ((0 == subscribetopic[0]) && (nextsubscribe <= nr_of_subsribe_topics))
  {
    //DEBUG("mqttdosubscriptions while nextsubscribe=%d\n", nextsubscribe);
    switch (nextsubscribe)
    {
#ifdef ESPMQTT_OPENTHERM
      case 0: subscribetopic = "setthermostattemporary"; break;
      case 1: subscribetopic = "setthermostatcontinue"; break;
      case 2: subscribetopic = "setchwatertemperature"; break;
      case 3: subscribetopic = "setmaxmodulationlevel"; break;
      case 4: subscribetopic = "setoutsidetemperature"; break;
#endif
#ifdef ESPMQTT_DUCOBOX
      case 5: subscribetopic = "setfan"; break;
#endif
#ifdef ESPMQTT_DIMMER
      case 6: subscribetopic = "setdimvalue"; break;
      case 7: subscribetopic = "setdimstate"; break;
#endif
#ifdef ESPMQTT_SONOFFBULB
      case 8: subscribetopic = "setcolor"; break;
#endif
#ifdef ESPMQTT_AMGPELLETSTOVE
      case 9: subscribetopic = "setonoff"; break;
      case 10: subscribetopic = "setpower"; break;
      case 11: subscribetopic = "settemperature"; break;
#endif
#ifdef SONOFFCH
      case 12:  if (0 < SONOFFCH) subscribetopic = "setrelay/0"; break;
      case 13:  if (1 < SONOFFCH) subscribetopic = "setrelay/1"; break;
      case 14:  if (2 < SONOFFCH) subscribetopic = "setrelay/2"; break;
      case 15:  if (3 < SONOFFCH) subscribetopic = "setrelay/3"; break;
#endif
#ifdef  ESPMQTT_SONOFF_FLOORHEATING
      case 16: subscribetopic = "setvalve"; break;
#endif
#ifdef ESPMQTT_QSWIFIDIMMERD01
      case 17:  subscribetopic = "setdimvalue"; break;
      case 18:  subscribetopic = "setdimstate"; break;
#endif
#if defined(ESPMQTT_QSWIFIDIMMERD02) || defined(ESPMQTT_TUYA_2GANGDIMMERV2)
      case 19:  subscribetopic = "setdimvalue/0"; break;
      case 20:  subscribetopic = "setdimvalue/1"; break;
      case 21:  subscribetopic = "setdimstate/0"; break;
      case 22:  subscribetopic = "setdimstate/1"; break;
#endif
#ifdef ESPMQTT_BHT002
      case 23: subscribetopic = "setsetpoint"; break;
#endif
      case 24: subscribetopic = "startfirmwareupgrade"; break;
#ifdef ESPMQTT_QSWIFISWITCH1C
      case 25: subscribetopic = "setrelay"; break;
#endif
#ifdef ESPMQTT_QSWIFISWITCH2C
      case 26: subscribetopic = "setrelay/0"; break;
      case 27: subscribetopic = "setrelay/1"; break;
#endif
      default: break;
    }
    nextsubscribe++;
  }

  if (0 == subscribetopic[0])
  {
    DEBUG_I("Subscribing to mqtt topics finished successfull\n");
    mainstate.mqttsubscribed = true;
    triggers.mqttsubscribed = true; // When subscription has finished start publishing of datamap
  }
  else
  {
    static char subscribetopicarray[100];  // We need this static variable because mqttclient.subscribe uses a pointer
    snprintf (subscribetopicarray, 50, "%s%s", mqtt_topicprefix, subscribetopic);
    DEBUG_D("MQTT Subscribing to: %s\n", subscribetopicarray);
    nextpacketid = mqttClient.subscribe(subscribetopicarray , 1);
  }
  DEBUG_V("mqttdosubscriptions end nextpacketid=%d\n", nextpacketid);
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
  DEBUG_V ("Subscribe acknowledged packetid=%d qos=%d\n", packetId, qos);
  mainstate.mqttlastsubscribedpacketid = packetId;
}


void onMqttPublish(uint16_t packetId)
{
  mainstate.mqttlastpublishedpacketid = packetId;
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties, size_t len, size_t, size_t) {
  String payloadstring = "";
#define TOPIC_BUFFER_SIZE 50
  char topic_buffer[TOPIC_BUFFER_SIZE];
  for (unsigned int i = 0; i < len; i++)
  {
    payloadstring += char(payload[i]);
  }
  String topicstring = topic;
  DEBUG_D("MQTT RECEIVED topic=%s payload=%s\n", topic, payloadstring.c_str());

#ifdef SONOFFCH
  for (byte i = 0; i < SONOFFCH; i++)
  {
    if (String(topic) == String(mqtt_topicprefix) + "setrelay/" + i)
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
  if (String(topic) == String(mqtt_topicprefix + "setmaxmodulationlevel"))
  {
    DEBUG_I("RECEIVED SETMAXMODULATIONLEVEL %s\n", payloadstring.c_str());
    opentherm_setmaxmodulationlevel(payloadstring.toInt());
  }
  if (String(topic) == String(mqtt_topicprefix + "setoutsidetemperature"))
  {
    DEBUG_I("RECEIVED SETOUTSIDETEMPERATURE %s\n", payloadstring.c_str());
    opentherm_setoutsidetemperature(payloadstring.toFloat());
  }
#endif

#ifdef  ESPMQTT_SONOFF_FLOORHEATING
  if (String(topic) == String(mqtt_topicprefix) + "setvalve") floorheating_valveon = payloadstring == "1" ? true : false;
#endif


#ifdef  ESPMQTT_DUCOBOX
  if (String(topic) == String(mqtt_topicprefix) + "setfan") ducobox_setfan(payloadstring.toInt());
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

#ifdef ESPMQTT_QSWIFIDIMMERD01
  if (topicstring == mqtt_topicprefix + "setdimvalue") qswifidimmer_setdimvalue(payloadstring.toInt());
  if (topicstring == mqtt_topicprefix + "setdimstate") qswifidimmer_setdimstate(payloadstring.toInt() ? 1 : 0);
#endif
#ifdef ESPMQTT_QSWIFIDIMMERD02
  if (topicstring == mqtt_topicprefix + "setdimvalue/0") qswifidimmer_setdimvalue(payloadstring.toInt(), 0);
  if (topicstring == mqtt_topicprefix + "setdimvalue/1") qswifidimmer_setdimvalue(payloadstring.toInt(), 1);
  if (topicstring == mqtt_topicprefix + "setdimstate/0") qswifidimmer_setdimstate(payloadstring.toInt(), 0);
  if (topicstring == mqtt_topicprefix + "setdimstate/1") qswifidimmer_setdimstate(payloadstring.toInt(), 1);
#endif
#ifdef ESPMQTT_QSWIFISWITCH1C
  if (topicstring == mqtt_topicprefix + "setrelay") qswifiswitch.setRelay(payloadstring.toInt());
#endif
#ifdef ESPMQTT_QSWIFISWITCH2C
  if (topicstring == mqtt_topicprefix + "setrelay/0") qswifiswitch.setRelay(0, payloadstring.toInt());
  if (topicstring == mqtt_topicprefix + "setrelay/1") qswifiswitch.setRelay(1, payloadstring.toInt());
#endif

  snprintf(topic_buffer, TOPIC_BUFFER_SIZE, "%sstartfirmwareupgrade", mqtt_topicprefix);
  if (strcmp(topic, topic_buffer) == 0)
  {
    triggers.firmwareupgrade = payloadstring;
  }
#ifdef ESPMQTT_BHT002
  if (topicstring == mqtt_topicprefix + "setsetpoint") bht002_setsetpoint(payloadstring.toInt());
#endif

#ifdef ESPMQTT_TUYA_2GANGDIMMERV2
  if (topicstring == mqtt_topicprefix + "setdimvalue/0") tuya_2gangdimmerv2_setdimvalue(payloadstring.toInt(), 0);
  if (topicstring == mqtt_topicprefix + "setdimvalue/1") tuya_2gangdimmerv2_setdimvalue(payloadstring.toInt(), 1);
  if (topicstring == mqtt_topicprefix + "setdimstate/0") tuya_2gangdimmerv2_setdimstate(payloadstring.toInt(), 0);
  if (topicstring == mqtt_topicprefix + "setdimstate/1") tuya_2gangdimmerv2_setdimstate(payloadstring.toInt(), 1);
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


void initMqtt()
{
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onPublish(onMqttPublish);
  //#define MQTT_SERVER_FINGERPRINT {0x3F, 0x80, 0xCF, 0x16, 0xD9, 0x43, 0x3B, 0x92, 0xB6, 0x3A, 0x0A, 0x02, 0xFE, 0x27, 0x0B, 0x60, 0xC1, 0x9A, 0x8B, 0xB1}
  //mqttClient.addServerFingerprint((const uint8_t[])MQTT_SERVER_FINGERPRINT);
}

void connectToMqtt()
{
  // Because mqtt lib uses a pointer to const char[], we have to make a static variables
  static char willtopic[MQTT_TOPICPREFIX_SIZE + 6] = "";
  snprintf (willtopic, MQTT_TOPICPREFIX_SIZE + 6, "%sstatus", mqtt_topicprefix);

  DEBUG_I("Connecting to Mqtt Username=\"%s\" Password=\"%s\" Server=\"%s\" Port=%d Secure=%d\n", mqtt_username, mqtt_password, mqtt_server, mqtt_port, mqtt_ssl);
  if (!mainstate.mqttconnected)
  {
    mqttClient.disconnect();
    mqttClient.setCredentials(mqtt_username, mqtt_password);
    mqttClient.setServer(mqtt_server, mqtt_port);
    mqttClient.setWill(willtopic, 0, 1, "offline");
    mqttClient.setSecure(mqtt_ssl);
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
  mainstate.mqttsubscribed = false;
  mainstate.mqttready = false;
  triggers.mqttdisconnected = true;
}



void initSerial()
{
  Serial.setRxBufferSize(100);
#if defined(MH_Z19) || defined( ESPMQTT_OPENTHERM) || defined( ESPMQTT_GROWATT) || defined( ESPMQTT_GOODWE)
  Serial.setDebugOutput(false);
  Serial.begin(9600);  //Init serial 9600 baud
#elif defined ( ESPMQTT_SONOFFPOWR2)
  Serial.setDebugOutput(false);
  Serial.begin(4800, SERIAL_8E1);
#elif defined ( ESPMQTT_AMGPELLETSTOVE)
  Serial.begin(115200); //Init serial 115200 baud
#elif defined ( ESPMQTT_SDM120) || defined (ESPMQTT_DDM18SD)
  Serial.setDebugOutput(false);
  sdm.begin();
#elif defined ( ESPMQTT_OBD2)
  Serial.setDebugOutput(false);
  Serial.begin(38400, SERIAL_8N1);
#elif defined ( ESPMQTT_ZMAI90)
  Serial.setDebugOutput(false);
  Serial.begin(9600, SERIAL_8E1);
#elif defined (ESPMQTT_SMARTMETER) || defined (QSWIFIDIMMERCHANNELS) || defined (ESPMQTT_BHT002)
  Serial.setDebugOutput(false);
  // do nothing, smartmeter initializes serial in init function.
#elif defined (ESPMQTT_NOSERIAL_DEBUG)
  // do not initialize serial
#else
  Serial.begin(115200); //Init serial 115200 baud
#endif

}
#ifdef DHTPIN
void update_dht()
{
  static uint8 errors = 4;
  float rh = dhtdevice.readHumidity();
  // Read temperature as Celsius (the default)
  float temp = dhtdevice.readTemperature();
#ifdef DHTTEMPOFFSET
  temp += DHTTEMPOFFSET;
#endif

  float hi = dhtdevice.computeHeatIndex(temp, rh, false);

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
    dht.changed.temperature = dht.temperature != (int)(temp * 10);
    dht.temperature = temp * 10;

    dht.changed.humidity = dht.humidity != (int)(rh * 10);
    dht.humidity = rh * 10;

    dht.changed.heatindex = dht.heatindex != (int)(hi * 10);
    dht.heatindex = hi * 10;
  }

  if (errors == 5)
  {
    dht.temperature = INT16_MAX;
    dht.humidity = UINT16_MAX;
    dht.heatindex = INT16_MAX;
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

#ifdef ONEWIREPIN
void update_onewire()
{
  DEBUG_V("Requesting DS18B20 temperatures...\n");
  oneWireSensors.requestTemperatures();
  int temperature;
#ifdef  ESPMQTT_SONOFFTH
  temperature = (oneWireSensors.getTempC(onewire_address) * 10);
  DEBUG_I("Temperature=%u.%01u\n", temperature / 10, temperature % 10);
  if ((temperature == -1270) || (temperature == 850)) temperature = INT16_MAX;
  if (sonoffth.temperature != temperature)
  {
    setDataMapSendStatus(DATAMAP_BASELENGTH, true);
    sonoffth.temperature = temperature;
  }
#endif
#ifdef  ESPMQTT_OPENTHERM
  temperature = (oneWireSensors.getTempC(onewire_chReturnWaterThermometer) * 10);
  DEBUG_I("ch return water Temperature=%u.%01u\n", temperature / 10, temperature % 10);
  if ((temperature == -1270) || (temperature == 850)) temperature = INT16_MAX;
  if (opentherm.onewire_ch_returnwatertemperature != temperature)
  {
    setDataMapSendStatus(DATAMAP_BASELENGTH, true);
    opentherm.onewire_ch_returnwatertemperature = temperature;
  }
  temperature = (oneWireSensors.getTempC(onewire_dcwSupplyWaterThermometer) * 10);
  DEBUG_I("dcw supply water Temperature=%u.%01u\n", temperature / 10, temperature % 10);
  if ((temperature == -1270) || (temperature == 850)) temperature = INT16_MAX;
  if (opentherm.onewire_dcw_returnwatertemperature != temperature)
  {
    setDataMapSendStatus(DATAMAP_BASELENGTH + 1, true);
    opentherm.onewire_dcw_returnwatertemperature = temperature;
  }
#endif
#ifdef  ESPMQTT_WEATHER
  temperature = (oneWireSensors.getTempC(onewire_OutsideAddress) * 10);
  DEBUG_I("Outside Temperature=%u.%01u\n", temperature / 10, temperature % 10);
  if ((temperature == -1270) || (temperature == 850)) temperature = INT16_MAX;
  if (weather.temperature != temperature)
  {
    setDataMapSendStatus(DATAMAP_BASELENGTH, true);
    weather.temperature = temperature;
  }
#endif
#ifdef  ESPMQTT_SONOFF_FLOORHEATING
  temperature = (oneWireSensors.getTempC(onewire_OutsideAddress) * 10);
  DEBUG_I("Floor Water Temperature=%u.%01u\n", temperature / 10, temperature % 10);
  if ((temperature == -1270) || (temperature == 850)) temperature = INT16_MAX;
  if (temperature != floorheating.temperature)
  {
    setDataMapSendStatus(DATAMAP_BASELENGTH, true);
    floorheating.temperature = temperature;
  }
  bool floorheatingrelayon = false;
  // When temperature exeeds maximum prevent pump from switching on
  if (temperature != INT16_MAX)
  {
    if (temperature < SONOFF_FLOORHEATING_TEMPMAX)
    {
      // Every 24 hours run the pump for 1 minute to prevent locking
      if ((int)(uptime / 60) % 1440 == 0)
      {
        floorheatingrelayon = true;
      }
      // Only if floorheating temperature as measured correctly put valve on if requested
      else
      {
        floorheatingrelayon = floorheating_valveon;
      }
    }
  }
#ifdef SONOFFCHINVERSE
  digitalWrite(sonoff_relays[0], floorheatingrelayon ? false : true); // Set floorheating
#else
  digitalWrite(sonoff_relays[0], floorheatingrelayon); // Set floorheating
#endif
#ifdef SONOFF_LEDS
  digitalWrite(sonoff_leds[0], sonoff_ledinverse == floorheatingrelayon ? 0 : 1);
#endif
#endif
  yield();
}
#endif

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
  display.drawString(OLEDX, lcdline += 9, "TEMP: " + String(float(dht.temperature) / 10 ,1) + " °C");
  display.drawString(OLEDX, lcdline += 9, "RH: " + String(float(dht.humidity) / 10 ,1) + " %");
  display.drawString(OLEDX, lcdline += 9, "HI: " + String(float(dht.heatindex) / 10 ,1) + " °C");
#else
  display.drawString(OLEDX, lcdline += 9, "T: " + String(float(dht.temperature) / 10 ,1) + " °C");
  display.drawString(OLEDX, lcdline += 9, "RH: " + String(float(dht.humidity) / 10 ,1) + " %");
  display.drawString(OLEDX, lcdline += 9, "HI: " + String(float(dht.heatindex) / 10 ,1) + " °C");
#endif
#endif


#if defined(MH_Z19)
#ifndef OLEDSMALL
  display.drawString(OLEDX, lcdline += 9, "CO2: " + String(mhz19.co2) + " ppm");
  display.drawString(OLEDX, lcdline += 9, "MHZ_TEMP: " + String(mhz19.temperature) + " °C");
#else
  display.drawString(OLEDX, lcdline += 9, "CO2: " + String(mhz19.co2) + " ppm");
  display.drawString(OLEDX, lcdline += 9, "MT: " + String(mhz19.temperature) + " °C");
#endif
#endif

#if defined(ESPMQTT_BBQTEMP)
  display.setFont(ArialMT_Plain_16);
  display.drawString(OLEDX, lcdline += 20, "1:" + getdatamap("temperature/0"));
  display.drawString(OLEDX, lcdline += 16, "2:" + getdatamap("temperature/1"));
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
      snprintf (esp_password, ESP_PASSWORD_SIZE, "%s", DEFAULT_PASSWORD);
      DEBUG_I("Web Password defaulted to esplogin until reboot!\n");
      mainstate.defaultpassword = true;
    }

    if (flashbuttontimer == 6) // After 6 seconds start access point
    {
      flashbuttonstatus = 1;
      startWifiAP();
    }

    if (flashbuttontimer == 10) ESP.reset();
  }
}
#endif

#ifdef ESPMQTT_ZMAI90
void zmai90_handle()
{
  static bool requestsend = 0;
  if (uptime % 10 == 0)
  {
    if (!requestsend)
    {
      requestsend = 1;
      uint8_t cmd[9] = {0xFE, 0x01, 0x0F, 0x08, 0x00, 0x00, 0x00, 0x1C};
      // command to ask for data

      DEBUG_V("Sending ZMAI request packet...\n");

      Serial.flush();
      Serial.write(cmd, 9); //request data
      zmai90pointer = 0;
    }
  }
  else
  {
    requestsend = 0;
  }

  while (Serial.available() > 0) {
    uint8_t zmai90data = Serial.read();
    DEBUG ("ZMAI90DATA=%d:%02X\n", zmai90pointer, zmai90data);
    char str[3];
    snprintf(str, 3, cF("%02X"), zmai90data);
    zmai90data = atoi(str);
    static uint32_t zmai90value = 0;
    switch (zmai90pointer)
    {
      case 3:
        zmai90value = zmai90data;
        break;
      case 4:
        zmai90value += zmai90data * 100;
        break;
      case 5:
        zmai90value += zmai90data * 10000;
        break;
      case 6:
        zmai90value += zmai90data * 1000000;
        putdatamap ("energy/kwh", String((double)zmai90value / 100, 2));
        break;

      case 7:
        zmai90value = zmai90data;
        break;
      case 8:
        zmai90value += zmai90data * 100;
        break;
      case 9:
        zmai90value += zmai90data * 10000;
        break;
      case 10:
        zmai90value += zmai90data * 1000000;
        putdatamap ("voltage", String((double)zmai90value / 10, 1));
        break;

      case 11:
        zmai90value = zmai90data;
        break;
      case 12:
        zmai90value += zmai90data * 100;
        break;
      case 13:
        zmai90value += zmai90data * 10000;
        break;
      case 14:
        zmai90value += zmai90data * 1000000;
        putdatamap ("current", String((double)zmai90value / 10000, 4));
        break;

      case 15:
        zmai90value = zmai90data;
        break;
      case 16:
        zmai90value += zmai90data * 100;
        putdatamap ("frequency", String((double)zmai90value / 100, 2));
        break;


      case 19:
        zmai90value = zmai90data;
        break;
      case 20:
        zmai90value += zmai90data * 100;
        break;
      case 21:
        zmai90value += zmai90data * 10000;
        break;
      case 22:
        zmai90value += zmai90data * 1000000;
        putdatamap ("power/active", String((double)zmai90value / 100, 1));
        break;

      case 23:
        zmai90value = zmai90data;
        break;
      case 24:
        zmai90value += zmai90data * 100;
        break;
      case 25:
        zmai90value += zmai90data * 10000;
        break;
      case 26:
        zmai90value += zmai90data * 1000000;
        putdatamap ("power/reactive", String((double)zmai90value / 100, 1));
        break;

      case 27:
        zmai90value = zmai90data;
        break;
      case 28:
        zmai90value += zmai90data * 100;
        break;
      case 29:
        zmai90value += zmai90data * 10000;
        break;
      case 30:
        zmai90value += zmai90data * 1000000;
        putdatamap ("power/apparent", String((double)zmai90value / 100, 1));
        break;
    }
    zmai90pointer++;
  }
}
#endif

#if  (defined ESPMQTT_WATERMETER) || (defined ESPMQTT_WATERMETER2)
void espmqtt_watermeter_handle()
{
  if (watermeter_handle())
  {
    for (uint8_t bitpointer = 0;  bitpointer < 4; bitpointer++)
    {
      setDataMapSendStatus(DATAMAP_BASELENGTH + bitpointer, true);
    }
    DataMap.status = DataMapStatus::online;
  }
}
#endif


#ifdef  ESPMQTT_SONOFFPOWR2
void sonoffpow2_handle()
{
  static uint8_t serbuffer[24];
  static uint8_t serpointer = 0;
  while (Serial.available() > 0)
  {
    uint8_t serval = Serial.read();
    if ((serpointer >= 0) && (serpointer < 24))
    {
      serbuffer[serpointer] = serval;
      serpointer++;
    }
    if ((((serbuffer[0] == 0x55) || ((serbuffer[0] & 0xfc) == 0xf0))) && (serbuffer[1] == 0x5A)) // Search for header and check if current and voltage cycle do not exceed range
    {
      if (serpointer == 24) // If all 24 bytes are received process message
      {
        uint32_t valueco = 0;
        uint32_t valueca = 0;
        uint8_t statusbyte = 0;
        uint8_t checksum = 0;

        if ((serbuffer[0] & 0xf0) == 0xf0) statusbyte = (serbuffer[0]  & 0x0f); // Extract statusbits from packet header if packetheader starts with 0xF0

        for (serpointer = 2; serpointer < 23; serpointer++) checksum += serbuffer[serpointer]; // calculate checksum

        if (checksum == serbuffer[23]) // If calculated checksum matches the received checksum calculate values
        {
          if (serbuffer[20] & 0x30) // Check if voltage cycle is completed
          {
            // Calculate voltage
            valueco = (uint32_t(serbuffer[2]) << 16) + (uint16_t(serbuffer[3]) << 8) + serbuffer[4];
            valueca = (uint32_t(serbuffer[5]) << 16) + (uint16_t(serbuffer[6]) << 8) + serbuffer[7];
            if (valueca > 0) voltval = double(double(valueco) / double(valueca));
            else voltval = 0;
          }

          if (serbuffer[20] & 0x10) // Check if power cycle is completed
          {
            // Calculate power
            valueco = (uint32_t(serbuffer[14]) << 16) + (uint16_t(serbuffer[15]) << 8) + serbuffer[16];
            valueca = (uint32_t(serbuffer[17]) << 16) + (uint16_t(serbuffer[18]) << 8) + serbuffer[19];
            if (valueca > 0) powerval = double(double(valueco) / double(valueca));
            else powerval = 0;
            if (statusbyte & 0x02) powerval = 0; // When bit 1 of statusbyte is 1 powercycle exeeds range and power is 0
          }

          if (serbuffer[20] & 0x20)// Check if current cycle is completed
          {
            // Calculate Current
            valueco = (uint32_t(serbuffer[8]) << 16) + (uint16_t(serbuffer[9]) << 8) + serbuffer[10];
            valueca = (uint32_t(serbuffer[11]) << 16) + (uint16_t(serbuffer[12]) << 8) + serbuffer[13];
            if (valueca > 0) currentval = double(double(valueco) / double(valueca));
            else currentval = 0;
            if (powerval == 0) currentval = 0; // If power is 0, current is also 0
          }
        }
        serpointer = 0;
      }
    }
    else // When header not found move second byte to first byte and fill second byte with byte from serial on next round
    {
      serbuffer[0] = serbuffer[1];
      serpointer = 1;
    }
    if (serpointer == 24) // If serbuffer is full flush serial and reset serpointer
    {
      Serial.flush();
      serpointer = 0;
    }
  }
}
#endif

#ifdef RAINMETERPIN
void rainmeter_handle()
{
  static uint32_t rainpinmillis = 0;
  bool rainpinstate = 0;
  static bool count = 0;
  rainpinstate = digitalRead(RAINMETERPIN);
  if ((rainpinstate == 1) && (millis() - 50 > rainpinmillis) && count) // Pulse has to settle for 50ms before counting for filtering spikes
  {
    weather.rain_pulses++;
    weather.rain_hour_pulses++;
    weather.rain_minute_pulses++;
    count = 0;

    setDataMapSendStatus(DATAMAP_BASELENGTH + 1, true);
    setDataMapSendStatus(DATAMAP_BASELENGTH + 2, true);
    setDataMapSendStatus(DATAMAP_BASELENGTH + 3, true);
    setDataMapSendStatus(DATAMAP_BASELENGTH + 4, true);
    setDataMapSendStatus(DATAMAP_BASELENGTH + 7, true);
    setDataMapSendStatus(DATAMAP_BASELENGTH + 8, true);
  }
  if (rainpinstate == 0)
  {
    rainpinmillis = millis();
    count = 1;
  }

  static bool hourreg = false;
  if ((uptime % 3600) == 0)
  {
    if (!hourreg)
    {
      if ((weather.rain_lasthour_pulses > 0) || (weather.rain_hour_pulses > 0))
      {
        setDataMapSendStatus(DATAMAP_BASELENGTH + 3, true);
        setDataMapSendStatus(DATAMAP_BASELENGTH + 4, true);
        setDataMapSendStatus(DATAMAP_BASELENGTH + 5, true);
        setDataMapSendStatus(DATAMAP_BASELENGTH + 6, true);
      }
      weather.rain_lasthour_pulses = weather.rain_hour_pulses;
      weather.rain_hour_pulses = 0;
      hourreg = true;
    }
  }
  else hourreg = false;

  static bool minutereg = false;
  if ((uptime % 60) == 0)
  {
    if (!minutereg)
    {
      if ((weather.rain_lastminute_pulses > 0) || (weather.rain_minute_pulses > 0))
      {
        setDataMapSendStatus(DATAMAP_BASELENGTH + 7, true);
        setDataMapSendStatus(DATAMAP_BASELENGTH + 8, true);
        setDataMapSendStatus(DATAMAP_BASELENGTH + 9, true);
        setDataMapSendStatus(DATAMAP_BASELENGTH + 10, true);
      }
      weather.rain_lastminute_pulses = weather.rain_minute_pulses;
      weather.rain_minute_pulses = 0;
      minutereg = true;
    }
  }
  else minutereg = false;

  digitalWrite(NODEMCULEDPIN, rainpinstate);
}
#endif

#ifdef  ESPMQTT_SDM120
void sdm120_readnextregister()
{
  static uint8_t sdmreadcounter = 1;
  double value = NAN;

  switch (sdmreadcounter)
  {
    case 1:
      putdatamap("status", "querying");
      putdatamap("voltage", doubletostring(sdm.readVal(SDM120CT_VOLTAGE), 2));
      break;
    case 2:
      putdatamap("current", doubletostring(sdm.readVal(SDM120CT_CURRENT), 2));
      break;
    case 3:
      putdatamap("power", doubletostring(sdm.readVal(SDM120CT_POWER), 2));
      break;
    case 4:
      putdatamap("power/apparant", doubletostring(sdm.readVal(SDM120CT_APPARENT_POWER), 2));
      break;
    case 5:
      putdatamap("power/reactive", doubletostring(sdm.readVal(SDM120CT_REACTIVE_POWER), 2));
      break;
    case 6:
      putdatamap("frequency", doubletostring(sdm.readVal(SDM120CT_FREQUENCY), 2));
      break;
    case 7:
      putdatamap("powerfactor", doubletostring(sdm.readVal(SDM120CT_POWER_FACTOR), 2));
      break;
    case 8:
      putdatamap("energy/active/import", doubletostring(sdm.readVal(SDM120CT_IMPORT_ACTIVE_ENERGY), 3));
      break;
    case 9:
      putdatamap("energy/active/export", doubletostring(sdm.readVal(SDM120CT_EXPORT_ACTIVE_ENERGY), 3));
      break;
    case 10:
      putdatamap("energy/active", doubletostring(sdm.readVal(SDM120CT_TOTAL_ACTIVE_ENERGY), 3));
      break;
    case 11:
      putdatamap("energy/reactive/import", doubletostring(sdm.readVal(SDM120CT_IMPORT_REACTIVE_ENERGY), 3));
      break;
    case 12:
      putdatamap("energy/reactive/export", doubletostring(sdm.readVal(SDM120CT_EXPORT_REACTIVE_ENERGY), 3));
      break;
    case 13:
      value = sdm.readVal(SDM120CT_TOTAL_REACTIVE_ENERGY);
      putdatamap("energy/reactive", doubletostring(value, 3));
      if (isnan(value)) putdatamap("status", "commerror");
      else putdatamap("status", "ready");
      break;
    case 14:
      if (uptime % 5 == 0) sdmreadcounter = 0;
      break;
  }
  if (sdmreadcounter < 14)
  {
    digitalWrite(NODEMCULEDPIN, isnan(value) ? 1 : 0);
    sdmreadcounter++;
  }
}
#endif

#ifdef  ESPMQTT_DDM18SD
void ddm18sd_readnextregister()
{
  static uint8_t sdmreadcounter = 1;
  double value = NAN;

  switch (sdmreadcounter)
  {
    case 1:
      putdatamap("status", "querying");
      putdatamap("voltage", String(sdm.readVal(DDM18SD_VOLTAGE), 2));
      break;
    case 2:
      putdatamap("current", String(sdm.readVal(DDM18SD_CURRENT), 2));
      break;
    case 3:
      putdatamap("power", String(sdm.readVal(DDM18SD_POWER), 2));
      break;
    case 4:
      putdatamap("power/reactive", String(sdm.readVal(DDM18SD_REACTIVE_POWER), 2));
      break;
    case 5:
      putdatamap("frequency", String(sdm.readVal(DDM18SD_FREQUENCY), 2));
      break;
    case 6:
      putdatamap("powerfactor", String(sdm.readVal(DDM18SD_POWER_FACTOR), 2));
      break;
    case 7:
      putdatamap("energy/active", String(sdm.readVal(DDM18SD_IMPORT_ACTIVE_ENERGY), 3));
      break;
    case 8:
      putdatamap("energy/reactive", String(sdm.readVal(DDM18SD_IMPORT_REACTIVE_ENERGY), 3));
      if (isnan(value)) putdatamap("status", "commerror");
      else putdatamap("status", "ready");
      break;
    case 9:
      if (uptime % 10) sdmreadcounter = 0;
      break;
  }
  if (sdmreadcounter < 9)
  {
    digitalWrite(NODEMCULEDPIN, isnan(value) ? 1 : 0);
    sdmreadcounter++;
  }
}
#endif

void espmqtt_handle_modules()
{
#ifdef QSWIFIDIMMERCHANNELS
  qswifidimmer_handle();
#endif

#ifdef QSWIFISWITCHCHANNELS
  qswifiswitch.handle();
#endif

#ifdef  ESPMQTT_DDNS
  EasyDDNS.update(10000);
#endif

#ifdef SONOFFCH
  sonoff_handle();
#endif

#ifdef  ESPMQTT_NOISE
  handle_noise();
#endif

#ifdef  ESPMQTT_DUCOBOX
  ducobox_handle();
#endif

#ifdef  ESPMQTT_DIMMER
  dimmer_handle();
#endif

#ifdef  ESPMQTT_GENERIC8266
#endif

#ifdef  ESPMQTT_BHT002
  bht002_handle();
#endif

#ifdef ESPMQTT_TUYA_2GANGDIMMERV2
  tuya_handle();
#endif

#ifdef RAINMETERPIN
  rainmeter_handle();
#endif

#if defined(ESPMQTT_WATERMETER) || defined(ESPMQTT_WATERMETER2)
  espmqtt_watermeter_handle();
#endif

#ifdef  ESPMQTT_OPENTHERM
  opentherm_handle();
#endif

#ifdef ESPMQTT_ZMAI90
  zmai90_handle();
#endif

#ifdef  ESPMQTT_SMARTMETER
  smartmeter_handle();
#endif

#ifdef  ESPMQTT_GROWATT
  growatt_handle();
#endif


#ifdef  ESPMQTT_GOODWE
  goodwe_handle();
#endif

#ifdef  ESPMQTT_GROWATT_MODBUS
  growattModbus_handle();
#endif

#ifdef FLASHBUTTON
  flashbutton_handle();
#endif

#ifdef  ESPMQTT_AMGPELLETSTOVE
  amgpelletstove_handle();
#endif

#ifdef ESPMQTT_OBD2
  obd2_handle();
#endif

#ifdef  ESPMQTT_SONOFFPOWR2
  sonoffpow2_handle();
#endif

#ifdef MH_Z19
  MHZ19_handle();
#endif
}


void espmqtt_handle_modules_100ms()
{
  //    Serial.print(".");
  timertick = 0;
#ifdef  ESPMQTT_SDM120
  sdm120_readnextregister();
#endif

#ifdef  ESPMQTT_DDM18SD
  ddm18sd_readnextregister();
#endif
}

#ifdef ESPMQTT_BBQTEMP
void update_bbqtemp()
{
  float temp = MAX6675_readCelsius(ESPMQTT_BBQTEMP_CS0);
  bbqtemp.temperature_0 = isnan(temp) ? INT16_MAX : temp * 10;
  temp = MAX6675_readCelsius(ESPMQTT_BBQTEMP_CS1);
  bbqtemp.temperature_1 = isnan(temp) ? INT16_MAX : temp * 10;
}
#endif

#ifdef  ESPMQTT_SONOFFPOWR2
void update_sonoffpowr2()
{
  static uint64_t deciwattsec = 0;
  deciwattsec += powerval >= 0 ? (powerval * 10) : 0;
  if ((uptime % 5) == 0) // Every 5 seconds send update about power usage
  {
    putdatamap("voltage", voltval >= 0 ? String(voltval, 1) : "-");
    putdatamap("current", currentval >= 0 ? String(currentval, 3) : "-");
    putdatamap("power", powerval >= 0 ? String(powerval, 1) : "-");
    putdatamap("power/apparent", currentval >= 0 ? String(voltval * currentval, 1) : "-");
    // Convert deciwattsec to watt per second (ws) string
    uint32_t lowws = (deciwattsec / 10) % 0xFFFFFFFF;
    uint32_t highws = ((deciwattsec / 10) >> 32) % 0xFFFFFFFF;
    String ws = (highws > 0 ? String(highws) : "") + String(lowws);
    putdatamap("energy/ws", ws);
    // Convert watt per second to kwh
    String wh = (highws > 0 ? String(highws / 3600) : "") + String(lowws / 3600);
    wh = String(wh.length() < 4 ? "0" : "") + String(wh.length() < 3 ? "0" : "") + String(wh.length() < 2 ? "0" : "") + wh; // Add leading zeros to wh before converting to kwh
    String kwh = wh.substring(0, wh.length() - 3) + "." + wh.substring(wh.length() - 3); // Add decimal for wh to kwh conversion;
    putdatamap("energy/kwh", kwh);
  }
}
#endif

void espmqtt_handle_modules_1sec()
{
#ifdef  ESPMQTT_BBQTEMP
  update_bbqtemp();
#endif

#ifdef  ESPMQTT_SONOFFPOWR2
  update_sonoffpowr2();
#endif

#ifdef DHTPIN
  if (uptime % 5 == 0) update_dht();
#endif

#ifdef ONEWIREPIN
  if (uptime % 10 == 0) update_onewire();
#endif

  write_oled_display();

  previouswifistatus = WiFi.status();
}

void dotasks()
{
  ESP.wdtFeed(); // Prevent watchdog to kick in...
  yield();
  ArduinoOTA.handle();
  yield();
  MDNS.update();
  yield();
  Debug.handle();
  yield();
  webserver.handleClient();
  yield();
}

void SleepDelay(uint32_t mseconds) {
  if (mseconds) {
    uint32_t wait = millis() + mseconds;
    while (wait > millis() && !Serial.available()) {  // We need to service serial buffer ASAP as otherwise we get uart buffer overrun
      delay(1);
    }
  } else {
    delay(0);
  }
}

void loop()
{
  dotasks();

#ifdef CPUSLEEP
  static uint32_t my_sleep = 0;
  if (my_sleep > 0)
  {
    uint32_t my_activity = millis() - my_sleep;
    SleepDelay(CPUSLEEP - my_activity);
    dotasks();
  }
  my_sleep = millis();
#endif

  espmqtt_handle_modules();

  switch (DataMap.status)
  {
    case DataMapStatus::online:
    case DataMapStatus::ready:
      mainstate.statusok = true;
      break;

    case DataMapStatus::receiving:
    case DataMapStatus::querying:
    case DataMapStatus::upgrading:
    case DataMapStatus::rebooting:
      // do not change statusok
      break;

    default:
      mainstate.statusok = false;
      break;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    wifichannel = WiFi.channel(); // We can't rely on wifi.channel because while scanning the channel is changed.
    if (!mainstate.wificonnected) triggers.wificonnected = true;
    mainstate.wificonnected = true;
    mainstate.wificonnectedonce = true;
  }
  else
  {
    wifichannel = 0;
    if (mainstate.wificonnected) triggers.wifidisconnected = true;
    mainstate.wificonnected = false;
  }
  yield();
  ESP.wdtFeed(); // Prevent watchdog to kick in...

  if ((0 != reboottimeout) && (uptime > reboottimeout))
  {
    ESP.restart();
    delay(1000);
  }
  yield();
  ESP.wdtFeed(); // Prevent watchdog to kick in...

  if ((0 != wifichangesettingstimeout) && (uptime > wifichangesettingstimeout))
  {
    mainstate.accesspoint = false;
    if ((wifissid[0]))
    {
      connectToWifi();
    }
    wifichangesettingstimeout = 0;
  }
  yield();
  ESP.wdtFeed(); // Prevent watchdog to kick in...

  if (triggers.wificonnected)
  {
    triggers.wificonnected = false;
    DEBUG_I("Connected to WiFi SSID=%s RSSI=%d LOCALIP=%s\n", WiFi.SSID().c_str(), WiFi.RSSI(), WiFi.localIP().toString().c_str());
    //syslogN("Connected to WiFi SSID=%s RSSI=%d\n", WiFi.SSID().c_str(), WiFi.RSSI());
    initMqtt();
    mqttReconnectTimer.once(1, connectToMqtt); // Wait 1 second before connecting mqtt
    MDNS.begin(esp_hostname);
    MDNS.addService("http", "tcp", 80);
#ifdef ESPMQTT_BHT002
    bht002_connected();
#endif

#ifdef ESPMQTT_TUYA_2GANGDIMMERV2
    tuya_connected();
#endif
  }
  yield();
  ESP.wdtFeed(); // Prevent watchdog to kick in...

  if (triggers.wifidisconnected)
  {
    triggers.wifidisconnected = false;
    DEBUG_W("Disconnected from Wi-Fi.\n");
    disconnectMqtt();
    if (!mainstate.accesspoint) wifiReconnectTimer.once(2, connectToWifi); // trying to connect to wifi can cause AP to fail
#ifdef ESPMQTT_BHT002
    bht002_disconnected();
#endif

#ifdef ESPMQTT_TUYA_2GANGDIMMERV2
    //tuya_disconnected(); // Don't do this, the device starts beeping....
#endif
  }
  yield();
  ESP.wdtFeed(); // Prevent watchdog to kick in...

  if (mainstate.mqttlastpublishedpacketid)
  {
    uint16_t mqttlastpublishedpacketid = mainstate.mqttlastpublishedpacketid;
    mainstate.mqttlastpublishedpacketid = 0;
    DEBUG_V("Acknowledged published packetid=%d\n", mqttlastpublishedpacketid);
    publishdatamap(mqttlastpublishedpacketid);
  }
  else publishdatamap();
  yield();
  ESP.wdtFeed(); // Prevent watchdog to kick in...

  if (triggers.mqttconnected)
  {
    triggers.mqttconnected = false;
    DEBUG_I("Connected to MQTT Server=%s\n", mqtt_server);
    //syslogN("Connected to MQTT Server=%s\n", mqtt_server);
    dotasks();  // Prevent crash because of to many debug data to send
    //    update_systeminfo(true);
    mqttdosubscriptions(-1);
    updateexternalip();
#ifdef ESPMQTT_TUYA_2GANGDIMMERV2
    tuya_connectedMQTT();
#endif
  }
  yield();
  ESP.wdtFeed(); // Prevent watchdog to kick in...


  if (mainstate.mqttlastsubscribedpacketid)
  {
    uint16_t mqttlastsubscribedpacketid = mainstate.mqttlastsubscribedpacketid;
    mainstate.mqttlastsubscribedpacketid = 0;
    DEBUG_V("Acknowledged subscribed packetid=%d\n", mqttlastsubscribedpacketid);
    mqttdosubscriptions(mqttlastsubscribedpacketid);
  }
  yield();
  ESP.wdtFeed(); // Prevent watchdog to kick in...


  if (triggers.mqttsubscribed)
  {
    triggers.mqttsubscribed = false;
    publishdatamap(-1, true, false);
  }
  yield();
  ESP.wdtFeed(); // Prevent watchdog to kick in...


  if (triggers.mqttdisconnected)
  {
    triggers.mqttdisconnected = false;
    DEBUG_W("Disconnected from MQTT Server=%s\n", mqtt_server);
    dotasks(); // Prevent crash because of to many debug data to send
    if (WiFi.isConnected()) {
      mqttReconnectTimer.once(5, connectToMqtt);
    }
  }
  yield();
  ESP.wdtFeed(); // Prevent watchdog to kick in...


  if (triggers.firmwareupgrade != "")
  {
    // wait 5 seconds before upgrading
    static uint64_t waitbeforeupgrade = 0;
    if (waitbeforeupgrade == 0)
    {
      DEBUG_I ("Received startfirmwareupgrade, upgrade pending...\n");
      DataMap.status = DataMapStatus::upgrading;
      DataMap.status_upgrade = DataMapStatusUpgrade::upgrading;
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
          DataMap.status_upgrade = DataMapStatusUpgrade::uptodate;
        }
        else if (getdatamap("firmware/upgradekey") != upgradekey)
        {
          DEBUG_I ("Upgrade canceled, upgradekey is incorrect\n");
          DataMap.status_upgrade = DataMapStatusUpgrade::incorrectupgradekey;
        }
        else
        {
          DEBUG_I ("Starting upgrade from:%s\n", upgradeurl.c_str());
          DataMap.status_upgrade = DataMapStatusUpgrade::upgrading;
          DataMap.status = DataMapStatus::upgrading;
          WiFiClient upgradeclient;
          t_httpUpdate_return ret = ESPhttpUpdate.update(upgradeclient, upgradeurl, ESPMQTT_VERSION);
          switch (ret)
          {
            case HTTP_UPDATE_FAILED:
              DEBUG_E("Firmware upgrade failed: %s.\n", ESPhttpUpdate.getLastErrorString().c_str());
              DataMap.status_upgrade = DataMapStatusUpgrade::httperror;
              break;
            case HTTP_UPDATE_NO_UPDATES:
              DEBUG_E("Firmware upgrade check finished, no new version available.");
              DataMap.status_upgrade = DataMapStatusUpgrade::uptodate;
              break;
            case HTTP_UPDATE_OK:
              DEBUG_E("Firmware upgrade done!\n"); // may not be called since we reboot the ESP
              DataMap.status_upgrade = DataMapStatusUpgrade::upgradedone;
              DataMap.status = DataMapStatus::rebooting;
              break;
          }
        }
      }
    }
  }
  yield();
  ESP.wdtFeed(); // Prevent watchdog to kick in...

  if (triggers.wifiscanready)
  {
    triggers.wifiscanready = false;
    int strongestwifiid = -1;
    int strongestwifirssi = -1000;
    int currentwifirssi = -1000;
    int currentwifiid = -1;

    for (int i = 0; i < wifinetworksfound; i++)
    {
      if (WiFi.SSID(i) == String(wifissid))
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
      Debug.handle();
      yield(); // Prevent crash because of to many debug data to send
      ESP.wdtFeed(); // Prevent watchdog to kick in...
    }

    DEBUG_D("CurrentAp ID=%d SSID=%s BSSID=%s RSSI=%d(%d), Strongest AP ID=%d SSID=%s, BSSID=%s RSSI=%d(%d)\n", currentwifiid, wifissid, WiFi.BSSIDstr().c_str(), currentwifirssi, WiFi.RSSI(), strongestwifiid, WiFi.SSID(strongestwifiid).c_str(), WiFi.BSSIDstr(strongestwifiid).c_str(), WiFi.RSSI(strongestwifiid), strongestwifirssi);
    Debug.handle();
    yield();  // Prevent crash because of to many debug data to send
    ESP.wdtFeed(); // Prevent watchdog to kick in...

    if (!mainstate.accesspoint)
    {
      if ((strongestwifiid >= 0) && ((WiFi.RSSI() >= 0) || (currentwifiid == -1) || ((currentwifiid != strongestwifiid) && (currentwifirssi + 10 < strongestwifirssi))))
      {
        DEBUG_I ("Switching to stronger AP %d (%s, %s, %s)\n", strongestwifiid, wifissid, wifipsk, WiFi.BSSIDstr(strongestwifiid).c_str());
        WiFi.begin(wifissid, wifipsk, WiFi.channel(strongestwifiid), WiFi.BSSID(strongestwifiid), 1);
        yield();
      }
    }
  }
  yield();
  ESP.wdtFeed(); // Prevent watchdog to kick in...

  dotasks();

  if (timertick == 1) // Every 0.1 second read next SDM120 register
  {
    espmqtt_handle_modules_100ms();
  }
  yield();
  ESP.wdtFeed(); // Prevent watchdog to kick in...

  if (timersectick == 1) // Every 1 second check sensors and update display (it would be insane to do it more often right?)
  {
    timersectick = 0;
    updatemqtt = 1;

    // scan for stronger wifi network: every 10 minutes, directly when wifi is not connected or every 30 seconds if signal is bad
    // try to do this as less as posisble because during scan the esp is unreachable for about a second.
    if ((!mainstate.accesspoint))
    {
      if ((uptime > 0) && (((uptime % 600) == 0) || (((uptime % 30) == 0) && ((WiFi.status() != WL_CONNECTED)))))
      {
        DEBUG_D("Starting Wifi Scan...\n");
        WiFi.scanNetworksAsync(wifiScanReady);
      }
    }
    yield();
    ESP.wdtFeed(); // Prevent watchdog to kick in...

    if ((uptime % 600) == 0)
    {
      updateexternalip();
    }
    yield();
    ESP.wdtFeed(); // Prevent watchdog to kick in...

    // Every 1 minutes publish all regular mqtt data
    if ((uptime % 60) == 0)
    {
      DEBUG_I ("Regular publishing datamap...\n");
      publishdatamap(-1, false, false, true);
    }

    // Every 10 minutes publish all regular mqtt data
    if ((uptime % 600) == 0)
    {
      DEBUG_I ("Publishing full datamap...\n");
      publishdatamap(-1, false, true, false);
    }

    yield();
    ESP.wdtFeed(); // Prevent watchdog to kick in...

    if ((uptime % 60) == 0)
    {
      char uptimestr[20];
      snprintf(uptimestr, 20, cF("%d:%02d:%02d:%02d"), uptime / 86400, (uptime / 3600) % 24, (uptime / 60) % 60, uptime % 60);
      time_t now;
      time(&now);
      struct tm * timeinfo;
      timeinfo = localtime (&now);
      char strtime[20];
      strftime(strtime, 20, cF("%d-%m-%Y %T"), timeinfo);
      DEBUG_I("Uptime=%s DateTime=%s\n", uptimestr, strtime);
    }
    yield();
    ESP.wdtFeed(); // Prevent watchdog to kick in...


#ifdef APONBOOT
    if ((uptime == 60) && (!mainstate.wificonnectedonce))
    {
      DEBUG_W("Connection to wifi failed, starting accesspoint\n");
      startWifiAP();
    }
    if (uptime == 660)
    {
      stopWifiAP();
    }
#endif

    // Every 10 seconds update system info
    //    if ((uptime % 10) == 0) update_systeminfo();

    espmqtt_handle_modules_1sec();
  }
}

#ifdef SONOFFCH
void sonoff_init()
{
  bool inverse = false;
#ifdef SONOFFCHINVERSE
  inverse = true;
#endif
  for (uint8_t i = 0; i < SONOFFCH; i++)
  {
    sonoff_relaystate[i] = false;

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
    bool relaystate = digitalRead(sonoff_relays[i]) != inverse ? 1 : 0;
    if ((i == 0) && (i <= SONOFFCH)) sonoff_relaystate[0] = relaystate;
    if ((i == 1) && (i <= SONOFFCH)) sonoff_relaystate[1] = relaystate;
    if ((i == 2) && (i <= SONOFFCH)) sonoff_relaystate[2] = relaystate;
    if ((i == 3) && (i <= SONOFFCH)) sonoff_relaystate[3] = relaystate;
    // TODO: Set send bit
  }

#ifdef HLW8012_CF_PIN
  static unsigned long nextupdatetime = 0;
  static unsigned long nextfiltertime = 0;
  static unsigned int filtered_power = hlw8012.getActivePower() * 10;
  static unsigned int filtered_apparent_power = hlw8012.getApparentPower() * 10;
  static unsigned int filtered_reactive_power = hlw8012.getReactivePower() * 10;
  static unsigned int filtered_current = hlw8012.getCurrent() * 1000;
  static unsigned int filtered_voltage = hlw8012.getVoltage() * 10;
  if (millis() > nextfiltertime)
  {
    nextfiltertime = millis() + 100;
    filtered_power = ((filtered_power * 4) + (hlw8012.getActivePower() * 10)) / 5;
    filtered_apparent_power = ((filtered_apparent_power * 4) + (hlw8012.getApparentPower() * 10)) / 5;
    filtered_reactive_power = ((filtered_reactive_power * 4) + (hlw8012.getReactivePower() * 10)) / 5;
    filtered_current = ((filtered_current * 4) + (hlw8012.getCurrent() * 1000)) / 5;
    filtered_voltage = ((filtered_voltage * 4) + (hlw8012.getVoltage() * 10)) / 5;
  }
  if (millis() > nextupdatetime)
  {
    putdatamap("voltage", String(((double)filtered_voltage / 10), 0));
    putdatamap("voltage/multiplier", String(hlw8012.getVoltageMultiplier()));
    putdatamap("current", String(((double)filtered_current / 1000), 3));
    putdatamap("current/multiplier", String(hlw8012.getCurrentMultiplier()));
    putdatamap("power", String(((double)filtered_power / 10), 1));
    putdatamap("power/apparent", String(((double)filtered_apparent_power / 10), 1));
    putdatamap("power/reactive", String(((double)filtered_reactive_power / 10), 1));
    putdatamap("power/factor", String(hlw8012.getPowerFactor(), 2));
    putdatamap("power/multiplier", String(hlw8012.getPowerMultiplier()));
    putdatamap("energy/ws", String(hlw8012.getEnergy()));
    putdatamap("energy/kwh", String((double)hlw8012.getEnergy() / 3600000, 3));
    nextupdatetime = millis() + 5000;
  }
#endif
}
#endif


// Publish datamap publishes the datamap one by one to mqtt broker to prevent buffer overflow
void publishdatamap(int32_t packetId, bool publishall, bool init, bool publishregular)
{
  static uint16_t datamappointer = 0;
  static int32_t nextpacketId = -1;
  static bool waitingforack = false;
  static int16_t onAirId = -1;

  if (init)
  {
    waitingforack = false;
    datamappointer = 0;
    nextpacketId = -1;
  }

  if ((packetId != -1) || publishall) DEBUG_V("Publishdatamap packetId=%d publishall=%d datamappointer=%d datamapsize=%d nextpacketid=%d waitingforack=%d\n", packetId, publishall, datamappointer, getDataMapLength(), nextpacketId, waitingforack);

  yield();

  if (publishall || publishregular)
  {
    uint8_t id = 0;
    while (id < getDataMapLength())
    {
      if (publishall) setDataMapSendStatus(id, true);
      if ((publishregular) && (getDataMapPublishRegular(id))) setDataMapSendStatus(id, true);
      DEBUG_V("id=%d datamaplength=%d publishall=%d publishregular=%d publishregularid=%d\n", id, getDataMapLength(), publishall, publishregular, getDataMapPublishRegular(id));
      id++;
      yield();
    }
    datamappointer = 0;
  }

  yield();

  // If connected to mqtt and waiting for ack wait for packetid which has to be acked
  if (waitingforack)
  {
    if (packetId == 0)
    {
      // If packetId == 0 resend because packet was not acked
      DEBUG_V("Not received mqtt ack id=%d\n", packetId);
      onAirId = -1;
      waitingforack = false;
    }
    else if (packetId == nextpacketId)
    {
      // Packet succesfull delivered proceed to next item
      DEBUG_V("Received mqtt ack id=%d\n", packetId);
      setDataMapSendStatus(onAirId, false);
      onAirId = -1;
      datamappointer++;
      waitingforack = false;
    }
  }

  // If not waiting for ack search for next item in datamap which has to be send
  else if (mqttClient.connected() && (WiFi.status() == WL_CONNECTED) && mainstate.mqttsubscribed)
  {
    if (datamappointer < getDataMapLength())
    {
      nextpacketId = -1;
      while ((datamappointer < getDataMapLength()) && (nextpacketId == -1))
      {
        String sendtopic = String(mqtt_topicprefix + getDataMapKey(datamappointer)); // we have to do this before getdatamapsendstatus
        if (getDataMapSendStatus(datamappointer))
        {
          nextpacketId = mqttClient.publish(sendtopic.c_str(), 1, true, getDataMapValue(datamappointer).c_str());
          if (nextpacketId > 0)
          {
            waitingforack = true;
            onAirId = datamappointer;
          }
          DEBUG_D ("MQTT PUBLISHING DATAMAP %s=%s (nextpacketId=%d)\n", sendtopic.c_str(), getDataMapValue(datamappointer).c_str(), nextpacketId);
          yield();
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
  yield();
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
    ledontime = 1;
    ledofftime = 50;
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

#ifdef NEOPIXELPIN
  update_neoled_status();
#endif
}

#ifdef MH_Z19
uint8_t MHZ19_handle()
{
  static unsigned long requesttime = 5000;
  static unsigned long readtime = 0;
  uint16_t newppm = UINT8_MAX;
  int8_t newtemp = INT_MAX;
  if (requesttime < millis())
  {
    MHZ19_send_request_cmd();
    requesttime = millis() + 5000;
    readtime = millis() + 100;
  }
  if ((readtime > 0) && (readtime < millis()))
  {
    readtime = 0;
    if (MHZ19_read(&newppm, &newtemp) == false)
    {
      DEBUG_I("MHZ19 ppm=%d temp=%d\n", newppm, newtemp);
      if (newppm == 0) return 1;
      else
      {
        mhz19.changed.temperature = mhz19.temperature != newtemp;
        mhz19.changed.co2 = mhz19.co2 != newppm;
        mhz19.temperature = newtemp;
        mhz19.co2 = newppm;
        return 0;
      }
    }
    else  
    {
      DEBUG_E("MHZ19 read error!\n");
      newppm = (uint16_t)UINT8_MAX;
      newtemp = (int8_t)INT8_MAX;
      mhz19.changed.temperature = mhz19.temperature != newtemp;
      mhz19.changed.co2 = mhz19.co2 != newppm;
      mhz19.temperature = newtemp;
      mhz19.co2 = newppm;
      return 2;
    }
  }
  return 1;
}


void MHZ19_init()
{
  MHZ19_setabs(true);
  DataMap.length += 2;
}

uint8_t MHZ19_checksum(uint8_t cmd[], uint8_t length)
{
  uint8_t sum = 0x00;
  for (int i = 1; i < length; i++)
  {
    sum += cmd[i];
  }
  sum = 0xff - sum + 0x01;
  return sum;
}

void MHZ19_write(uint8_t cmd[], uint8_t length)
{
  uint8_t checksum = MHZ19_checksum(cmd, length);
  Serial.write (cmd, length);
  Serial.write (checksum);
  Serial.flush();
}

void MHZ19_setabs(bool enabled)
{
  DEBUG_V("Sending MHZ19 ABS enable=%d...\n", enabled);
  if (enabled)
  {
    //Set abs enable
    uint8_t cmd[8] = {0xff, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00};
    MHZ19_write(cmd, 8);
  }
  else
  {
    uint8_t cmd[8] = {0xff, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00};
    MHZ19_write(cmd, 8);
  }
}

void MHZ19_send_request_cmd()
{
  uint8_t cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00};
  // command to ask for data

  DEBUG_V("Sending MHZ19 request packet...\n");

  MHZ19_write(cmd, 8);
}

bool MHZ19_read(uint16_t *ppm, int8_t *temp)
{
  static uint8_t errorcounter = 255;
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
      uint16_t responseHigh = (int) response[2];
      uint16_t responseLow = (int) response[3];
      *ppm = (256 * responseHigh) + responseLow;
      *temp = response[4] - 40;
      errorcounter = 0;
    }
  }
  else
  {
    DEBUG_E("Invalid response from MHZ_19 CO2 sensor!\n");
    if (errorcounter < 5) errorcounter++;
    else return true;
  }
  return false;
}


#endif



void handleWWWSettings()
{
  if (!webserver.authenticate("admin", esp_password)) {
    return webserver.requestAuthentication();
  }


  if (webserver.method() == HTTP_POST)
  {
    for (uint8_t i = 0; i < webserver.args(); i++)
    {
      if (webserver.argName(i) == "rebootdevice")
      {
        webserver.send(200, "text/html", F("<HTML><HEAD><meta http-equiv=\"refresh\" content=\"5;url=\\\" /></HEAD><BODY>Device Rebooting, Please Wait...</BODY></HTML>"));
        reboottimeout = uptime + 4;
        return;
      }
    }
    mqtt_ssl = 0;
    for (uint8_t i = 0; i < webserver.args(); i++)
    {
      if (webserver.argName(i) == "wifissid") snprintf(wifissid, WIFISSID_SIZE, "%s", webserver.arg(i).c_str());
      if (webserver.argName(i) == "wifipsk") snprintf (wifipsk, WIFIPSK_SIZE, "%s", webserver.arg(i).c_str());
      if (webserver.argName(i) == "mqttserver") snprintf (mqtt_server, MQTT_SERVER_SIZE, "%s", webserver.arg(i).c_str());
      if (webserver.argName(i) == "mqttusername") snprintf(mqtt_username, MQTT_USERNAME_SIZE, "%s", webserver.arg(i).c_str());
      if (webserver.argName(i) == "mqttpassword") snprintf(mqtt_password, MQTT_PASSWORD_SIZE, "%s", webserver.arg(i).c_str());
      if (webserver.argName(i) == "mqtttopicprefix") snprintf(mqtt_topicprefix, MQTT_TOPICPREFIX_SIZE, "%s", webserver.arg(i).c_str());
      if (webserver.argName(i) == "mqttport") sscanf(webserver.arg(i).c_str(), "%d", &mqtt_port);;
      if (webserver.argName(i) == "mqttssl") mqtt_ssl = 1;
      if (webserver.argName(i) == "webpassword")
      {
        if (strcmp (esp_password, webserver.arg(i).c_str()) != 0)
        {
          snprintf (esp_password, ESP_PASSWORD_SIZE, webserver.arg(i).c_str());
          ArduinoOTA.setPassword(esp_password);
          ArduinoOTA.begin();
          Debug.setPassword(esp_password);
        }
      }
      if (webserver.argName(i) == "hostname") snprintf(esp_hostname, ESP_HOSTNAME_SIZE, "%s", webserver.arg(i).c_str());
      replacechar(esp_hostname, '_', '-'); // RFC doesn't alllow underscores.
#ifdef  ESPMQTT_WATERMETER
      if (webserver.argName(i) == "watermeterliter")
      {
        if (webserver.arg(i) != getdatamap("water/liter"))
        {
          watermeter_setliters(atoi(webserver.arg(i).c_str()));
          i2cEeprom_write(watermeter_getliters());
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
    eeprom_save_variables();

    mainstate.defaultpassword = false;

    disconnectMqtt(); // Disconnect mqtt server

    if (strcmp(esp_hostname, WiFi.hostname().c_str()) != 0)
    {
      WiFi.hostname(esp_hostname);
      wifichangesettingstimeout = uptime + 4;
    }

    if ((wifissid[0] != 0) && (wifipsk[0] != 0) && (esp_hostname[0] != 0) && ((strcmp(wifissid, WiFi.SSID().c_str()) != 0) || (strcmp(wifipsk, WiFi.psk().c_str()) != 0)))
    {
      webserver.send(200, "text/html", F("<HTML><BODY>Settings Saved.<BR>Please wait a moment and connect to proper wifi network and open the page of the saved hostname.</BODY></HTML>"));
      flashbuttonstatus = 0;
      previouswifistatus = -1;
      wifichangesettingstimeout = uptime + 4;
      return;
    }
    else
    {
      wifichangesettingstimeout = uptime + 4;
      webserver.send(200, "text/html", F("<HTML><HEAD><meta http-equiv=\"refresh\" content=\"5;url=\\\" /></HEAD><BODY>Settings Saved, Please Wait...</BODY></HTML>"));
    }

    initMqtt();
    connectToMqtt();
    //    update_systeminfo(true);
  }
  else
  {
    String wifiselectoptions = "<option value=\"" + WiFi.SSID() + "\" selected>" + WiFi.SSID() + "</option>";
    uint8_t n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i) {
      if (WiFi.SSID(i) != WiFi.SSID()) wifiselectoptions += "<option value=\"" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</option>";
    }


    webserver.setContentLength(CONTENT_LENGTH_UNKNOWN);
    webserver.send (200, "text/html", F("<HTML><HEAD><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></HEAD><BODY><CENTER><div align=\"left\" style=\"width:400px; margin:auto\">"));
    webserver.sendContent (F("<CENTER><H1>"));
    webserver.sendContent (WiFi.hostname() + "</H1>");
    webserver.sendContent (F("</CENTER><form action=\"/settings\" method=\"post\" autocomplete=\"off\"><TABLE style=\"width:400px; margin:auto\">"));
    webserver.sendContent (F("<TR><TD>Hostname</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"40\" name=\"hostname\" value=\""));
    webserver.sendContent (WiFi.hostname() + "\">");
    webserver.sendContent (F("</TD></TR>"));
    webserver.sendContent (F("<TR><TD>Wifi SSID</TD><TD><select style=\"width:200\" name=\"wifissid\">"));
    webserver.sendContent (wifiselectoptions + "</select>");
    webserver.sendContent (F("</TD></TR>"));
    webserver.sendContent (F("<TR><TD>wifi Key</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"64\" name=\"wifipsk\" value=\""));
    webserver.sendContent (String(WiFi.psk()) + "\">");
    webserver.sendContent (F("</TD></TR>"));
    webserver.sendContent (F("<TR><TD>MQTT Server</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"20\" name=\"mqttserver\" value=\""));
    webserver.sendContent (String(mqtt_server) + "\">");
    webserver.sendContent (F("</TD></TR>"));
    webserver.sendContent (F("<TR><TD>MQTT Port</TD><TD><input style=\"width:200\" type=\"number\" maxlength=\"5\" name=\"mqttport\" value=\""));
    webserver.sendContent (String(mqtt_port) + "\"></TD></TR>");
    webserver.sendContent (F("<TR><TD>MQTT Ssl</TD><TD ALIGN=\"left\"><input type=\"checkbox\" name=\"mqttssl\" "));
    webserver.sendContent (String(mqtt_ssl ? "checked" : "") + ("></TD></TR>"));
    webserver.sendContent (F("<TR><TD>MQTT Username</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"20\" name=\"mqttusername\" value=\""));
    webserver.sendContent (String(mqtt_username) + "\">");
    webserver.sendContent (F("</TD></TR>"));
    webserver.sendContent (F("<TR><TD>MQTT Password</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"20\" name=\"mqttpassword\" value=\""));
    webserver.sendContent (String(mqtt_password) + "\">");
    webserver.sendContent (F("</TD></TR>"));
    webserver.sendContent (F("<TR><TD>MQTT Topic Prefix</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"50\" name=\"mqtttopicprefix\" value=\""));
    webserver.sendContent (String(mqtt_topicprefix) + "\">");
    webserver.sendContent (F("</TD></TR>"));
    webserver.sendContent (F("<TR><TD>ESP Password</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"20\" name=\"webpassword\" value=\""));
    webserver.sendContent (String(esp_password) + "\">");
    webserver.sendContent (F("</TD></TR>"));
#ifdef  ESPMQTT_WATERMETER
    webserver.sendContent (F("<TR><TD>Watermeter Liter</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"64\" name=\"watermeterliter\" value=\""));
    webserver.sendContent (getDataMap("water/liter"));
    webserver.sendContent (F("\"></TD></TR>"));
#endif
#ifdef  ESPMQTT_QSWIFIDIMMERD01
    webserver.sendContent (F("<TR><TD>Dimmer Offset</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"3\" name=\"qswifidimoffset\" value=\""));
    webserver.sendContent (getdatamap("dimoffset"));
    webserver.sendContent (F("\"></TD></TR>"));
#endif
#ifdef  ESPMQTT_QSWIFIDIMMERD02
    webserver.sendContent (F("<TR><TD>Dimmer Offset 0</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"3\" name=\"qswifidimoffset0\" value=\""));
    webserver.sendContent (getdatamap("dimoffset/0"));
    webserver.sendContent (F("\"></TD></TR>"));
    webserver.sendContent (F("<TR><TD>Dimmer Offset 1</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"3\" name=\"qswifidimoffset1\" value=\""));
    webserver.sendContent (getdatamap("dimoffset/1"));
    webserver.sendContent (F("\"></TD></TR>"));
#endif
    webserver.sendContent (F("</TABLE><BR><CENTER><input type=\"submit\" value=\"Save Settings\"></form><BR><BR><form action=\"/settings\" method=\"post\" autocomplete=\"off\"><input type=\"hidden\" name=\"rebootdevice\" value=\"1\"><input type=\"submit\" value=\"Reboot Device\"></form><BR><BR><A HREF=\"/\">Return to main page</A></CENTER></div></BODY></HTML>"));
    webserver.sendContent (""); // end chunked data
  }
}

void handleJsonData() {
  String json = "{";
  webserver.setContentLength(CONTENT_LENGTH_UNKNOWN);
  webserver.send(200, "text/html", "{");
  for (int i = 0; i < getDataMapLength(); i++)
  {
    webserver.sendContent ("\"" + String(getDataMapKey(i)) + "\":\"" + getDataMapValue(i));
    if (i < getDataMapLength() - 1) webserver.sendContent("\",");
  }
  webserver.sendContent("\"}");
  webserver.sendContent("");
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
  if (dimchannel == 0)
  {
    putdatamap ("dimvalue/0", String(dimvalue));
    putdatamap ("dimstate/0", String(dimstate));
  }
  if (dimchannel == 1)
  {
    putdatamap ("dimvalue/1", String(dimvalue));
    putdatamap ("dimstate/1", String(dimstate));
  }
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
  if (dimchannel == 0) putdatamap ("switchstate/0", String(switchstate));
  if (dimchannel == 1) putdatamap ("switchstate/1", String(switchstate));
}
#endif

#if (QSWIFISWITCHCHANNELS == 1)
void qswifiswitch_relaycallback(uint8_t , bool state)
{
  putdatamap ("relaystate", String(state));
}
void qswifiswitch_switchcallback(uint8_t , bool switchstate)
{
  putdatamap ("switchstate", String(switchstate));
}
#endif
#if (QSWIFISWITCHCHANNELS == 2)
void qswifiswitch_relaycallback(uint8_t channel, bool state)
{
  if (channel == 0) putdatamap ("relaystate/0", String(state));
  if (channel == 1) putdatamap ("relaystate/1", String(state));
}
void qswifiswitch_switchcallback(uint8_t channel, bool switchstate)
{
  if (channel == 0) putdatamap ("switchstate/0", String(switchstate));
  if (channel == 1) putdatamap ("switchstate/1", String(switchstate));
}
#endif

#ifdef ESPMQTT_DUCOBOX
void ducoboxcallback ()
{
  if (ducobox_DataMap.status == Ducobox_status::ready)
  {
    if (DataMap.status != DataMapStatus::ready)
    {
      DataMap.status = DataMapStatus::ready;
      DataMap.sendStatusAfterPublishCompleted = true;
    }
  }

  if ((ducobox_DataMap.status == Ducobox_status::commerror))
  {
    if (DataMap.status != DataMapStatus::commerror)
    {
      DataMap.status = DataMapStatus::commerror;
      setDataMapSendStatus("status", true);
    }
  }

  for (uint8_t bitpointer = 0;  bitpointer < 6; bitpointer++)
  {
    //if (*(uint32_t*)&growattModbus_DataMap.changed & (1 << bitpointer))
    //{
      //*(uint32_t*)&growattModbus_DataMap.changed &=  ~(1 << bitpointer);
      setDataMapSendStatus(DATAMAP_BASELENGTH + bitpointer, true);
    //}
  }
}
#endif

void amgpelletstovecallback (const char *topic, String payload)
{
  //  putdatamap(topic, payload);
}

void bht002callback (const char *topic, String payload)
{
  //  putdatamap(topic, payload);
  yield();
}

void tuyacallback (const char *topic, String payload)
{
  //  putdatamap(topic, payload);
  yield();
}

void openthermcallback (const char *topic, String payload)
{
  //  putdatamap(topic, payload);
}

#ifdef  ESPMQTT_GROWATT
void growattcallback ()
{
  if (growatt_DataMap.status == Growatt_status::ready)
  {
    if (DataMap.status != DataMapStatus::ready)
    {
      DataMap.status = DataMapStatus::ready;
      DataMap.sendStatusAfterPublishCompleted = true;
    }
  }

  if ((DataMap.status != DataMapStatus::querying) && (DataMap.status != DataMapStatus::commerror))
  {
    if (DataMap.status != DataMapStatus::querying)
    {
      DataMap.status = DataMapStatus::querying;
      setDataMapSendStatus("status", true);
    }
  }

  if (growatt_DataMap.status == Growatt_status::offline)
  {
    if (DataMap.status != DataMapStatus::commerror)
    {
      DataMap.status = DataMapStatus::commerror;
      setDataMapSendStatus("status", true);
    }
  }

  for (uint8_t bitpointer = 0;  bitpointer < 16; bitpointer++)
  {
    for (unint8_t bytepointer = 0; bytepointer < 2; bytepointer++)
    {
    if (*(uint8_t*)&growatt_DataMap.changed[bytepointer] & (1 << bitpointer))
    {
      *(uint8_t*)&growatt_DataMap.changed[bytepointer] &=  ~(1 << bitpointer);
      setDataMapSendStatus(DATAMAP_BASELENGTH + (bitpointer + (bytepointer * 8)), true);
    }
    }
  }
}
#endif

#ifdef  ESPMQTT_GOODWE
void goodwecallback (const char *topic, String payload)
{
  if (topic == "status")
  {
    if (payload == "ready") digitalWrite(NODEMCULEDPIN, 0);
    else digitalWrite(NODEMCULEDPIN, 1);
  }
  //putdatamap(topic, payload);
}
#endif

#ifdef  ESPMQTT_GROWATT_MODBUS
void growattModbuscallback ()
{
  //DEBUG("changed=%u\n", *(uint16_t*)&growattModbus_DataMap.changed);
  for (uint8_t bitpointer = 0;  bitpointer < GROWATT_MODBUS_DATAMAP_LENGTH; bitpointer++)
  {
    if (*(uint32_t*)&growattModbus_DataMap.changed & (1 << bitpointer))
    {
      *(uint32_t*)&growattModbus_DataMap.changed &=  ~(1 << bitpointer);
      setDataMapSendStatus(DATAMAP_BASELENGTH + bitpointer, true);
    }
  }

  if (growattModbus_DataMap.status == GrowattModbus_status::ready)
  {
    if (DataMap.status != DataMapStatus::ready)
    {
      DataMap.status = DataMapStatus::ready;
      DataMap.sendStatusAfterPublishCompleted = true;
    }
  }

  if (growattModbus_DataMap.status == GrowattModbus_status::querying)
  {
    if ((DataMap.status != DataMapStatus::querying) && (DataMap.status != DataMapStatus::commerror))
    {
      DataMap.status = DataMapStatus::querying;
      setDataMapSendStatus("status", true);
    }
  }

  if (growattModbus_DataMap.status == GrowattModbus_status::offline)
  {
    if (DataMap.status != DataMapStatus::commerror)
    {
      DataMap.status = DataMapStatus::commerror;
      setDataMapSendStatus("status", true);
    }
  }
}
#endif

#ifdef  ESPMQTT_SMARTMETER
void smartmetercallback ()
{
  if (smartmeter_DataMap.status == Smartmeter_status::ready) DataMap.status = DataMapStatus::ready;
  if (smartmeter_DataMap.status == Smartmeter_status::receiving) DataMap.status = DataMapStatus::receiving;
  if (smartmeter_DataMap.status == Smartmeter_status::disconnected) DataMap.status = DataMapStatus::commerror;
  setDataMapSendStatus(1);

  if (smartmeter_DataMap.status != Smartmeter_status::ready) return;
  
  for (uint8_t bitpointer = 0;  bitpointer < 8; bitpointer++)
  {
    if (*(uint8_t*)&smartmeter_DataMap.electricity.changed & (1 << bitpointer))
    {
      *(uint8_t*)&smartmeter_DataMap.electricity.changed &=  ~(1 << bitpointer);
      setDataMapSendStatus(DATAMAP_BASELENGTH + bitpointer, true);
    }
  }
  for (uint8_t bitpointer = 0;  bitpointer < 3; bitpointer++)
  {
    if (*(uint8_t*)&smartmeter_DataMap.gas.changed & (1 << bitpointer))
    {
      *(uint8_t*)&smartmeter_DataMap.gas.changed &=  ~(1 << bitpointer);
      setDataMapSendStatus(DATAMAP_BASELENGTH + bitpointer + 8, true);
    }
  }
}
#endif

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
    DEBUG("  mqttconnect ((re)connects to MQTT broker)\n");
    DEBUG("  showmainstate\n");
    DEBUG("  factoryreset\n");
    DEBUG("  showeeprom\n");
    DEBUG("  scanwifinetworks (scan for stronger wifi network and connect to it)\n");
    DEBUG("  showtime\n");
    DEBUG("  getrestartreason\n");
    DEBUG("  wifidisconnect\n");
  }

  if (lastCmd == "wifidisconnect")
  {
    WiFi.disconnect();
    connectToWifi();
  }

  if (lastCmd == "getrestartreason")
  {
    switch (ESP.getResetInfoPtr()->reason)
    {
      case REASON_DEFAULT_RST:
        DEBUG ("Normal Startup\n");
        break;
      case REASON_WDT_RST:
        DEBUG ("Hardware Watchdog Reset\n");
        break;
      case REASON_EXCEPTION_RST:
        DEBUG ("Exception Reset\n");
        break;
      case REASON_SOFT_WDT_RST:
        DEBUG ("Software Watchdog Reset\n");
        break;
      case REASON_SOFT_RESTART:
        DEBUG ("Software Restart\n");
        break;
      case REASON_DEEP_SLEEP_AWAKE:
        DEBUG ("Wake up from deep sleep\n");
        break;
      case REASON_EXT_SYS_RST:
        DEBUG ("External Reset\n");
        break;
    }
  }

  if (lastCmd == "showtime")
  {
    time_t now;
    char strftime_buf[64];
    struct tm timeinfo;

    time(&now);

    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);

    DEBUG ("Datetime= %s\n", strftime_buf);
  }

  if (lastCmd == "scanwifinetworks")
  {
    DEBUG_D("Starting Wifi Scan...\n");
    WiFi.scanNetworksAsync(wifiScanReady);
    yield();
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

  if (lastCmd == "mqttconnect")
  {
    connectToMqtt();
  }

  if (lastCmd == "mqttdisconnect")
  {
    disconnectMqtt();
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
    DEBUG("i2cEeprom write lfiters=%d\n", watermeter_getliters());
    i2cEeprom_write(watermeter_getliters());
  }
#endif

#ifdef ESPMQTT_BHT002
  if (lastCmd == "b")
  {
    bht002_senddebug(atoi(lastArg.c_str()));
  }
#endif

  if (lastCmd == "showeeprom")
  {
    uint16_t eeprompointer = 0;
    while (eeprompointer < 512)
    {
      DEBUG("%d: %c (%d)\n", eeprompointer, EEPROM.read(eeprompointer), EEPROM.read(eeprompointer));
      eeprompointer++;
    }
  }

  if (lastCmd == "writeeeprom")
  {
    eeprom_save_variables();
  }

#ifdef ESPMQTT_OPENTHERM
  if (lastCmd == "resetopentherm")
  {
    DEBUG_D("Resetting opentherm...\n");
    opentherm_reset();
  }
#endif
}



int16_t eeprom_read(char *data, uint8_t eepromindex, uint8_t maxlength)
{
  DEBUG_D("eeprom_read index=%d;\n", eepromindex);
  uint16_t eeprompointer = 0;
  uint16_t eepromdatastartpointer = 0;
  uint8_t index = 0;
  data[0] = '\0';
  while (eeprompointer < 512)
  {
    eepromdatastartpointer = eeprompointer;
    uint8_t datasize = EEPROM.read(eeprompointer++);
    //DEBUG_D("datasize=%d\n", datasize);
    if (datasize == 0) return -1;
    uint8_t checksum = 20;
    if (index == eepromindex)
    {
      for (uint8_t pos = 0; pos < datasize - 1; pos++)
      {
        if (pos < maxlength - 2)
        {
          data[pos] = char(EEPROM.read(eeprompointer));
          data[pos + 1] = '\0';
          checksum += char(EEPROM.read(eeprompointer++));
        }
      }
      uint8_t eepromchecksum = EEPROM.read(eeprompointer++);
      if (eepromchecksum != checksum)
      {
        DEBUG_E("Read from eeprom index=%d datapointerstart=%d data=\"%s\" (checksum %d=%d)\n", eepromindex, eepromdatastartpointer, data, checksum, eepromchecksum);
        data[0] = '\0';
        return -1;
      }
      DEBUG_D("Read from eeprom index=%d datapointerstart=%d data=\"%s\" (checksum %d=%d)\n", eepromindex, eepromdatastartpointer, data, checksum, eepromchecksum);
      return eepromdatastartpointer;
    }
    eeprompointer += datasize;
    index++;
  }
  DEBUG_E("Read from eeprom failed, index not found. Index=%d\n", eepromindex);
  return -1;
}

void eeprom_erase()
{
  for (int16_t eeprompointer = 0; eeprompointer < 512; eeprompointer++)
  {
    EEPROM.write (eeprompointer, 0);
  }
}

uint16_t eeprom_write(const char *value, uint16_t eeprompos)
{
  uint8_t checksum = 20;
  DEBUG_D("eeprom_write %d,%s\n", eeprompos, value);
  EEPROM.write(eeprompos++, strlen(value) + 1);
  for (uint16_t valueindex = 0; valueindex < strlen(value); valueindex++)
  {
    EEPROM.write(eeprompos++, value[valueindex]);
    checksum += value[valueindex];
  }
  EEPROM.write(eeprompos++, checksum);
  EEPROM.write(eeprompos + 1, 0);
  DEBUG_D("checksum=%d, eepromposend=%d\n", checksum, eeprompos);
  return eeprompos;
}

void eeprom_save_variables()
{
  uint16_t pos = 0;
  pos = eeprom_write(mqtt_server, pos);
  pos = eeprom_write(mqtt_username, pos);
  pos = eeprom_write(mqtt_password, pos);
  pos = eeprom_write(esp_password, pos);
  pos = eeprom_write(esp_hostname, pos);
  char buffer [10];
  snprintf (buffer, 10, "%d", mqtt_port);
  pos = eeprom_write(buffer, pos);
  pos = eeprom_write(mqtt_ssl ? "1" : "0", pos);
  pos = eeprom_write(mqtt_topicprefix, pos);
#ifdef  ESPMQTT_QSWIFIDIMMERD01
  pos = eeprom_write(String(qswifidimmer_getdimoffset()), pos);
  putdatamap("dimoffset", String(qswifidimmer_getdimoffset()));
  pos = eeprom_write("", pos);
#elif defined ESPMQTT_QSWIFIDIMMERD02
  pos = eeprom_write(String(qswifidimmer_getdimoffset(0)), pos);
  putdatamap("dimoffset/0", String(qswifidimmer_getdimoffset(0)));
  pos = eeprom_write(String(qswifidimmer_getdimoffset(1)), pos);
  putdatamap("dimoffset/1", String(qswifidimmer_getdimoffset(1)));
#else
  pos = eeprom_write("", pos);
  pos = eeprom_write("", pos);
#endif
  pos = eeprom_write(wifissid, pos);
  pos = eeprom_write(wifipsk, pos);
  EEPROM.commit();
}

void eeprom_load_variables()
{
  // Read settings from EEPROM
  DEBUG_D("Reading internal EEPROM...\n");
  if (0 > eeprom_read(mqtt_server, 0, MQTT_SERVER_SIZE))
  {
    DEBUG_E("Error reading mqtt server from internal eeprom\n");
  }
  DEBUG_D("mqtt server=%s\n", mqtt_server);

  if (0 > eeprom_read(mqtt_username, 1, MQTT_USERNAME_SIZE))
  {
    DEBUG_E("Error reading mqtt username from internal eeprom\n");
  }
  DEBUG_D("mqtt username=%s\n", mqtt_username);

  if (0 > eeprom_read(mqtt_password, 2, MQTT_PASSWORD_SIZE))
  {
    DEBUG_E("Error reading mqtt password from internal eeprom\n");
  }
  else
  {
    mainstate.defaultpassword = false;
  }

  DEBUG_D("mqtt password=%s\n", mqtt_password);

  if (0 > eeprom_read(esp_password, 3, ESP_PASSWORD_SIZE))
  {
    DEBUG_E("Error reading esp password from internal eeprom\n");
  }
  if (strlen(esp_password) == 0) strcpy(esp_password, "esplogin");
  DEBUG_D("esp password=%s\n", esp_password);

  if (0 > eeprom_read(esp_hostname, 4, ESP_HOSTNAME_SIZE))
  {
    DEBUG_E("Error reading hostname from internal eeprom\n");
    snprintf (esp_hostname, ESP_HOSTNAME_SIZE, "%s-%s", FIRMWARE_TARGET, chipid.c_str());
  }

  replacechar (esp_hostname, '_', '-'); // RFC doesn't alllow underscores.
  DEBUG_I("Hostname=%s\n", esp_hostname);

  WiFi.hostname(esp_hostname);

  char buffer[8];
  if (0 > eeprom_read(buffer, 5, 8))
  {
    DEBUG_E("Error reading mqttport from internal eeprom\n");
  }
  else
  {
    sscanf(buffer, "%d", &mqtt_port);
  }
  DEBUG_D("mqtt port=%d\n", mqtt_port);

  if (0 > eeprom_read(buffer, 6, 8))
  {
    DEBUG_E("Error reading mqtt ssl from internal eeprom\n");
  }
  else
  {
    mqtt_ssl = 0;
    if (strcmp(buffer, "1") == 0) mqtt_ssl = 1;
  }
  DEBUG_D("mqtt ssl=%d\n", mqtt_ssl);

  if (0 > eeprom_read(mqtt_topicprefix, 7, MQTT_TOPICPREFIX_SIZE))
  {
    DEBUG_E("Error reading mqtt main topic from internal eeprom\n");
    snprintf(mqtt_topicprefix, MQTT_TOPICPREFIX_SIZE, "home/%s/", esp_hostname);
  }
  DEBUG_D("mqtt topicprefix=%s\n", mqtt_topicprefix);

#ifdef ESPMQTT_QSWIFIDIMMERD01
  char dimoffset[4] = "0";
  if (0 > eeprom_read(dimoffset, 8, 4))
  {
    DEBUG_E("Error reading dimmer offset from internal eeprom\n");
    strcpy (dimoffset, "20");
  }
  qswifidimmer_setdimoffset(atoi(dimoffset));
  putdatamap("dimoffset", String(qswifidimmer_getdimoffset()));
#endif

#ifdef ESPMQTT_QSWIFIDIMMERD02
  char dimoffsetarray[4] = "0";
  if (0 > eeprom_read(dimoffset, 8, 4))
  {
    DEBUG_E("Error reading dimmer offset 0 from internal eeprom\n");
    strcpy (dimoffset, "20");
  }
  qswifidimmer_setdimoffset(atoi(dimoffset), 0);
  dimoffset[0] = 0;
  if (0 > eeprom_read(dimoffset, 9, 4))
  {
    DEBUG_E("Error reading dimmer offset 1 from internal eeprom\n");
    strcpy (dimoffset, "20");
  }
  qswifidimmer_setdimoffset(atoi(dimoffset), 1);
  putdatamap("dimoffset/0", String(qswifidimmer_getdimoffset(0)));
  putdatamap("dimoffset/1", String(qswifidimmer_getdimoffset(1)));
#endif

  if (0 > eeprom_read(wifissid, 10, WIFISSID_SIZE))
  {
    DEBUG_E("Error reading wifi ssid from internal eeprom\n");
    snprintf(wifissid, WIFISSID_SIZE, "%s", WiFi.SSID().c_str());
  }
  if (0 == wifissid[0]) snprintf(wifissid, WIFISSID_SIZE, "%s", WiFi.SSID().c_str());
  DEBUG_D("wifi ssid=%s\n", wifissid);

  if (0 > eeprom_read(wifipsk, 11, WIFIPSK_SIZE))
  {
    DEBUG_E("Error reading wifi key from internal eeprom\n");
    snprintf(wifipsk, WIFIPSK_SIZE, "%s", WiFi.psk().c_str());
    eeprom_save_variables(); // If reading key was in error, wifi data was not stored in eeprom before, so store it.
  }
  if (0 == wifipsk[0]) snprintf (wifipsk, WIFIPSK_SIZE, "%s", WiFi.psk().c_str());
  DEBUG_D("wifi key=%s\n", wifipsk);
}

void setup() {
#ifndef SERIALLOG
  Serial.setDebugOutput(false);
#endif

  ESP.wdtDisable(); // Use hardware watchdog of 6 seconds to prevent auto reboot when function takes more time..
  initSerial();

#ifdef SERIALLOG
  Debug.setSerialEnabled(true);
#else
  Debug.setSerialEnabled(false);
#endif

  Debug.begin("");
  DEBUG_I("\n\nInitializing ESP8266 %s %s...\n\n", FIRMWARE_TARGET, ESPMQTT_VERSION);

  EEPROM.begin(512);

  char buffer[25];
  snprintf(buffer, 25, "%08X", ESP.getChipId());
  chipid = buffer;

  configTime(MYTZ, "nl.pool.ntp.org");
  yield();

  eeprom_load_variables();

  //  update_systeminfo(true);

#ifdef  ESPMQTT_DDNS
  EasyDDNS.service("duckdns");
  EasyDDNS.client("renegusta.duckdns.org", "c4f55de5-0d15-477d-a938-f39c19c67b33");
#endif


#ifdef ESPMQTT_BHT002
  bht002_init(bht002callback);
#endif

#ifdef ESPMQTT_TUYA_2GANGDIMMERV2
  tuya_init(tuyacallback);
#endif


#ifdef SONOFFCH
  sonoff_init();
#endif



  Debug.begin(esp_hostname, DEBUGLEVEL);
  Debug.setPassword(esp_password);
  Debug.setResetCmdEnabled(true);
  Debug.setCallBackProjectCmds(&processCmdRemoteDebug);

  //syslog.server(mqtt_server.c_str(), 514);
  //syslog.deviceHostname(esp_hostname.c_str());
  //syslog.appName(FIRMWARE_TARGET);
  //syslog.defaultPriority(LOG_KERN);

  DEBUG_I("ESP8266 Started...\n");
  DEBUG_I("Hostname=%s\n", WiFi.hostname().c_str());

  ArduinoOTA.onStart([]() {
#ifdef  ESPMQTT_DIMMER
    dimmer_stop();
#endif
    Serial.end();
    WiFi.setSleepMode(WIFI_NONE_SLEEP); // When sleep is on regular disconnects occur https://github.com/esp8266/Arduino/issues/5083
  });

  ArduinoOTA.onEnd([]() {
    WiFi.setSleepMode(WIFI_MODEM_SLEEP);
    initSerial();
#ifdef  ESPMQTT_DIMMER
    dimmer_init(ZEROCROSS_PIN, TRIAC_PIN);
#endif
  });

  ArduinoOTA.onProgress([](unsigned int, unsigned int) {
    ESP.wdtFeed();
  });

  ArduinoOTA.setHostname(esp_hostname);
  ArduinoOTA.setPassword(esp_password);
  ArduinoOTA.begin();

  //  update_systeminfo(true);

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

#ifdef DATAMAPLENGTH_ADD
  DataMap.length += DATAMAPLENGTH_ADD;
#endif

#ifdef SONOFFCH
  DataMap.length += SONOFFCH;
#endif

#ifdef ESPMQTT_WATERMETER2
  DataMap.status = DataMapStatus::online;
  watermeter_init(WATERPULSEPIN, NODEMCULEDPIN, 0);
#endif

#ifdef  ESPMQTT_WATERMETER
  DataMap.status = DataMapStatus::online;
  i2cEeprom_init(I2C_SDA, I2C_SCL, I2C_EEPROM_ADDRESS, EE24LC512MAXBYTES);
  uint32_t watermeter_liters = i2cEeprom_read();
  watermeter_init(WATERPULSEPIN, NODEMCULEDPIN, watermeter_liters);
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
  if (!oneWireSensors.getAddress(onewire_chReturnWaterThermometer, 1)) {
    DEBUG_E("Unable to find address for Device onewire_chReturnWaterThermometer\n");
  }
#endif

#ifdef  ESPMQTT_WEATHER
  DataMap.status = DataMapStatus::online;
  if (!oneWireSensors.getAddress(onewire_OutsideAddress, 0)) {
    DEBUG_E("Unable to find address for onewire_outsidetemp\n");
    DataMap.status = DataMapStatus::commerror;
  }
  else DataMap.status = DataMapStatus::online;
#endif

#ifdef  ESPMQTT_SONOFFTH
  if (!oneWireSensors.getAddress(onewire_address, 0)) {
    DEBUG_E("Unable to find address for onewire temperature sensor\n");
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



  systemTimer.attach_ms(100, systemTimerCallback);
  // In my case the ZMAI90 is always on because it is the main power feed of the house
#ifdef ESPMQTT_ZMAI90
  digitalWrite(ZMAI90RELAY, 1);
  pinMode(ZMAI90RELAY, OUTPUT);
#endif


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
  DataMap.length += 11;
#endif

#ifdef  ESPMQTT_OPENTHERM
  opentherm_init(openthermcallback);
#endif

#ifdef  ESPMQTT_GROWATT
  growatt_init(growattcallback, FANPIN);
  DataMap.length += 16;
#endif

#ifdef  ESPMQTT_GOODWE
  goodwe_init(goodwecallback);
#endif

#ifdef  ESPMQTT_GROWATT_MODBUS
  growattModbus_init(growattModbuscallback);
  DataMap.length += GROWATT_MODBUS_DATAMAP_LENGTH;
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
  ducobox_init(sonoff_relays[0], sonoff_relays[1], ducoboxcallback);
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
  dhtdevice.begin();
  DataMap.length += 3;
#endif

#ifdef QSWIFIDIMMERCHANNELS
  qswifidimmer_init(QSWIFIDIMMERCHANNELS, qswifidimmer_callback);
  qswifidimmer_setswitchcallback(qswifidimmer_switchcallback);
#endif

#ifdef QSWIFISWITCHCHANNELS
  qswifiswitch.setSwitchCallback(qswifiswitch_switchcallback);
  qswifiswitch.setRelayCallback(qswifiswitch_relaycallback);
  qswifiswitch.setRelay(0, 0);
  qswifiswitch.setRelay(1, 0);
#endif

#ifdef MH_Z19
  MHZ19_init();
#endif

#ifdef ESPMQTT_BBQTEMP
  SPI.begin();
  pinMode(ESPMQTT_BBQTEMP_CS0, OUTPUT);
  pinMode(ESPMQTT_BBQTEMP_CS1, OUTPUT);
  digitalWrite(ESPMQTT_BBQTEMP_CS0, HIGH);
  digitalWrite(ESPMQTT_BBQTEMP_CS1, HIGH);
#endif

  strcpy(DataMap.firmware_upgradekey, getRandomString(10).c_str()); // Set firmware_upgradekey to a random string

  setDataMapPublishRegular(10); //uptime
  setDataMapPublishRegular(11); //freeram
  setDataMapPublishRegular(15); //bssid
  setDataMapPublishRegular(16); //rssi
  setDataMapPublishRegular(17); //channel
  connectToWifi(); // After everything is set, connect to wifi.
}


static void handleDataExternalIpServer(void*, AsyncClient * client, void *data, size_t len) {
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
  strcpy (DataMap.externalip, datastring.c_str());
  client->close();
}

void onConnectExternalIpServer(void*, AsyncClient * client) {
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
