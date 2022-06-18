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
#define DEBUGLEVEL Debug.DEBUG
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
// #define ESPMQTT_SMARTMETER
#define ESPMQTT_GROWATT
// #define ESPMQTT_GROWATT_MODBUS
// #define ESPMQTT_SDM120
// #define ESPMQTT_DDM18SD
// #define ESPMQTT_WATERMETER
// #define ESPMQTT_GENERIC8266
// #define ESPMQTT_MAINPOWERMETER
// #define ESPMQTT_OBD2
// #define ESPMQTT_NOISE
// #define ESPMQTT_SOIL
// #define ESPMQTT_DIMMER
// #define ESPMQTT_RELAY
// #define ESPMQTT_LIVINGROOM
// #define ESPMQTT_BBQTEMP
// #define ESPMQTT_GOODWE
// #define ESPMQTT_WHR930

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

#ifdef  ESPMQTT_WHR930
#define FIRMWARE_TARGET "WHR930"
#define ESPLED 2
#define APONBOOT
#undef SERIALLOG
#define DHTPIN 4
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
#include "whr930.h"
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
#endif

#define EEPROMSTRINGSIZE 40 // 2 positions are used, one for 0 character and one for checksum

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

struct Triggers {
  bool wificonnected = false;
  bool wifidisconnected = false;
  bool mqttconnected = false;
  bool mqttdisconnected = false;
  bool wifiscanready = false;
  bool mqttsubscribed = false;
  String firmwareupgrade = "";
} triggers;

struct Mainstate {
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
} mainstate;

struct dataMapStruct {
  char *payload = (char *) "";;
  bool send = true;
  bool onair = false;
  bool publishregular = false;
};

int wifichannel = 0;
int wifinetworksfound = 0;


char _gFmtBuf[_gFmtBufSize];

SimpleMap<const char*, dataMapStruct> *dataMap = new SimpleMap<const char*, dataMapStruct>([](const char* &a, const char* &b) -> int {
  return strcmp(a,b);      // a and b are equal
});


bool updatemqtt = 0;

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
String mqtt_server = "";
String mqtt_username = "";
String mqtt_password = "";
String mqtt_topicprefix = "";
int mqtt_port = 1883;
bool mqtt_ssl = 0;
String esp_password = DEFAULT_PASSWORD;
String esp_hostname = "";
RemoteDebug Debug;
String wifissid = "";
String wifipsk = "";

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


void publishdatamap(int32_t packetId = -1, bool publishall = false, bool init = false, bool publishregular = false);

String getRandomString(int len) {
  String alphanum = sF("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

  String returnstring = "";

  for (int i = 0; i < len; ++i) {
    returnstring += alphanum.charAt(random(alphanum.length() - 1));
  }

  return returnstring;
}

void updateexternalip();

String getdatamap(const char *topic)
{
  return dataMap->get(topic).payload;
  yield();
}

void showdatamap()
{
  for (int i = 0; i < dataMap->size(); i++)
  {
    Debug.printf(cF("%s=%s (send=%d, onair=%d)\n"), dataMap->getKey(i), dataMap->getData(i).payload, dataMap->getData(i).send, dataMap->getData(i).onair);
    yield();
  }
}

void putdatamap(const char *topic, int value, bool sendupdate, bool forceupdate, bool publishregular)
{
  putdatamap(topic, String(value), sendupdate, forceupdate, publishregular);
}

void putdatamap(const char *topic, String value, bool sendupdate, bool forceupdate, bool publishregular)
{
  dataMapStruct datamapstruct;
  char *datamapTopic;


  if (dataMap->has(topic))
  {
    datamapstruct = dataMap->get(topic);
    if ((strcmp (datamapstruct.payload , value.c_str()) == 0) && !forceupdate) return;

    if (strcmp(topic, "status") == 0)
    {
      if (strcmp (datamapstruct.payload, "upgrading") == 0)
      {
        // When upgrading only accept upgradefailed or upgradedone as value
        if ((value != "upgrade_exit") && (value != "rebooting")) return;
        if (value == "upgrade_exit") value = "online";
      }
    }
  }

  // Do not output debug for uptime
  if (strcmp(topic, "system/uptime") != 0) if (Debug.isActive(Debug.INFO)) Debug.printf("DATAMAP %s=%s (sendupdate=%d, oldval=%s oldsend=%d forceupdate=%d)\n", topic, value.c_str(), sendupdate, datamapstruct.payload, datamapstruct.send, forceupdate);

  datamapstruct.onair = false;
  datamapstruct.send = sendupdate;
  datamapstruct.publishregular = publishregular;

  int n = value.length() + 1;

  // Remove old payload from memory
  if (!dataMap->has(topic))
  {
    datamapstruct.payload = (char*) malloc(n * sizeof(char));
    datamapTopic = (char*) malloc((strlen(topic) + 1)* sizeof(char));
    strcpy(datamapTopic, topic);
  }
  else
  {
    datamapstruct.payload = (char*) realloc (datamapstruct.payload, n * sizeof(char));
    datamapTopic = (char*) dataMap->getKey(dataMap->getIndex(topic));
  }

  // Copy value to memory
  strcpy(datamapstruct.payload, value.c_str());

  dataMap->put(datamapTopic, datamapstruct);

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
        if (serialstring == sF("UNABLE TO CONNECT"))
        {
          putdatamap(cF("status"), cF("commerror"));
        }
        switch (obdcmd)
        {
          case 0:
            if (serialstring.indexOf(sF("ELM327")) == 0)
            {
              serialstring.remove(0, serialstring.indexOf(" v") + 2);
              serialstring.remove(serialstring.indexOf(13));
              if (Debug.isActive(Debug.INFO)) Debug.printf(cF("\n\n\nOBD2 Connected! Version=%s\n\n\n"), serialstring.c_str());
              putdatamap(cF("obd/version"), serialstring);
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
              putdatamap(cF("obd/temperature/coolant"), value);
              putdatamap(cF("status"), sF("connected"));
              obdcmd++;
            }

            if (serialstring.indexOf("41 0D") == 0)
            {
              value = String(strtol(serialstring.substring(6, 8).c_str(), NULL, 16));
              putdatamap(cF("obd/speed"), value);
              putdatamap(cF("status"), sF("connected"));
              nextsleeptime = millis() + 30000;
              obdcmd++;
            }

            if (serialstring.indexOf("41 0C") == 0)
            {
              int A = strtol(serialstring.substring(6, 8).c_str(), NULL, 16);
              int B = strtol(serialstring.substring(9, 11).c_str(), NULL, 16);
              value = String(((A * 256) + B) / 4);
              putdatamap(cF("obd/enginerpm"), value);
              putdatamap(cF("status"), sF("connected"));
              obdcmd++;
            }

            if (serialstring.indexOf("41 11") == 0)
            {
              value = String((strtol(serialstring.substring(6, 8).c_str(), NULL, 16) * 100) / 255);
              putdatamap(cF("obd/throttleposition"), value);
              putdatamap(cF("status"), sF("connected"));
              obdcmd++;
            }

            if (serialstring.indexOf("41 0F") == 0)
            {
              value = String(strtol(serialstring.substring(6, 8).c_str(), NULL, 16) - 40);
              putdatamap(cF("obd/temperature/intake"), value);
              putdatamap(cF("status"), sF("connected"));
              obdcmd++;
            }

            if (serialstring.indexOf("41 46") == 0)
            {
              value = String(strtol(serialstring.substring(6, 8).c_str(), NULL, 16) - 40);
              putdatamap(cF("obd/temperature/ambient"), value);
              putdatamap(cF("status"), sF("connected"));
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

void update_systeminfo(bool writestaticvalues = false)
{
  char uptimestr[20];
  const bool sendupdate = true;
  sprintf(uptimestr, cF("%d:%02d:%02d:%02d"), uptime / 86400, (uptime / 3600) % 24, (uptime / 60) % 60, uptime % 60);
  if (writestaticvalues)
  {
    putdatamap(cF("hostname"), WiFi.hostname(), sendupdate, false, false);
    String firmwarename = __FILE__;
    firmwarename = firmwarename.substring(firmwarename.lastIndexOf("/") + 1);
    firmwarename = firmwarename.substring(firmwarename.lastIndexOf("\\") + 1);
    firmwarename = firmwarename.substring(0, firmwarename.lastIndexOf("."));
    putdatamap(cF("firmware/name"), firmwarename, sendupdate, false, false);
    putdatamap(cF("firmware/target"), FIRMWARE_TARGET, sendupdate, false, false);
    //putdatamap(cF("firmware/sourcefile", String(__FILE__).substring(String(__FILE__).lastIndexOf('/') + 1), sendupdate, false, false);
    putdatamap(cF("firmware/version"), ESPMQTT_VERSION, sendupdate, false, false);
    putdatamap(cF("firmware/compiletime"), String(__DATE__) + " " + __TIME__, sendupdate, false, false);
    putdatamap(cF("firmware/upgradekey"), getRandomString(10), sendupdate, false, false);
    putdatamap(cF("status"), sF("online"), sendupdate, false, false);
    putdatamap(cF("status/upgrade"), sF("not checked"), sendupdate, false, false);
    //putdatamap(cF("flash/id", String(ESP.getFlashChipId()), sendupdate, false, false);
    //putdatamap(cF("flash/size/real", String(ESP.getFlashChipRealSize()), sendupdate, false, false);
    //putdatamap(cF("flash/size/ide", String(ESP.getFlashChipSize()), sendupdate, false, false);
    //FlashMode_t ideMode = ESP.getFlashChipMode();
    //putdatamap(cF("flash/mode", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"), sendupdate, false, false);
    //putdatamap(cF("flash/speed", String(ESP.getFlashChipSpeed()), sendupdate, false, false);
    putdatamap(cF("system/chipid"), String(chipid), sendupdate, false, false);
    putdatamap(cF("wifi/mac"), String(WiFi.macAddress()), sendupdate, false, false);
  }
  putdatamap(cF("system/uptime"), String(uptimestr), uptime % 60 == 0, true, false);
  putdatamap(cF("system/freeram"), String(system_get_free_heap_size()), (uptime % 60 == 0), false, false);
  putdatamap(cF("wifi/state"), WiFi.status() == WL_CONNECTED ? sF("connected") : sF("disconnected"), sendupdate, false, false);
  putdatamap(cF("wifi/localip"), WiFi.localIP().toString(), sendupdate, false, false);
  putdatamap(cF("wifi/ssid"), String(WiFi.SSID()), sendupdate, false, false);
  putdatamap(cF("wifi/bssid"), String(WiFi.BSSIDstr()), sendupdate, false, false);
  if ((abs(getdatamap(cF("wifi/rssi")).toInt() - WiFi.RSSI()) > 5) || (uptime % 60 == 0)) putdatamap(cF("wifi/rssi"), String(WiFi.RSSI()), sendupdate, false, false);
  putdatamap(cF("wifi/channel"), String(wifichannel), sendupdate, false, false);
  putdatamap(cF("mqtt/server"), String(mqtt_server), sendupdate, false, false);
  putdatamap(cF("mqtt/port"), String(mqtt_port), sendupdate, false, false);
  putdatamap(cF("mqtt/ssl"), String(mqtt_ssl), sendupdate, false, false);
  putdatamap(cF("mqtt/state"), mqttClient.connected() ? sF("connected") : sF("disconnected"), sendupdate, false, false);
  putdatamap(cF("mqtt/clientid"), String(mqttClient.getClientId()), sendupdate, false, false);
  putdatamap(cF("mqtt/user"), mqtt_username, sendupdate, false, false);
}

void startWifiAP()
{
  if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Starting WiFi Accesspoint\n"));
  if (WiFi.softAP(WiFi.hostname().c_str(), DEFAULT_PASSWORD, 6, 0))
  {
    mainstate.accesspoint = true;
    if (Debug.isActive(Debug.INFO)) Debug.printf(cF("WiFi Accesspoint Activated\n"));
    static uint8 routermode = 0;
    wifi_softap_set_dhcps_offer_option(OFFER_ROUTER, &routermode);
    WiFi.mode(WIFI_AP);
    esp_password = DEFAULT_PASSWORD;
    mainstate.defaultpassword = true;
    ArduinoOTA.setPassword(esp_password.c_str());
    Debug.setPassword(esp_password);
#ifdef ESPMQTT_TUYA_2GANGDIMMERV2
    //tuya_apmode();
#endif
  }
}

void stopWifiAP()
{
  if (Debug.isActive(Debug.INFO)) Debug.printf(cF("Stopping accesspoint, connecting to accesspoint.\n"));
  mainstate.accesspoint = false;
  connectToWifi();
}

void connectToWifi()
{
  // If no ssid was configured start accesspoint
  if (wifissid != "")
  {
    if (!mainstate.wificonnected && !mainstate.accesspoint)
    {
      if (Debug.isActive(Debug.INFO)) Debug.printf(cF("Connecting to Wi-Fi: SSID='%s' HOSTNAME='%s'...\n"), wifissid.c_str(), esp_hostname.c_str());

      WiFi.persistent(true); // Save wifi credentials for other firmware
      WiFi.setAutoReconnect(false); // We handle reconnect our self
      WiFi.setSleepMode(WIFI_MODEM_SLEEP);
      //WiFi.setSleepMode(WIFI_NONE_SLEEP); // When sleep is on regular disconnects occur https://github.com/esp8266/Arduino/issues/5083
      WiFi.setOutputPower(20);        // 10dBm == 10mW, 14dBm = 25mW, 17dBm = 50mW, 20dBm = 100mW
      WiFi.hostname(esp_hostname);
      WiFi.mode(WIFI_STA);

      ArduinoOTA.setHostname(esp_hostname.c_str());
      ArduinoOTA.setPassword(esp_password.c_str());
      Debug.setPassword(esp_password);

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
  if (Debug.isActive(Debug.INFO)) Debug.printf(cF("Connected to MQTT sessionPresent=%d\n"), sessionPresent);
  mqttReconnectTimer.detach();
  mainstate.mqttconnected = true;
  triggers.mqttconnected = true;
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
  if (Debug.isActive(Debug.WARNING)) Debug.printf(cF("Disconnected from MQTT reason:%d\n"), reason);

  if (reason == AsyncMqttClientDisconnectReason::TLS_BAD_FINGERPRINT) {
    if (Debug.isActive(Debug.WARNING)) Debug.printf(cF("Bad server fingerprint.\n"));
  }

  mainstate.mqttconnected = false;
  mainstate.mqttready = false;
  triggers.mqttdisconnected = true;
}

void onMqttUnsubscribe(uint16_t packetId) {
  if (Debug.isActive(Debug.INFO)) Debug.printf(cF("Unsubscribe acknowledged. packetid=%d\n") + packetId);
}


void mqttdosubscriptions(int32_t packetId = -1)
{
  const uint8_t nr_of_subsribe_topics = 31;

  static int32_t nextpacketid = 1;
  static uint16_t nextsubscribe = 0;
  static String subscribetopic = ""; // We need this static variable because mqttclient.subscribe uses a pointer
  if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("mqttdosubscriptions (packetId=%d, nextpacketId=%d, nextsubscribe=%d)\n"), packetId, nextpacketid, nextsubscribe);

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
  
  subscribetopic = "";
  while ((subscribetopic == "") && (nextsubscribe <= nr_of_subsribe_topics))
  {
    //DEBUG("mqttdosubscriptions while nextsubscribe=%d\n", nextsubscribe);
    switch (nextsubscribe)
    {
#ifdef ESPMQTT_OPENTHERM
      case 0: subscribetopic = mqtt_topicprefix + sF("setthermostattemporary"); break;
      case 1: subscribetopic = mqtt_topicprefix + sF("setthermostatcontinue"); break;
      case 2: subscribetopic = mqtt_topicprefix + sF("setchwatertemperature"); break;
      case 3: subscribetopic = mqtt_topicprefix + sF("setmaxmodulationlevel"); break;
      case 4: subscribetopic = mqtt_topicprefix + sF("setoutsidetemperature"); break;
#endif
#ifdef ESPMQTT_DUCOBOX
      case 5: subscribetopic = mqtt_topicprefix + sF("setfan"); break;
#endif
#ifdef ESPMQTT_DIMMER
      case 6: subscribetopic = mqtt_topicprefix + sF("setdimvalue"); break;
      case 7: subscribetopic = mqtt_topicprefix + sF("setdimstate"); break;
#endif
#ifdef ESPMQTT_SONOFFBULB
      case 8: subscribetopic = mqtt_topicprefix + sF("setcolor"); break;
#endif
#ifdef ESPMQTT_AMGPELLETSTOVE
      case 9: subscribetopic = mqtt_topicprefix + sF("setonoff"); break;
      case 10: subscribetopic = mqtt_topicprefix + sF("setpower"); break;
      case 11: subscribetopic = mqtt_topicprefix + sF("settemperature"); break;
#endif
#ifdef SONOFFCH
      case 12:  if (0 < SONOFFCH) subscribetopic = mqtt_topicprefix + sF("setrelay/0"); break;
      case 13:  if (1 < SONOFFCH) subscribetopic = mqtt_topicprefix + sF("setrelay/1"); break;
      case 14:  if (2 < SONOFFCH) subscribetopic = mqtt_topicprefix + sF("setrelay/2"); break;
      case 15:  if (3 < SONOFFCH) subscribetopic = mqtt_topicprefix + sF("setrelay/3"); break;
#endif
#ifdef  ESPMQTT_SONOFF_FLOORHEATING
      case 16: subscribetopic = mqtt_topicprefix + sF("setvalve"); break;
#endif
#ifdef ESPMQTT_QSWIFIDIMMERD01
      case 17:  subscribetopic = mqtt_topicprefix + sF("setdimvalue"); break;
      case 18:  subscribetopic = mqtt_topicprefix + sF("setdimstate"); break;
#endif
#if defined(ESPMQTT_QSWIFIDIMMERD02) || defined(ESPMQTT_TUYA_2GANGDIMMERV2)
      case 19:  subscribetopic = mqtt_topicprefix + sF("setdimvalue/0"); break;
      case 20:  subscribetopic = mqtt_topicprefix + sF("setdimvalue/1"); break;
      case 21:  subscribetopic = mqtt_topicprefix + sF("setdimstate/0"); break;
      case 22:  subscribetopic = mqtt_topicprefix + sF("setdimstate/1"); break;
#endif
#ifdef ESPMQTT_BHT002
      case 23: subscribetopic = mqtt_topicprefix + sF("setsetpoint"); break;
#endif
      case 24:  subscribetopic = mqtt_topicprefix + sF("startfirmwareupgrade"); break;
#ifdef ESPMQTT_QSWIFISWITCH1C
      case 25:  subscribetopic = mqtt_topicprefix + sF("setrelay"); break;
#endif
#ifdef ESPMQTT_QSWIFISWITCH2C
      case 26:  subscribetopic = mqtt_topicprefix + sF("setrelay/0"); break;
      case 27:  subscribetopic = mqtt_topicprefix + sF("setrelay/1"); break;
#endif
#ifdef ESPMQTT_WHR930
      case 28:  subscribetopic = mqtt_topicprefix + sF("setfanlevel"); break;
      case 29:  subscribetopic = mqtt_topicprefix + sF("setcomforttemperature"); break;
#endif
      default: break;
    }
    if (subscribetopic == "") nextsubscribe++;
  }

  if (subscribetopic == "") 
  {
    if (Debug.isActive(Debug.VERBOSE)) Debug.printf(cF("Subscribing to mqtt topics finished successfull\n"));
    mainstate.mqttsubscribed = true;
    triggers.mqttsubscribed = true; // When subscription has finished start publishing of datamap
  }
  else
  {
    if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("MQTT Subscribing to: %s\n"), subscribetopic.c_str());
    nextpacketid = mqttClient.subscribe(subscribetopic.c_str() , 1);
  }
  if (Debug.isActive(Debug.VERBOSE)) Debug.printf(cF("mqttdosubscriptions end nextpacketid=%d\n"), nextpacketid);
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
  if (Debug.isActive(Debug.VERBOSE)) Debug.printf(cF("Subscribe acknowledged packetid=%d qos=%d\n"), packetId, qos);
  mainstate.mqttlastsubscribedpacketid = packetId;
}


void onMqttPublish(uint16_t packetId)
{
  mainstate.mqttlastpublishedpacketid = packetId;
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties, size_t len, size_t, size_t) {
  String payloadstring = "";
  for (unsigned int i = 0; i < len; i++)
  {
    payloadstring += char(payload[i]);
  }
  String topicstring = topic;
  if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("MQTT RECEIVED topic=%s payload=%s\n"), topic, payloadstring.c_str());

#ifdef SONOFFCH
  for (byte i = 0; i < SONOFFCH; i++)
  {
    if (String(topic) == String(mqtt_topicprefix + sF("setrelay/") + i))
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
  if (String(topic) == String(mqtt_topicprefix + sF("setthermostattemporary")))
  {
    if (Debug.isActive(Debug.INFO)) Debug.printf(cF("RECEIVED SETTHERMOSTATTEMPORARY %s\n"), payloadstring.c_str());
    opentherm_setthermosttattemporary(payloadstring.toFloat());
  }
  if (String(topic) == String(mqtt_topicprefix + sF("setthermostatcontinue")))
  {
    if (Debug.isActive(Debug.INFO)) Debug.printf(cF("RECEIVED SETTHERMOSTATCONTINUE %s\n"), payloadstring.c_str());
    opentherm_setthermosttatcontinue(payloadstring.toFloat());
  }
  if (String(topic) == String(mqtt_topicprefix + sF("setchwatertemperature")))
  {
    if (Debug.isActive(Debug.INFO)) Debug.printf(cF("RECEIVED SETCHWATERTEMPERATURE %s\n"), payloadstring.c_str());
    opentherm_setchwatertemperature(payloadstring.toFloat());
  }
  if (String(topic) == String(mqtt_topicprefix + sF("setmaxmodulationlevel")))
  {
    if (Debug.isActive(Debug.INFO)) Debug.printf(cF("RECEIVED SETMAXMODULATIONLEVEL %s\n"), payloadstring.c_str());
    opentherm_setmaxmodulationlevel(payloadstring.toInt());
  }
  if (String(topic) == String(mqtt_topicprefix + sF("setoutsidetemperature")))
  {
    if (Debug.isActive(Debug.INFO)) Debug.printf(cF("RECEIVED SETOUTSIDETEMPERATURE %s\n"), payloadstring.c_str());
    opentherm_setoutsidetemperature(payloadstring.toFloat());
  }
#endif

#ifdef  ESPMQTT_SONOFF_FLOORHEATING
  if (String(topic) == String(mqtt_topicprefix + sF("setvalve"))) floorheating_valveon = payloadstring == "1" ? true : false;
#endif


#ifdef  ESPMQTT_DUCOBOX
  if (String(topic) == String(mqtt_topicprefix + sF("setfan"))) ducobox_setfan(payloadstring.toInt());
#endif

#ifdef  ESPMQTT_DIMMER
  if (String(topic) == String(mqtt_topicprefix + sF("setdimvalue")))
  {
    dimmer_setdimvalue(payloadstring.toInt());
    putdatamap(cF("dimvalue"), String(dimmer_getdimvalue()));
  }
#endif

#ifdef  ESPMQTT_SONOFFBULB
  if (String(topic) == String(mqtt_topicprefix + sF("setcolor")))
  {
    long number = strtol(payloadstring.substring(0, 6).c_str(), NULL, 16);
    _my92xx->setChannel(MY92XX_RED, (number >> 16) & 0xFF);
    _my92xx->setChannel(MY92XX_GREEN, (number >> 8) & 0xFF);
    _my92xx->setChannel(MY92XX_BLUE, number & 0xFF);

    number = strtol(payloadstring.substring(6, 10).c_str(), NULL, 16);
    _my92xx->setChannel(MY92XX_CW, (number >> 8) & 0xFF);
    _my92xx->setChannel(MY92XX_WW, number & 0xFF);
    _my92xx->update();
    putdatamap(cF("color"), payloadstring);
  }
#endif

#ifdef  ESPMQTT_AMGPELLETSTOVE
  amgpelletstove_receivemqtt(topicstring, payloadstring);
#endif

#ifdef ESPMQTT_QSWIFIDIMMERD01
  if (topicstring == mqtt_topicprefix + sF("setdimvalue")) qswifidimmer_setdimvalue(payloadstring.toInt());
  if (topicstring == mqtt_topicprefix + sF("setdimstate")) qswifidimmer_setdimstate(payloadstring.toInt() ? 1 : 0);
#endif
#ifdef ESPMQTT_QSWIFIDIMMERD02
  if (topicstring == mqtt_topicprefix + sF("setdimvalue/0")) qswifidimmer_setdimvalue(payloadstring.toInt(), 0);
  if (topicstring == mqtt_topicprefix + sF("setdimvalue/1")) qswifidimmer_setdimvalue(payloadstring.toInt(), 1);
  if (topicstring == mqtt_topicprefix + sF("setdimstate/0")) qswifidimmer_setdimstate(payloadstring.toInt(), 0);
  if (topicstring == mqtt_topicprefix + sF("setdimstate/1")) qswifidimmer_setdimstate(payloadstring.toInt(), 1);
#endif
#ifdef ESPMQTT_QSWIFISWITCH1C
  if (topicstring == mqtt_topicprefix + sF("setrelay")) qswifiswitch.setRelay(payloadstring.toInt());
#endif
#ifdef ESPMQTT_QSWIFISWITCH2C
  if (topicstring == mqtt_topicprefix + sF("setrelay/0")) qswifiswitch.setRelay(0, payloadstring.toInt());
  if (topicstring == mqtt_topicprefix + sF("setrelay/1")) qswifiswitch.setRelay(1, payloadstring.toInt());
#endif


  if (topicstring == mqtt_topicprefix + sF("startfirmwareupgrade"))
  {
    triggers.firmwareupgrade = payloadstring;
  }

#ifdef ESPMQTT_BHT002
  if (topicstring == mqtt_topicprefix + sF("setsetpoint")) bht002_setsetpoint(payloadstring.toInt());
#endif

#ifdef ESPMQTT_TUYA_2GANGDIMMERV2
  if (topicstring == mqtt_topicprefix + sF("setdimvalue/0")) tuya_2gangdimmerv2_setdimvalue(payloadstring.toInt(), 0);
  if (topicstring == mqtt_topicprefix + sF("setdimvalue/1")) tuya_2gangdimmerv2_setdimvalue(payloadstring.toInt(), 1);
  if (topicstring == mqtt_topicprefix + sF("setdimstate/0")) tuya_2gangdimmerv2_setdimstate(payloadstring.toInt(), 0);
  if (topicstring == mqtt_topicprefix + sF("setdimstate/1")) tuya_2gangdimmerv2_setdimstate(payloadstring.toInt(), 1);
#endif


#ifdef ESPMQTT_WHR930
  if (topicstring == mqtt_topicprefix + sF("setfanlevel")) zehnder_whr930.setfanlevel(payloadstring.toInt());
  if (topicstring == mqtt_topicprefix + sF("setcomforttemperature")) zehnder_whr930.setcomforttemperature(payloadstring.toInt());
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
  static String willtopic = String(mqtt_topicprefix) + sF("status");

  if (Debug.isActive(Debug.INFO)) Debug.printf(cF("Connecting to Mqtt\n"));
  if (!mainstate.mqttconnected)
  {
    mqttClient.disconnect();
    mqttClient.setCredentials(mqtt_username.c_str(), mqtt_password.c_str());
    mqttClient.setServer(mqtt_server.c_str(), mqtt_port);
    mqttClient.setWill(willtopic.c_str(), 0, 1, "offline");
    mqttClient.setSecure(mqtt_ssl);
    mqttClient.connect();
    mqttReconnectTimer.once(30, connectToMqtt); // retry over 30 seconds if connection can not be established
  }
}

void disconnectMqtt()
{
  if(Debug.isActive(Debug.WARNING)) Debug.printf(cF("Disconnecting Mqtt\n"));
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
    if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Reading DHT22 failed!\n"));
    if (errors < 255) errors++;
  }
  else errors = 0;

  if (Debug.isActive(Debug.INFO)) Debug.printf(cF("DHT22 temp=%.1f, rh=%.1f, hi=%.1f\n"), temp, rh, hi);

    putdatamap(cF("dht22/temperature"), errors < 5 ? String(temp, 1) : "-");
    putdatamap(cF("dht22/humidity"), errors < 5 ? String(rh, 1) : "-");
    putdatamap(cF("dht22/heatindex"), errors < 5 ? String(hi, 1) : "-");

#ifdef NEOPIXELPIN
  if (rh < 1) neopixelleds.setPixelColor(1, neopixelleds.Color(0, 0, 0));
  else if (rh < 60) neopixelleds.setPixelColor(1, neopixelleds.Color(0, 20, 0));
  else if (rh < 90) neopixelleds.setPixelColor(1, neopixelleds.Color(30, 15, 0));
  else neopixelleds.setPixelColor(1, neopixelleds.Color(50, 0, 0));
  neopixelleds.show();
#endif
}
#endif

void write_oled_display()
{
#ifdef OLED_ADDRESS
  int lcdline = -9;
  if(Debug.isActive(Debug.VERBOSE)) Debug.printf(cF("Writing OLED display\n"));
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
      if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("Analog read=%d, Analog mean=%.0f\n"), analogread, analogmean);
      putdatamap(cF("noise"), String(analogmean, 0));
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
      if (Debug.isActive(Debug.INFO)) Debug.printf(cF("Flash button is pressed, flashbuttontimer=%d\n"), flashbuttontimer);
    }
    else
    {
      flashbuttontimer = 0;
    }

    if (flashbuttontimer == 3) // After 3 seconds clear passwords
    {
      flashbuttonstatus = 2;
      esp_password = DEFAULT_PASSWORD;
      if (Debug.isActive(Debug.INFO)) Debug.printf(cF("Web Password defaulted to esplogin until reboot!\n"));
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

      if(Debug.isActive(Debug.VERBOSE)) Debug.printf(cF("Sending ZMAI request packet...\n"));

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
    Debug.printf(cF("ZMAI90DATA=%d:%02X\n"), zmai90pointer, zmai90data);
    char str[3];
    sprintf(str, cF("%02X"), zmai90data);
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
        putdatamap(cF("energy/kwh"), String((double)zmai90value / 100, 2));
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
        putdatamap(cF("voltage"), String((double)zmai90value / 10, 1));
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
        putdatamap(cF("current"), String((double)zmai90value / 10000, 4));
        break;

      case 15:
        zmai90value = zmai90data;
        break;
      case 16:
        zmai90value += zmai90data * 100;
        putdatamap(cF("frequency"), String((double)zmai90value / 100, 2));
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
        putdatamap(cF("power/active"), String((double)zmai90value / 100, 1));
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
        putdatamap(cF("power/reactive"), String((double)zmai90value / 100, 1));
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
        putdatamap(cF("power/apparent"), String((double)zmai90value / 100, 1));
        break;
    }
    zmai90pointer++;
  }
}
#endif

#ifdef  ESPMQTT_WATERMETER
void espmqtt_watermeter_handle()
{
  static uint32_t watermeter_liters = watermeter_getliters();
  if (watermeter_handle())
  {
    putdatamap(cF("water/lmin"), String(watermeter_getflow(), 1));
    putdatamap(cF("water/m3h"), String(watermeter_getflow() * 0.06, 3));

    if (watermeter_liters != watermeter_getliters())
    {
      watermeter_liters = watermeter_getliters();
      i2cEeprom_write(watermeter_liters);
      //syslogN("Watermeter Liters Changed=%d\n", watermeter_liters);
      putdatamap(cF("water/liter"), String(watermeter_liters));
      putdatamap(cF("water/m3"), String((double(watermeter_liters) / 1000), 3));
    }
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
    String mqtttopic = String(mqtt_topicprefix + sF("rain/pulse"));
    mqttClient.publish(mqtttopic.c_str(), 0, false, "1");
    count = 0;
  }
  if (rainpinstate == 0)
  {
    rainpinmillis = millis();
    count = 1;
  }
  putdatamap(cF("rain/pulses"), String(rainpulses));
  putdatamap(cF("rain/mm"), String((double(rainpulses)*RAINMETERPULSEMM), 1));

  static bool hourreg = false;
  static bool firsthourreg = true;
  if ((uptime % 3600) || (firsthourreg))
  {
    putdatamap(cF("rain/hour/pulses"), String(rainpulseshour));
    putdatamap(cF("rain/hour/mm"), String((double(rainpulseshour)*RAINMETERPULSEMM), 1));
    firsthourreg = false;
    if (!hourreg)
    {
      putdatamap(cF("rain/lasthour/pulses"), String(rainpulseshour));
      putdatamap(cF("rain/lasthour/mm"), String((double(rainpulseshour)*RAINMETERPULSEMM), 1));
      rainpulseshour = 0;
      hourreg = true;
    }
  }
  else hourreg = false;

  static bool minreg = false;
  static bool firstminreg = true;
  if ((uptime % 60) || (firstminreg))
  {
    putdatamap(cF("rain/minute/pulses"), String(rainpulsesminute));
    putdatamap(cF("rain/minute/mm"), String((double(rainpulsesminute)*RAINMETERPULSEMM), 1));
    firstminreg = false;
    if (!minreg)
    {
      putdatamap(cF("rain/lastminute/pulses"), String(rainpulsesminute));
      putdatamap(cF("rain/lastminute/mm"), String((double(rainpulsesminute)*RAINMETERPULSEMM), 1));
      rainpulsesminute = 0;
      minreg = true;
    }
  }
  else minreg = false;

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
      putdatamap(cF("status"), sF("querying"));
      putdatamap(cF("voltage"), doubletostring(sdm.readVal(SDM120CT_VOLTAGE), 2));
      break;
    case 2:
      putdatamap(cF("current"), doubletostring(sdm.readVal(SDM120CT_CURRENT), 2));
      break;
    case 3:
      putdatamap(cF("power"), doubletostring(sdm.readVal(SDM120CT_POWER), 2));
      break;
    case 4:
      putdatamap(cF("power/apparant"), doubletostring(sdm.readVal(SDM120CT_APPARENT_POWER), 2));
      break;
    case 5:
      putdatamap(cF("power/reactive"), doubletostring(sdm.readVal(SDM120CT_REACTIVE_POWER), 2));
      break;
    case 6:
      putdatamap(cF("frequency"), doubletostring(sdm.readVal(SDM120CT_FREQUENCY), 2));
      break;
    case 7:
      putdatamap(cF("powerfactor"), doubletostring(sdm.readVal(SDM120CT_POWER_FACTOR), 2));
      break;
    case 8:
      putdatamap(cF("energy/active/import"), doubletostring(sdm.readVal(SDM120CT_IMPORT_ACTIVE_ENERGY), 3));
      break;
    case 9:
      putdatamap(cF("energy/active/export"), doubletostring(sdm.readVal(SDM120CT_EXPORT_ACTIVE_ENERGY), 3));
      break;
    case 10:
      putdatamap(cF("energy/active"), doubletostring(sdm.readVal(SDM120CT_TOTAL_ACTIVE_ENERGY), 3));
      break;
    case 11:
      putdatamap(cF("energy/reactive/import"), doubletostring(sdm.readVal(SDM120CT_IMPORT_REACTIVE_ENERGY), 3));
      break;
    case 12:
      putdatamap(cF("energy/reactive/export"), doubletostring(sdm.readVal(SDM120CT_EXPORT_REACTIVE_ENERGY), 3));
      break;
    case 13:
      value = sdm.readVal(SDM120CT_TOTAL_REACTIVE_ENERGY);
      putdatamap(cF("energy/reactive"), doubletostring(value, 3));
      if (isnan(value)) putdatamap(cF("status"), sF("commerror"));
      else putdatamap(cF("status"), sF("ready"));
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
      putdatamap(cF("status"), sF("querying"));
      putdatamap(cF("voltage"), String(sdm.readVal(DDM18SD_VOLTAGE), 2));
      break;
    case 2:
      putdatamap(cF("current"), String(sdm.readVal(DDM18SD_CURRENT), 2));
      break;
    case 3:
      putdatamap(cF("power"), String(sdm.readVal(DDM18SD_POWER), 2));
      break;
    case 4:
      putdatamap(cF("power/reactive"), String(sdm.readVal(DDM18SD_REACTIVE_POWER), 2));
      break;
    case 5:
      putdatamap(cF("frequency"), String(sdm.readVal(DDM18SD_FREQUENCY), 2));
      break;
    case 6:
      putdatamap(cF("powerfactor"), String(sdm.readVal(DDM18SD_POWER_FACTOR), 2));
      break;
    case 7:
      putdatamap(cF("energy/active"), String(sdm.readVal(DDM18SD_IMPORT_ACTIVE_ENERGY), 3));
      break;
    case 8:
      putdatamap(cF("energy/reactive"), String(sdm.readVal(DDM18SD_IMPORT_REACTIVE_ENERGY), 3));
      if (isnan(value)) putdatamap(cF("status"), sF("commerror"));
      else putdatamap(cF("status"), sF("ready"));
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

#ifdef  ESPMQTT_WATERMETER
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
  static int mhz19ppm = 0;
  static int mhz19temp = 0;
  switch (MHZ19_handle(&mhz19ppm, &mhz19temp))
  {
    case 0:
      putdatamap(cF("mhz19/co2"), String(mhz19ppm));
      putdatamap(cF("mhz19/temperature"), String(mhz19temp));
      break;
    case 2:
      putdatamap(cF("mhz19/co2"), "-");
      putdatamap(cF("mhz19/temperature"), "-");
      break;
    default:
      break;
  }
#endif

#ifdef ESPMQTT_WHR930
  zehnder_whr930.loop();
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

void espmqtt_handle_modules_1sec()
{
#ifdef  ESPMQTT_BBQTEMP
    double temp = MAX6675_readCelsius(ESPMQTT_BBQTEMP_CS0);
    putdatamap(cF("temperature/0"), temp == NAN ? "-" : String(temp, 1));
    temp = MAX6675_readCelsius(ESPMQTT_BBQTEMP_CS1);
    putdatamap(cF("temperature/1"), temp == NAN ? "-" : String(temp, 1));
#endif

#ifdef  ESPMQTT_SONOFFPOWR2
    static uint64_t deciwattsec = 0;
    deciwattsec += powerval >= 0 ? (powerval * 10) : 0;
    if ((uptime % 5) == 0) // Every 5 seconds send update about power usage
    {
      putdatamap(cF("voltage"), voltval >= 0 ? String(voltval, 1) : "-");
      putdatamap(cF("current"), currentval >= 0 ? String(currentval, 3) : "-");
      putdatamap(cF("power"), powerval >= 0 ? String(powerval, 1) : "-");
      putdatamap(cF("power/apparent"), currentval >= 0 ? String(voltval * currentval, 1) : "-");
      // Convert deciwattsec to watt per second (ws) string
      uint32_t lowws = (deciwattsec / 10) % 0xFFFFFFFF;
      uint32_t highws = ((deciwattsec / 10) >> 32) % 0xFFFFFFFF;
      String ws = (highws > 0 ? String(highws) : "") + String(lowws);
      putdatamap(cF("energy/ws"), ws);
      // Convert watt per second to kwh
      String wh = (highws > 0 ? String(highws / 3600) : "") + String(lowws / 3600);
      wh = String(wh.length() < 4 ? "0" : "") + String(wh.length() < 3 ? "0" : "") + String(wh.length() < 2 ? "0" : "") + wh; // Add leading zeros to wh before converting to kwh
      String kwh = wh.substring(0, wh.length() - 3) + "." + wh.substring(wh.length() - 3); // Add decimal for wh to kwh conversion;
      putdatamap(cF("energy/kwh"), kwh);
    }
#endif

#ifdef  ESPMQTT_MAINPOWERMETER
    static uint8_t circuitnr = 0;
    int32_t mW;
    int32_t mVA;
    int32_t mA;
    int32_t mV;
    uint8_t nrofsamples;
    circuitspowermeter_read(circuitnr, mW, mVA, mA, mV, 10);
    if (circuitnr == 0) putdatamap(cF("mainsvoltage", String(mV / 1000));
    //putdatamap(cF("circuit/" + String((circuitnr + 1)) + "/mA", String(mA));
    //putdatamap(cF("circuit/" + String((circuitnr + 1)) + "/W", String(mW / 1000));
    //putdatamap(cF("circuit/" + String((circuitnr + 1)) + "/VA", String(mVA / 1000));
    if (circuitnr < 14) circuitnr++;
    else circuitnr = 0;
#endif

#ifdef DHTPIN
    if (uptime % 5 == 0) update_dht();
#endif

#ifdef ONEWIREPIN
    if (uptime % 10 == 0)
    {
      if(Debug.isActive(Debug.VERBOSE)) Debug.printf(cF("Requesting DS18B20 temperatures...\n"));
      oneWireSensors.requestTemperatures();
      float temperature;
#ifdef  ESPMQTT_SONOFFTH
      temperature = oneWireSensors.getTempC(onewire_address);
      if (Debug.isActive(Debug.INFO)) Debug.printf(cF("temperature=%f\n"), temperature);
      if ((temperature != -127) && (temperature != 85)) putdatamap(cF("temperature"), String(temperature, 1));
      else putdatamap(cF("temperature"), "-");
#endif
#ifdef  ESPMQTT_OPENTHERM
      temperature = oneWireSensors.getTempC(onewire_chReturnWaterThermometer);
      if (Debug.isActive(Debug.INFO)) Debug.printf(cF("chreturnwatertemp=%f\n"), temperature);
      if ((temperature != -127) && (temperature != 85)) putdatamap(cF("ow/ch/returnwatertemperature"), String(temperature, 1));
      else putdatamap(cF("ow/ch/returnwatertemperature"), "-");
      temperature = oneWireSensors.getTempC(onewire_dcwSupplyWaterThermometer);
      if (Debug.isActive(Debug.INFO)) Debug.printf(cF("dcwsupplywatertemp=%f\n"), temperature);
      if ((temperature != -127) && (temperature != 85)) putdatamap(cF("ow/dcw/temperature"), String(temperature, 1));
      else putdatamap(cF("ow/dcw/temperature"), "-");
      yield();
#endif
#ifdef  ESPMQTT_WEATHER
      temperature = oneWireSensors.getTempC(onewire_OutsideAddress);
      if (Debug.isActive(Debug.INFO)) Debug.printf(cF("Outside Temperature=%f\n"), temperature);
      if ((temperature != -127) && (temperature != 85)) putdatamap(cF("temperature"), String(temperature, 1));
      else putdatamap(cF("temperature"), "-");
      yield();
#endif
#ifdef  ESPMQTT_SONOFF_FLOORHEATING
      temperature = oneWireSensors.getTempC(onewire_floorWaterAddress);
      if (Debug.isActive(Debug.INFO)) Debug.printf(cF("Floor Water Temperature=%f\n"), temperature);
      bool floorheatingrelayon = false;
      // When temperature exeeds maximum prevent pump from switching on
      if ((temperature != -127) && (temperature != 85))
      {
        putdatamap(cF("temperature"), String(temperature, 1));
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
      else putdatamap(cF("temperature"), "-");
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
    }

#ifdef ESPMQTT_WHR930
  zehnder_whr930.secondTick(uptime);
#endif

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
    if ((wifissid != ""))
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
    if (Debug.isActive(Debug.INFO)) Debug.printf(cF("Connected to WiFi SSID=%s RSSI=%d IP=%s\n"), WiFi.SSID().c_str(), WiFi.RSSI(), WiFi.localIP().toString().c_str());
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
    if(Debug.isActive(Debug.WARNING)) Debug.printf(cF("Disconnected from Wi-Fi.\n"));
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
    if(Debug.isActive(Debug.VERBOSE)) Debug.printf(cF("Acknowledged published packetid=%d\n"), mqttlastpublishedpacketid);
    publishdatamap(mqttlastpublishedpacketid);
  }
  else publishdatamap();
  yield();
  ESP.wdtFeed(); // Prevent watchdog to kick in...

  if (triggers.mqttconnected)
  {
    triggers.mqttconnected = false;
    if (Debug.isActive(Debug.INFO)) Debug.printf(cF("Connected to MQTT Server=%s\n"), mqtt_server.c_str());
    //syslogN("Connected to MQTT Server=%s\n", mqtt_server.c_str());
    dotasks();  // Prevent crash because of to many debug data to send
    update_systeminfo(true);
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
    if(Debug.isActive(Debug.VERBOSE)) Debug.printf(cF("Acknowledged subscribed packetid=%d\n"), mqttlastsubscribedpacketid);
    mqttdosubscriptions(mqttlastsubscribedpacketid);
  }
  yield();
  ESP.wdtFeed(); // Prevent watchdog to kick in...


  if (triggers.mqttsubscribed)
  {
    triggers.mqttsubscribed = false;
    publishdatamap(-1, true, true);
  }
  yield();
  ESP.wdtFeed(); // Prevent watchdog to kick in...


  if (triggers.mqttdisconnected)
  {
    triggers.mqttdisconnected = false;
    if(Debug.isActive(Debug.WARNING)) Debug.printf(cF("Disconnected from MQTT Server=%s\n"), mqtt_server.c_str());
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
      if(Debug.isActive(Debug.INFO)) Debug.printf(cF("Received startfirmwareupgrade, upgrade pending...\n"));
      putdatamap(cF("status"), sF("upgrading"));
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

        if(Debug.isActive(Debug.INFO)) Debug.printf(cF("Starting firmware upgrade\n upgradeversion=%s\n upgradeurl=%s\n upgradekey=%s\n"), upgradeversion.c_str(), upgradeurl.c_str(), upgradekey.c_str());

        if (upgradeversion == ESPMQTT_VERSION)
        {
          if(Debug.isActive(Debug.INFO)) Debug.printf(cF("Upgrade canceled, version is the same\n"));
          putdatamap(cF("status/upgrade"), sF("up to date, same firmware version"));
          putdatamap(cF("status"), sF("upgrade_exit"));
        }
        else if (getdatamap(cF("firmware/upgradekey")) != upgradekey)
        {
          if(Debug.isActive(Debug.INFO)) Debug.printf(cF("Upgrade canceled, upgradekey is incorrect\n"));
          putdatamap(cF("status/upgrade"), sF("error, incorrect upgradekey"));
          putdatamap(cF("status"), sF("upgrade_exit"));
        }
        else
        {
          if(Debug.isActive(Debug.INFO)) Debug.printf(cF("Starting upgrade from:%s\n"), upgradeurl.c_str());
          putdatamap(cF("status/upgrade"), sF("upgrading"));
          WiFiClient upgradeclient;
          t_httpUpdate_return ret = ESPhttpUpdate.update(upgradeclient, upgradeurl, ESPMQTT_VERSION);
          switch (ret)
          {
            case HTTP_UPDATE_FAILED:
              if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Firmware upgrade failed: %s.\n"), ESPhttpUpdate.getLastErrorString().c_str());
              putdatamap(cF("status/upgrade"), String(sF("error http: ") + ESPhttpUpdate.getLastErrorString()));
              putdatamap(cF("status"), sF("upgrade_exit"));
              break;
            case HTTP_UPDATE_NO_UPDATES:
              if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Firmware upgrade check finished, no new version available.\n"));
              putdatamap(cF("status/upgrade"), sF("up to date, same firmware version"));
              putdatamap(cF("status"), sF("upgrade_exit"));
              break;
            case HTTP_UPDATE_OK:
              if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Firmware upgrade done!\n")); // may not be called since we reboot the ESP
              putdatamap(cF("status/upgrade"), sF("upgrade finished"));
              putdatamap(cF("status"), sF("rebooting"));
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
      if (WiFi.SSID(i) == wifissid.c_str())
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

      String enctype = sF("None");
      switch (WiFi.encryptionType(i)) {
        case ENC_TYPE_WEP:
          enctype = sF("WEP");
          break;
        case ENC_TYPE_TKIP:
          enctype = sF("WPA");
          break;
        case ENC_TYPE_CCMP:
          enctype = sF("WPA2");
          break;
        case ENC_TYPE_NONE:
          enctype = sF("None");
          break;
        case ENC_TYPE_AUTO:
          enctype = sF("Auto");
          break;
      }


      if(Debug.isActive(Debug.VERBOSE)) Debug.printf(cF(" %d: %s, %s, Ch:%d (%ddBm) %s\n"), i, WiFi.SSID(i).c_str(), WiFi.BSSIDstr(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), enctype.c_str());
      Debug.handle();
      yield(); // Prevent crash because of to many debug data to send
      ESP.wdtFeed(); // Prevent watchdog to kick in...
    }

    if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("CurrentAp ID=%d SSID=%s BSSID=%s RSSI=%d(%d), Strongest AP ID=%d SSID=%s, BSSID=%s RSSI=%d(%d)\n"), currentwifiid, wifissid.c_str(), WiFi.BSSIDstr().c_str(), currentwifirssi, WiFi.RSSI(), strongestwifiid, WiFi.SSID(strongestwifiid).c_str(), WiFi.BSSIDstr(strongestwifiid).c_str(), WiFi.RSSI(strongestwifiid), strongestwifirssi);
    Debug.handle();
    yield();  // Prevent crash because of to many debug data to send
    ESP.wdtFeed(); // Prevent watchdog to kick in...

    if (!mainstate.accesspoint)
    {
      if ((strongestwifiid >= 0) && ((WiFi.RSSI() >= 0) || (currentwifiid == -1) || ((currentwifiid != strongestwifiid) && (currentwifirssi + 10 < strongestwifirssi))))
      {
        if(Debug.isActive(Debug.INFO)) Debug.printf(cF("Switching to stronger AP %d (%s, %s, %s)\n"), strongestwifiid, wifissid.c_str(), wifipsk.c_str(), WiFi.BSSIDstr(strongestwifiid).c_str());
        WiFi.begin(wifissid.c_str(), wifipsk.c_str(), WiFi.channel(strongestwifiid), WiFi.BSSID(strongestwifiid), 1);
        yield();
      }
    }
  }
  yield();
  ESP.wdtFeed(); // Prevent watchdog to kick in...

  espmqtt_handle_modules();
  yield();
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
        if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("Starting Wifi Scan...\n"));
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

    // Every 1 minutes (+/- 60 seconds) publish all mqtt data
    static int8_t dividefactor = random(-60, 60);
    if ((uptime > 60) && (((uptime + dividefactor) % 60) == 0))
    {
      if(Debug.isActive(Debug.INFO)) Debug.printf(cF("Regular publishing datamap...\n"));
      publishdatamap(-1, false, false, true);
    }
    yield();
    ESP.wdtFeed(); // Prevent watchdog to kick in...

    if ((uptime % 60) == 0)
    {
      char uptimestr[20];
      sprintf(uptimestr, cF("%d:%02d:%02d:%02d"), uptime / 86400, (uptime / 3600) % 24, (uptime / 60) % 60, uptime % 60);
      time_t now;
      time(&now);
      String strtime = ctime(&now);
      strtime.replace("\n", "");
      if (mainstate.wificonnected)
      {
        //syslogI("Uptime=%s DateTime=%s\n", uptimestr, strtime.c_str());
      }
      if (Debug.isActive(Debug.INFO)) Debug.printf(cF("Uptime=%s DateTime=%s\n"), uptimestr, strtime.c_str());
    }
    yield();
    ESP.wdtFeed(); // Prevent watchdog to kick in...


#ifdef APONBOOT
    if ((uptime == 60) && (!mainstate.wificonnectedonce))
    {
      if(Debug.isActive(Debug.WARNING)) Debug.printf(cF("Connection to wifi failed, starting accesspoint\n"));
      startWifiAP();
    }
    if (uptime == 660)
    {
      stopWifiAP();
    }
#endif

    // Every 10 seconds update system info
    if ((uptime % 10) == 0) update_systeminfo();

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
          if(Debug.isActive(Debug.INFO)) Debug.printf(cF("SONOFF BUTTON %d PRESSED\n"), i);
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
          if(Debug.isActive(Debug.INFO)) Debug.printf(cF("SONOFF RELAY IS %s\n"), digitalRead(sonoff_relays[i]) != inverse ? "ON" : "OFF");
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
    if (i == 0) putdatamap(cF("relay/0"), relaystate);
    if (i == 1) putdatamap(cF("relay/1"), relaystate);
    if (i == 2) putdatamap(cF("relay/2"), relaystate);
    if (i == 3) putdatamap(cF("relay/3"), relaystate);
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
    putdatamap(cF("voltage"), String(((double)filtered_voltage / 10), 0));
    putdatamap(cF("voltage/multiplier"), String(hlw8012.getVoltageMultiplier()));
    putdatamap(cF("current"), String(((double)filtered_current / 1000), 3));
    putdatamap(cF("current/multiplier"), String(hlw8012.getCurrentMultiplier()));
    putdatamap(cF("power"), String(((double)filtered_power / 10), 1));
    putdatamap(cF("power/apparent"), String(((double)filtered_apparent_power / 10), 1));
    putdatamap(cF("power/reactive"), String(((double)filtered_reactive_power / 10), 1));
    putdatamap(cF("power/factor"), String(hlw8012.getPowerFactor(), 2));
    putdatamap(cF("power/multiplier"), String(hlw8012.getPowerMultiplier()));
    putdatamap(cF("energy/ws"), String(hlw8012.getEnergy()));
    putdatamap(cF("energy/kwh"), String((double)hlw8012.getEnergy() / 3600000, 3));
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

  if (init)
  {
    waitingforack = false;
    datamappointer = 0;
    nextpacketId = -1;
  }

  if ((packetId != -1) || publishall) if(Debug.isActive(Debug.VERBOSE)) Debug.printf(sF("Publishdatamap packetId=%d publishall=%d datamappointer=%d datamapsize=%d nextpacketid=%d waitingforack=%d\n").c_str(), packetId, publishall, datamappointer, dataMap->size(), nextpacketId, waitingforack);

  yield();

  if (publishall || publishregular)
  {
    int32_t publishallpointer = 0;
    while (publishallpointer < dataMap->size())
    {
      const char *topic = dataMap->getKey(publishallpointer);
      dataMapStruct data = dataMap->getData(publishallpointer);
      data.onair = false;
      if (publishall) data.send = true;
      if (publishregular) data.send = data.publishregular;
      dataMap->put(topic, data);
      publishallpointer++;
      //DEBUG("publishallpointer=%d datamapsize=%d\n",publishallpointer, dataMap->size());
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
      if(Debug.isActive(Debug.VERBOSE)) Debug.printf(cF("Not received mqtt ack id=%d\n"), packetId);
      const char *topic = dataMap->getKey(datamappointer);
      dataMapStruct data = dataMap->getData(datamappointer);
      if (data.onair)
      {
        data.onair = false;
        dataMap->put(topic, data);
      }
      waitingforack = false;
    }
    else if (packetId == nextpacketId)
    {
      // Packet succesfull delivered proceed to next item
      if(Debug.isActive(Debug.VERBOSE)) Debug.printf(cF("Received mqtt ack id=%d\n"), packetId);
      const char *topic = dataMap->getKey(datamappointer);
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

  // If not waiting for ack search for next item in datamap which has to be send
  else if (mqttClient.connected() && (WiFi.status() == WL_CONNECTED) && mainstate.mqttsubscribed)
  {
    if (datamappointer < dataMap->size())
    {
      nextpacketId = -1;
      while ((datamappointer < dataMap->size()) && (nextpacketId == -1))
      {
        const char *topic = dataMap->getKey(datamappointer);
        dataMapStruct data = dataMap->getData(datamappointer);
        //DEBUG ("datamappointer=%d datamapsize=%d send=%d\n", datamappointer, dataMap->size(), data.send);
        if (data.send)
        {
          String sendtopic = String(mqtt_topicprefix + topic);
          
          nextpacketId = mqttClient.publish(sendtopic.c_str(), 1, true, data.payload);
          if (nextpacketId > 0)
          {
            waitingforack = true;
            data.onair = true;
            dataMap->put(topic, data);
          }
          if(Debug.isActive(Debug.DEBUG)) Debug.printf(cF("MQTT PUBLISHING DATAMAP %s=%s (nextpacketId=%d)\n"), topic, data.payload, nextpacketId);
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
}

#ifdef MH_Z19
uint8_t MHZ19_handle(int *ppm, int *temp)
{
  static unsigned long requesttime = 5000;
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
      if (Debug.isActive(Debug.INFO)) Debug.printf(cF("MHZ19 ppm=%d temp=%d\n"), *ppm, *temp);
      if (ppm == 0) return 1;
      else return 0;
    }
    else
    {
      if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("MHZ19 read error!\n"));
      return 2;
    }
  }
  return 1;
}


void MHZ19_init()
{
  putdatamap(cF("mhz19/co2"), "-");
  putdatamap(cF("mhz19/temperature"), "-");
  MHZ19_setabs(true);
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
  if(Debug.isActive(Debug.VERBOSE)) Debug.printf(cF("Sending MHZ19 ABS enable=%d...\n"), enabled);
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

  if(Debug.isActive(Debug.VERBOSE)) Debug.printf(cF("Sending MHZ19 request packet...\n"));

  MHZ19_write(cmd, 8);
}

bool MHZ19_read(int *ppm, int *temp)
{
  static uint8_t errorcounter = 0;
  uint8_t response[9]; // for answer

  if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("Receiving MHZ19 data...\n"));

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
    if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Invalid response from MHZ_19 CO2 sensor!\n"));
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
      if (webserver.argName(i) == sF("rebootdevice"))
      {
        webserver.send(200, cF("text/html"), sF("<HTML><HEAD><meta http-equiv=\"refresh\" content=\"5;url=\\\" /></HEAD><BODY>Device Rebooting, Please Wait...</BODY></HTML>"));
        reboottimeout = uptime + 4;
        return;
      }
    }
    mqtt_ssl = 0;
    for (uint8_t i = 0; i < webserver.args(); i++)
    {
      if (webserver.argName(i) == sF("wifissid")) wifissid = webserver.arg(i);
      if (webserver.argName(i) == sF("wifipsk")) wifipsk = webserver.arg(i);
      if (webserver.argName(i) == sF("mqttserver")) mqtt_server = webserver.arg(i);
      if (webserver.argName(i) == sF("mqttusername")) mqtt_username = webserver.arg(i);
      if (webserver.argName(i) == sF("mqttpassword")) mqtt_password = webserver.arg(i);
      if (webserver.argName(i) == sF("mqtttopicprefix")) mqtt_topicprefix = webserver.arg(i);
      if (webserver.argName(i) == sF("mqttport")) mqtt_port = String(webserver.arg(i)).toInt();
      if (webserver.argName(i) == sF("mqttssl")) mqtt_ssl = 1;
      if (webserver.argName(i) == sF("webpassword"))
      {
        if (esp_password != webserver.arg(i))
        {
          esp_password = webserver.arg(i);
          ArduinoOTA.setPassword(esp_password.c_str());
          ArduinoOTA.begin();
          Debug.setPassword(esp_password);
        }
      }
      if (webserver.argName(i) == sF("hostname")) esp_hostname = webserver.arg(i);
      esp_hostname.replace("_","-"); // RFC doesn't alllow underscores.
#ifdef  ESPMQTT_WATERMETER
      if (webserver.argName(i) == sF("watermeterliter"))
      {
        if (webserver.arg(i) != getdatamap("water/liter"))
        {
          watermeter_setliters(atoi(webserver.arg(i).c_str()));
          i2cEeprom_write(watermeter_getliters());
          putdatamap(cF("water/liter"), String(watermeter_getliters()));
          putdatamap(cF("water/m3"), String(double(watermeter_getliters()) / 1000, 3));
        }
      }
#endif
#ifdef  ESPMQTT_QSWIFIDIMMERD01
      if (webserver.argName(i) == sF("qswifidimoffset")) qswifidimmer_setdimoffset((webserver.arg(i)).toInt());
#endif
#ifdef  ESPMQTT_QSWIFIDIMMERD02
      if (webserver.argName(i) == sF("qswifidimoffset0")) qswifidimmer_setdimoffset((webserver.arg(i)).toInt(), 0);
      if (webserver.argName(i) == sF("qswifidimoffset1")) qswifidimmer_setdimoffset((webserver.arg(i)).toInt(), 1);
#endif
    }
    eeprom_save_variables();

    mainstate.defaultpassword = false;

    disconnectMqtt(); // Disconnect mqtt server

    if (esp_hostname != WiFi.hostname())
    {
      WiFi.hostname(esp_hostname);
      wifichangesettingstimeout = uptime + 4;
    }

    if ((wifissid != "") && (wifipsk != "") && (esp_hostname != "") && ((wifissid != WiFi.SSID()) || (wifipsk != WiFi.psk())))
    {
      webserver.send(200, cF("text/html"), sF("<HTML><BODY>Settings Saved.<BR>Please wait a moment and connect to proper wifi network and open the page of the saved hostname.</BODY></HTML>"));
      flashbuttonstatus = 0;
      previouswifistatus = -1;
      wifichangesettingstimeout = uptime + 4;
      return;
    }
    else
    {
      wifichangesettingstimeout = uptime + 4;
      webserver.send(200, cF("text/html"), sF("<HTML><HEAD><meta http-equiv=\"refresh\" content=\"5;url=\\\" /></HEAD><BODY>Settings Saved, Please Wait...</BODY></HTML>"));
    }

    initMqtt();
    connectToMqtt();
    update_systeminfo(true);
  }
  else
  {
    String wifiselectoptions = sF("<option value=\"") + WiFi.SSID() + sF("\" selected>") + WiFi.SSID() + sF("</option>");
    uint8_t n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i) {
      if (WiFi.SSID(i) != WiFi.SSID()) wifiselectoptions += "<option value=\"" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</option>";
    }


    webserver.setContentLength(CONTENT_LENGTH_UNKNOWN);
    webserver.send (200, "text/html", sF("<HTML><HEAD><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"></HEAD><BODY><CENTER><div align=\"left\" style=\"width:400px; margin:auto\"><CENTER><H1>"));
    webserver.sendContent (WiFi.hostname() + sF("</H1>"));
    webserver.sendContent (sF("</CENTER><form action=\"/settings\" method=\"post\" autocomplete=\"off\"><TABLE style=\"width:400px; margin:auto\"><TR><TD>Hostname</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\""));
    webserver.sendContent (String(EEPROMSTRINGSIZE - 2));
    webserver.sendContent (sF("\" name=\"hostname\" value=\""));
    webserver.sendContent (WiFi.hostname());
    webserver.sendContent (sF("\"></TD></TR><TR><TD>Wifi SSID</TD><TD><select style=\"width:200\" name=\"wifissid\">"));
    webserver.sendContent (wifiselectoptions);
    webserver.sendContent (sF("</select></TD></TR><TR><TD>wifi Key</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"64\" name=\"wifipsk\" value=\""));
    webserver.sendContent (String(WiFi.psk()));
    webserver.sendContent (sF("\"></TD></TR><TR><TD>MQTT Server</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"20\" name=\"mqttserver\" value=\""));
    webserver.sendContent (mqtt_server);
    webserver.sendContent (sF("\"></TD></TR><TR><TD>MQTT Port</TD><TD><input style=\"width:200\" type=\"number\" maxlength=\"5\" name=\"mqttport\" value=\""));
    webserver.sendContent (String(mqtt_port));
    webserver.sendContent (sF("\"></TD></TR><TR><TD>MQTT Ssl</TD><TD ALIGN=\"left\"><input type=\"checkbox\" name=\"mqttssl\" "));
    webserver.sendContent (String(mqtt_ssl ? "checked" : "") + sF("></TD></TR>"));
    webserver.sendContent (sF("<TR><TD>MQTT Username</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"20\" name=\"mqttusername\" value=\""));
    webserver.sendContent (mqtt_username);
    webserver.sendContent (sF("\"></TD></TR><TR><TD>MQTT Password</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"20\" name=\"mqttpassword\" value=\""));
    webserver.sendContent (mqtt_password + "\">");
    webserver.sendContent (sF("</TD></TR><TR><TD>MQTT Topic Prefix</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"50\" name=\"mqtttopicprefix\" value=\""));
    webserver.sendContent (mqtt_topicprefix);
    webserver.sendContent (sF("\"></TD></TR><TR><TD>ESP Password</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"20\" name=\"webpassword\" value=\""));
    webserver.sendContent (esp_password);
    webserver.sendContent (sF("\"></TD></TR>"));
#ifdef  ESPMQTT_WATERMETER
    webserver.sendContent (sF("<TR><TD>Watermeter Liter</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"64\" name=\"watermeterliter\" value=\""));
    webserver.sendContent (getdatamap("water/liter"));
    webserver.sendContent (sF("\"></TD></TR>"));
#endif
#ifdef  ESPMQTT_QSWIFIDIMMERD01
    webserver.sendContent (sF("<TR><TD>Dimmer Offset</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"3\" name=\"qswifidimoffset\" value=\""));
    webserver.sendContent (getdatamap("dimoffset"));
    webserver.sendContent (sF("\"></TD></TR>"));
#endif
#ifdef  ESPMQTT_QSWIFIDIMMERD02
    webserver.sendContent (sF("<TR><TD>Dimmer Offset 0</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"3\" name=\"qswifidimoffset0\" value=\""));
    webserver.sendContent (getdatamap("dimoffset/0"));
    webserver.sendContent (sF("\"></TD></TR>"));
    webserver.sendContent (sF("<TR><TD>Dimmer Offset 1</TD><TD><input style=\"width:200\" type=\"text\" maxlength=\"3\" name=\"qswifidimoffset1\" value=\""));
    webserver.sendContent (getdatamap("dimoffset/1"));
    webserver.sendContent (sF("\"></TD></TR>"));
#endif
    webserver.sendContent (sF("</TABLE><BR><CENTER><input type=\"submit\" value=\"Save Settings\"></form><BR><BR><form action=\"/settings\" method=\"post\" autocomplete=\"off\"><input type=\"hidden\" name=\"rebootdevice\" value=\"1\"><input type=\"submit\" value=\"Reboot Device\"></form><BR><BR><A HREF=\"/\">Return to main page</A></CENTER></div></BODY></HTML>"));
    webserver.sendContent (""); // end chunked data
  }
}

void handleJsonData() {
  webserver.setContentLength(CONTENT_LENGTH_UNKNOWN);
  webserver.send(200, "text/json", "{");
  for (int i = 0; i < dataMap->size(); i++)
  {
    webserver.sendContent ("\"" + String(dataMap->getKey(i)) + "\":\"" + dataMap->getData(i).payload);
    if (i < dataMap->size() - 1) webserver.sendContent("\",");
  }
  webserver.sendContent("\"}");
  webserver.sendContent("");
}

void handleWWWRoot() {
  static const char webpage_P[] PROGMEM = "<!DOCTYPE html><html><meta charset=\"UTF-8\"><meta name=\"google\" content=\"notranslate\"><meta http-equiv=\"Content-Language\" content=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><style>table{width: 400px; margin: auto;}</style></head><body><CENTER><div align='center' style='width:400px; margin:auto'><CENTER><H1><p id='header'></p></H1></CENTER><p id='table'></p><A HREF='settings'>Settings</A></div></CENTER><script>function refreshsite(){var obj,dbParam,xmlhttp,myObj,x,txt ='';xmlhttp=new XMLHttpRequest();xmlhttp.onreadystatechange=function(){if(this.readyState==4&&this.status==200){myObj=JSON.parse(this.responseText);txt+='<TABLE>';for (x in myObj){if(x=='hostname')document.getElementById('header').innerHTML=myObj[x].toUpperCase();txt+='<tr><td>'+x.split('/').join(' ')+'</td><td>'+myObj[x]+'</td></tr>';}txt+='</table>';document.getElementById('table').innerHTML = txt;}};xmlhttp.open('GET','data.json',true);xmlhttp.setRequestHeader('Content-type','application/x-www-form-urlencoded');xmlhttp.send();}refreshsite();window.setInterval(refreshsite, 5000);</script></body></html>";
  if (Debug.isActive(Debug.INFO)) Debug.printf(cF("New webclient connected...\n"));
  webserver.send_P(200, "text/html", webpage_P);
}


#if (QSWIFIDIMMERCHANNELS == 1)
void qswifidimmer_callback (uint8_t , uint8_t dimvalue, bool dimstate)
{
  putdatamap(cF("dimvalue"), String(dimvalue));
  putdatamap(cF("dimstate"), String(dimstate));
}
#endif
#if (QSWIFIDIMMERCHANNELS == 2)
void qswifidimmer_callback (uint8_t dimchannel, uint8_t dimvalue, bool dimstate)
{
  if (dimchannel == 0)
  {
    putdatamap(cF("dimvalue/0"), String(dimvalue));
    putdatamap(cF("dimstate/0"), String(dimstate));
  }
  if (dimchannel == 1)
  {
    putdatamap(cF("dimvalue/1"), String(dimvalue));
    putdatamap(cF("dimstate/1"), String(dimstate));
  }
}
#endif

#if (QSWIFIDIMMERCHANNELS == 1)
void qswifidimmer_switchcallback(uint8_t , bool switchstate)
{
  putdatamap(cF("switchstate"), String(switchstate));
}
#endif
#if (QSWIFIDIMMERCHANNELS == 2)
void qswifidimmer_switchcallback(uint8_t dimchannel, bool switchstate)
{
  if (dimchannel == 0) putdatamap(cF("switchstate/0"), String(switchstate));
  if (dimchannel == 1) putdatamap(cF("switchstate/1"), String(switchstate));
}
#endif

#if (QSWIFISWITCHCHANNELS == 1)
void qswifiswitch_relaycallback(uint8_t , bool state)
{
  putdatamap(cF("relaystate"), String(state));
}
void qswifiswitch_switchcallback(uint8_t , bool switchstate)
{
  putdatamap(cF("switchstate"), String(switchstate));
}
#endif
#if (QSWIFISWITCHCHANNELS == 2)
void qswifiswitch_relaycallback(uint8_t channel, bool state)
{
  if (channel == 0) putdatamap(cF("relaystate/0"), String(state));
  if (channel == 1) putdatamap(cF("relaystate/1"), String(state));
}
void qswifiswitch_switchcallback(uint8_t channel, bool switchstate)
{
  if (channel == 0) putdatamap(cF("switchstate/0"), String(switchstate));
  if (channel == 1) putdatamap(cF("switchstate/1"), String(switchstate));
}
#endif

void ducoboxcallback (const char *topic, String payload)
{
  putdatamap(topic, payload);
}

void amgpelletstovecallback (const char *topic, String payload)
{
  putdatamap(topic, payload);
}

void bht002callback (const char *topic, String payload)
{
  putdatamap(topic, payload);
  yield();
}

void tuyacallback (const char *topic, String payload)
{
  putdatamap(topic, payload);
  yield();
}

void openthermcallback (const char *topic, String payload)
{
  putdatamap(topic, payload);
}

#ifdef  ESPMQTT_GROWATT
void growattcallback (const char *topic, String payload)
{
  if (strcmp(topic, cF("status")) == 0)
  {
    if (payload == sF("ready")) digitalWrite(NODEMCULEDPIN, 0);
    else digitalWrite(NODEMCULEDPIN, 1);
  }
  putdatamap(topic, payload);
}
#endif

#ifdef  ESPMQTT_GOODWE
void goodwecallback (const char *topic, String payload)
{
  if (topic == sF("status"))
  {
    if (payload == sF("ready")) digitalWrite(NODEMCULEDPIN, 0);
    else digitalWrite(NODEMCULEDPIN, 1);
  }
  putdatamap(topic, payload);
}
#endif

#ifdef  ESPMQTT_GROWATT_MODBUS
void growattModbuscallback (const char *topic, String payload)
{
  if (strcmp(topic, cF("status")))
  {
    if (payload == sF("ready")) digitalWrite(NODEMCULEDPIN, 0);
    else digitalWrite(NODEMCULEDPIN, 1);
  }
  putdatamap(topic, payload);
}
#endif

#ifdef  ESPMQTT_SMARTMETER
void smartmetercallback (const char *topic, String payload)
{
  static uint32 nextupdatetime = 0;
  static bool sendupdate = 0;

  if (strcmp(topic, cF("status")) == 0)
  {
    if (payload == sF("receiving")) digitalWrite(NODEMCULEDPIN, 1);
    else digitalWrite(NODEMCULEDPIN, 0);
  }

  if ((nextupdatetime < uptime) && (strcmp(topic, cF("status")) == 0) && (payload == sF("receiving"))) // wait for start of new packet from smartmeter
  {
    sendupdate = 1;
    nextupdatetime = uptime + 8; // wait 8 seconds before accepting new values from smartmeter (some smartmeters keep sending out data)
  }

  if (sendupdate) putdatamap(topic, payload);

  if ((strcmp(topic, cF("status")) == 0) && (payload == sF("ready")) && sendupdate) // stop processing when a complete packet was pushed to datamap
  {
    sendupdate = 0;
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
  if (Debug.isActive(Debug.INFO)) Debug.printf(cF("[%s]  [%s]\n"), lastCmd.c_str(), lastArg.c_str());
  if (lastCmd == sF("help"))
  {
    Debug.printf(cF("Available Debug Commands:\n"));
    Debug.printf(cF("  showdatamap\n"));
    Debug.printf(cF("  ping [hostname]\n"));
    Debug.printf(cF("  route\n"));
    Debug.printf(cF("  mqttconnect\n"));
    Debug.printf(cF("  showmainstate\n"));
    Debug.printf(cF("  factoryreset\n"));
    Debug.printf(cF("  showeeprommap\n"));
    Debug.printf(cF("  scanwifinetworks (scan for stronger wifi network and connect to it)\n)"));
    Debug.printf(cF("  showtime\n"));
    Debug.printf(cF("  getrestartreason\n"));
    Debug.printf(cF("  wifidisconnect\n"));
  }

  if (lastCmd == sF("wifidisconnect"))
  {
    WiFi.disconnect();
    connectToWifi();
  }

  if (lastCmd == sF("getrestartreason"))
  {
    switch (ESP.getResetInfoPtr()->reason)
    {
      case REASON_DEFAULT_RST:
        Debug.printf(cF("Normal Startup\n"));
        break;
      case REASON_WDT_RST:
        Debug.printf(cF("Hardware Watchdog Reset\n"));
        break;
      case REASON_EXCEPTION_RST:
        Debug.printf(cF("Exception Reset\n"));
        break;
      case REASON_SOFT_WDT_RST:
        Debug.printf(cF("Software Watchdog Reset\n"));
        break;
      case REASON_SOFT_RESTART:
        Debug.printf(cF("Software Restart\n"));
        break;
      case REASON_DEEP_SLEEP_AWAKE:
        Debug.printf(cF("Wake up from deep sleep\n"));
        break;
      case REASON_EXT_SYS_RST:
        Debug.printf(cF("External Reset\n"));
        break;
    }
  }

  if (lastCmd == sF("showtime"))
  {
    time_t now;
    char strftime_buf[64];
    struct tm timeinfo;

    time(&now);

    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);

    Debug.printf(cF("Datetime= %s\n"), strftime_buf);
  }

  if (lastCmd == sF("scanwifinetworks"))
  {
    if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("Starting Wifi Scan...\n"));
    WiFi.scanNetworksAsync(wifiScanReady);
    yield();
  }

  if (lastCmd == sF("ping"))
  {
    if (Ping.ping(lastArg.c_str())) {
      Debug.printf(cF("Ping Success!!\n"));
    } else {
      Debug.printf(cF("Ping Error!!\n"));
    }
  }

  if (lastCmd == sF("route"))
  {
    String gw = WiFi.gatewayIP().toString();
    Debug.printf(cF("%s\n"), gw.c_str());
  }

  if (lastCmd == sF("mqttconnect"))
  {
    connectToMqtt();
  }

  if (lastCmd == sF("mqttdisconnect"))
  {
    disconnectMqtt();
  }

  if (lastCmd == sF("showmainstate"))
  {
    Debug.printf(cF("wificonnected=%d\n"), mainstate.wificonnected);
    Debug.printf(cF("mqttconnected=%d\n"), mainstate.mqttconnected);
    Debug.printf(cF("mqttready=%d\n"), mainstate.mqttready);
    Debug.printf(cF("mqttsenddatamap=%d\n"), mainstate.mqttsenddatamap);
    Debug.printf(cF("defaultpassword=%d\n"), mainstate.defaultpassword);
    Debug.printf(cF("accesspoint=%d\n"), mainstate.accesspoint);
  }

  if (lastCmd == sF("showdatamap")) showdatamap();

  if (lastCmd == sF("factoryreset"))
  {
    WiFi.disconnect();
    eeprom_erase();
    delay(1000);
    ESP.restart();
    delay(1000);
  }

#ifdef  ESPMQTT_WATERMETER
  if (lastCmd == sF("help")) Debug.printf(cF("  watermeterreadeeprom\n  watermeterwriteeeprom\n");
  if (lastCmd == sF("watermeterreadeeprom"))
  {
    Debug.printf(cF("i2cEeprom read liters=%d\n"), i2cEeprom_read());
    watermeter_setliters(i2cEeprom_read());
  }
  if (lastCmd == sF("watermeterwriteeeprom"))
  {
    Debug.printf(cF("i2cEeprom write liters=%d\n"), watermeter_getliters());
    i2cEeprom_write(watermeter_getliters());
  }
#endif

#ifdef ESPMQTT_BHT002
  if (lastCmd == "b")
  {
    bht002_senddebug(atoi(lastArg.c_str()));
  }
#endif

  if (lastCmd == sF("showeeprommap"))
  {
  }

#ifdef ESPMQTT_OPENTHERM
  if (lastCmd == sF("resetopentherm"))
  {
    if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("Resetting opentherm...\n"));
    opentherm_reset();
  }
#endif
}



int16_t eeprom_read(String * data, uint8_t eepromindex)
{
  DEBUG_D("eeprom_read index=%d;\n", eepromindex);
  uint16_t eeprompointer = 0;
  uint16_t eepromdatastartpointer = 0;
  uint8_t index = 0;
  *data = "";
  while (eeprompointer < 512)
  {
    eepromdatastartpointer = eeprompointer;
    uint8_t datasize = EEPROM.read(eeprompointer++);
    DEBUG("datasize=%d\n", datasize);
    if (datasize == 0) return (-1);
    byte checksum = 20;
    for (int pos = 0; pos < datasize - 1; pos++)
    {
      *data += String(char(EEPROM.read(eeprompointer)));
      checksum += char(EEPROM.read(eeprompointer++));
    }
    byte eepromchecksum = EEPROM.read(eeprompointer++);
    if (index == eepromindex)
    {
      if (eepromchecksum != checksum)
      {
        DEBUG_E("Error reading eeprom index %d (wrong checksum)!", index);
        return -1;
      }
      DEBUG_D("Read from eeprom %d=%s (checksum %d=%d)\n", eeprompointer, data->c_str(), checksum, eepromchecksum);
      return eepromdatastartpointer;
    }
    *data = "";
    index++;
  }
  return -1;
}

void eeprom_erase()
{
  for (int16_t eeprompointer = 0; eeprompointer < 512; eeprompointer++)
  {
    EEPROM.write (eeprompointer, 0);
  }
}

uint16_t eeprom_write(String value, uint16_t eeprompos)
{
  DEBUG_D("eeprom_write %d,%s\n", eeprompos, value.c_str());
  uint16_t eeprompointer = eeprompos;
  uint8_t checksum = 20;
  EEPROM.write(eeprompointer++, value.length() + 1);
  for (uint16_t valueindex = 0; valueindex < value.length(); valueindex++)
  {
    char valuechar = value[valueindex];
    EEPROM.write(eeprompointer++, valuechar);
    checksum += valuechar;
  }
  EEPROM.write(eeprompointer++, checksum);
  EEPROM.write(eeprompointer + 1, 0);
  return eeprompointer;
}

void eeprom_save_variables()
{
  uint16_t pos = 0;
  pos = eeprom_write(mqtt_server, pos);
  pos = eeprom_write(mqtt_username, pos);
  pos = eeprom_write(mqtt_password, pos);
  pos = eeprom_write(esp_password, pos);
  pos = eeprom_write(esp_hostname, pos);
  pos = eeprom_write(String(mqtt_port), pos);
  pos = eeprom_write(String(mqtt_ssl), pos);
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
  if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("Reading internal EEPROM...\n"));
  eeprom_init();
    if (!eeprom_read(&mqtt_server, 0))
  {
    if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Error reading mqtt server from internal eeprom\n"));
  }
  if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("mqtt server=%s\n"), mqtt_server.c_str());

  if (!eeprom_read(&mqtt_username, 1))
  {
    if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Error reading mqtt username from internal eeprom\n"));
  }
  if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("mqtt username=%s\n"), mqtt_username.c_str());

  if (!eeprom_read(&mqtt_password, 2))
  {
    if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Error reading mqtt password from internal eeprom\n"));
  }
  else
  {
    mainstate.defaultpassword = false;
  }

  if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("mqtt password=%s\n"), mqtt_password.c_str());

  if (!eeprom_read(&esp_password, 3))
  {
    if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Error reading esp password from internal eeprom\n"));
  }
  if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("esp password=%s\n"), esp_password.c_str());

  if (!eeprom_read(&esp_hostname, 4))
  {
    if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Error reading hostname from internal eeprom\n"));
    esp_hostname = String(FIRMWARE_TARGET) + "-" + String(chipid);
  }
  esp_hostname.replace("_","-"); // RFC doesn't alllow underscores.
  if (Debug.isActive(Debug.INFO)) Debug.printf(cF("Hostname=%s\n"), esp_hostname.c_str());

  String mqttportstr = "";
  if (!eeprom_read(&mqttportstr, 5))
  {
    if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Error reading mqttport from internal eeprom\n"));
  }
  else
  {
    if ((mqttportstr != "") && (mqttportstr.toInt() > 0 ) && (mqttportstr.toInt() < 65536)) mqtt_port = mqttportstr.toInt();
  }
  if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("mqtt port=%d\n"), mqtt_port);

  String mqttsslstr = "";
  if (!eeprom_read(&mqttsslstr, 6))
  {
    if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Error reading mqtt ssl from internal eeprom\n"));
  }
  else
  {
    if (mqttsslstr != "") mqtt_ssl = mqttsslstr == "1" ? 1 : 0;
  }
  if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("mqtt ssl=%d\n"), mqtt_ssl);

  if (!eeprom_read(&mqtt_topicprefix, 7))
  {
    if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Error reading mqtt main topic from internal eeprom\n"));
    mqtt_topicprefix = "home/" + esp_hostname + "/";
  }
  if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("mqtt topicprefix=%s\n"), mqtt_topicprefix.c_str());

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

  if (!eeprom_read(&wifissid, 10))
  {
    if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Error reading wifi ssid from internal eeprom\n"));
    wifissid = WiFi.SSID();
  }
  if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("wifi ssid=%s\n"), wifissid.c_str());
  if (wifissid == "") wifissid = WiFi.SSID();

  if (!eeprom_read(&wifipsk, 11))
  {
    if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Error reading wifi key from internal eeprom\n"));
    wifipsk = WiFi.psk();
    eeprom_save_variables(); // If reading key was in error, wifi data was not stored in eeprom before, so store it.
  }
  if (wifipsk == "") wifipsk = WiFi.psk();
  if (Debug.isActive(Debug.DEBUG)) Debug.printf(cF("wifi key=%s\n"), wifipsk.c_str());
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
  if (Debug.isActive(Debug.INFO)) Debug.printf(cF("\n\nInitializing ESP8266 %s %s...\n\n"), FIRMWARE_TARGET, ESPMQTT_VERSION);

  EEPROM.begin(512);

  char buffer[25];
  snprintf(buffer, 25, cF("%08X"), ESP.getChipId());
  chipid = buffer;

  configTime(MYTZ, "nl.pool.ntp.org");
  yield();

  eeprom_load_variables();

  update_systeminfo(true);

#ifdef ESPMQTT_BHT002
  bht002_init(bht002callback);
#endif

#ifdef ESPMQTT_TUYA_2GANGDIMMERV2
  tuya_init(tuyacallback);
#endif


#ifdef SONOFFCH
  sonoff_init();
#endif



  Debug.begin(esp_hostname.c_str(), DEBUGLEVEL);
  Debug.setPassword(esp_password);
  Debug.setResetCmdEnabled(true);
  Debug.setCallBackProjectCmds(&processCmdRemoteDebug);

  //syslog.server(mqtt_server.c_str(), 514);
  //syslog.deviceHostname(esp_hostname.c_str());
  //syslog.appName(FIRMWARE_TARGET);
  //syslog.defaultPriority(LOG_KERN);

  if (Debug.isActive(Debug.INFO)) Debug.printf(cF("ESP8266 Started...\n"));
  if (Debug.isActive(Debug.INFO)) Debug.printf(cF("Hostname=%s\n"), WiFi.hostname().c_str());

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

  ArduinoOTA.setHostname(esp_hostname.c_str());
  ArduinoOTA.setPassword(esp_password.c_str());
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
  putdatamap(cF("water/lmin"), cF("0"));
  putdatamap(cF("water/m3h"), cF("0"));
  putdatamap(cF("water/liter"), String(watermeter_liters));
  putdatamap(cF("water/m3"), String(double(watermeter_liters) / 1000, 3));
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
    if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Unable to find address for onewire_dcwSupplyWaterThermometer\n"));
  }
  if (!oneWireSensors.getAddress(onewire_chReturnWaterThermometer, 1)) {
    if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Unable to find address for Device onewire_chReturnWaterThermometer\n"));
  }
#endif

#ifdef  ESPMQTT_WEATHER
  if (!oneWireSensors.getAddress(onewire_OutsideAddress, 0)) {
    if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Unable to find address for onewire_outsidetemp\n"));
  }
#endif

#ifdef  ESPMQTT_SONOFFTH
  if (!oneWireSensors.getAddress(onewire_address, 0)) {
    if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Unable to find address for onewire temperature sensor\n"));
  }
#endif

#ifdef  ESPMQTT_SONOFF_FLOORHEATING
  if (!oneWireSensors.getAddress(onewire_floorWaterAddress, 0)) {
    if(Debug.isActive(Debug.ERROR)) Debug.printf(cF("Unable to find address for onewire_floorwater\n"));
  }
#endif
#endif

  webserver.on(cF("/"), handleWWWRoot);
  webserver.on(cF("/data.json"), handleJsonData);
  webserver.on(cF("/settings"), handleWWWSettings);
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
#endif

#ifdef  ESPMQTT_OPENTHERM
  opentherm_init(openthermcallback);
#endif

#ifdef  ESPMQTT_GROWATT
  growatt_init(growattcallback, FANPIN);
#endif

#ifdef  ESPMQTT_GOODWE
  goodwe_init(goodwecallback);
#endif

#ifdef  ESPMQTT_GROWATT_MODBUS
  growattModbus_init(growattModbuscallback);
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

#ifdef ESPMQTT_WHR930
  zehnder_whr930.setup();
#endif

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
  putdatamap(cF("wifi/externalip"), datastring, true, true, false);
  client->close();
}

void onConnectExternalIpServer(void*, AsyncClient * client) {
  client->add("GET /\r\n", 16);
  client->send();
}

void updateexternalip()
{
  static  AsyncClient* client = new AsyncClient;
  client->close();
  client->onData(&handleDataExternalIpServer, client);
  client->onConnect(&onConnectExternalIpServer, client);
  client->connect("extip.jst-it.nl", 80);
}
