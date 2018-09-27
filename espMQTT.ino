//#define BATHROOM
//#define BEDROOM2
//#define GARDEN //ESP8285
//#define MAINPOWERMETER
//#define OPENTHERM
//#define SONOFF4CH //ESP8285
//#define DUCOBOX
//#define SONOFFDUAL
//#define OLDBATHROOM
#define SMARTMETER
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
uint16_t ducobox_minfanspeed = 0;
#include <ESP8266WiFi.h>
WiFiServer ducoserver(2233);
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
#include <SPI.h>
#define FLASHBUTTON D3
#define ESPLED D4
#define ADS_SPISPEED 1250000
#define ADS0_RDY_PIN    D0 //ADS1256 data ready
#define ADS0_CS_PIN    D1 //ADS1256 chip select
#define ADS1_RDY_PIN    D2 //ADS1256 data ready
#define ADS1_CS_PIN    D8 //ADS1256 chip select
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

SimpleMap<String, dataStruct> *dataMap = new SimpleMap<String, dataStruct>([](String &a, String &b) -> int {
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
  return dataMap->get(topic).data;
}

void showdatamap()
{
  for (int i = 0; i < dataMap->size(); i++)
  {
    DEBUG("%s=%s\n", dataMap->getKey(i).c_str(), dataMap->getData(i).data.c_str());
    yield();
  }
}

void putdatamap(String topic, String value, bool sendupdate = true, bool forcesend = false)
{
  if ((dataMap->get(topic).data != value) || forcesend)
  {
    DEBUGV ("DATAMAP %s=%s\n", topic.c_str(), value.c_str());
    if (mqttclient.connected())
    {
      dataMap->put(topic, dataStruct{value, false});
      String mqtttopic = String("home/" + WiFi.hostname() + "/" + topic);
      if (sendupdate)
      {
        mqttclient.publish(mqtttopic.c_str(), value.c_str(), true);
        DEBUG("MQTT PUBLISHING %s=%s\n", mqtttopic.c_str(), value.c_str());
      }
    }
    else dataMap->put(topic, dataStruct{value, sendupdate});
  }
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
  /*  if (String(topic) == String("home/" + WiFi.hostname() + "/setminfanspeed"))
    {
      uint16_t minfanspeed = payloadstring.toInt();
      DEBUG("RECEIVED SETMINFANSPEED %d\n", minfanspeed);
      if ((minfanspeed > 0) && (minfanspeed <= 3000)) ducobox_minfanspeed = minfanspeed;
    }*/
  if (String(topic) == String("home/" + WiFi.hostname() + "/setfan")) ducobox_setfan("mqtt", payloadstring.toInt());
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
  handle_sonoff();
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
    update_mainpowermeter();
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
void init_sonoff()
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

void handle_sonoff()
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
    String payload = dataMap->getData(i).data;
    mqttclient.publish(topic.c_str(), payload.c_str(), true);
    DEBUG("MQTT PUBLISHING %s=%s\n", topic.c_str(), payload.c_str());
    dataMap->put(dataMap->getKey(i), dataStruct{payload, false});
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

#ifdef DUCOBOX
void ducobox_setfan(String source, uint8_t value)
{
  static SimpleMap<String, uint8_t> *ducodataMap = new SimpleMap<String, uint8_t>([](String & a, String & b) -> int {
    if (a == b) return 0;      // a and b are equal
    else if (a > b) return 1;  // a is bigger than b
    else return -1;            // a is smaller than b
  });

  ducodataMap->put(source, value);

  uint8_t newvalue = 0;

  for (int i = 0; i < ducodataMap->size(); i++)
  {
    newvalue = max(newvalue, ducodataMap->getData(i));
  }

  DEBUG("ducobox_setfan newvalue=%d\n", newvalue);

  digitalWrite(sonoff_relays[0], newvalue == 1 ? 1 : 0);
  digitalWrite(sonoff_relays[1], newvalue > 1 ? 1 : 0);
}

// This function reads the ducobox fanspeed, node 2 co2 and node 2 temperature.
// It also can write the minimum fanspeed to the ducobox
// minfanspeed : this value will be written to the ducobox upon change
// return: 0 if no new data was received, 1 if new data is available
int ducobox_handle()
{
  static String serialmessage = "";
  static uint8_t ducocmd = 0;
  static unsigned long nextupdatetime = millis();
  int returnvalue = 0;
  String topic = "";
  String ducovalue = "";
  static bool firstrun = 1;

  if (firstrun)
  {
    putdatamap("1/fanspeed", "-");
    putdatamap("1/minfanspeed", "-");
    putdatamap("2/co2", "-");
    putdatamap("2/co2/retries", "-");
    putdatamap("2/temperature", "-");
    putdatamap("2/temperature/retries", "-");
    firstrun = 0;
  }

  static WiFiClient ducoclient;
  if (!ducoclient || !ducoclient.connected()) ducoclient = ducoserver.available();
  else
  {
    while (ducoclient.available())
    {
      yield();
      delay(10);
      Serial.print((char)ducoclient.read());
    }
  }

  while (Serial.available() > 0) {
    char ducochar = char(Serial.read());
    serialmessage += ducochar;
  }
  yield();

  if (ducoclient && ducoclient.connected())
  {
    for (int i = 0; i < serialmessage.length(); i++)
    {
      yield();
      char ducochar = serialmessage.charAt(i);
      ducoclient.print(ducochar);
      if (ducochar == '\r') ducoclient.print('\n');
    }
    serialmessage = "";
  }
  else
  {
    if (millis() > nextupdatetime)
    {

      nextupdatetime = millis() + 30000; // Retry after 30 seconds
      if (!ducoclient.connected())
      {
        DEBUG("Requesting Fanspeed from ducobox...\n");
        ducobox_writeserial("\r\nfanspeed\r\n");                // Request fanspeed
        ducocmd = 0;
      }
    }


    while (serialmessage.indexOf('\r') >= 0)
    {
      yield();
      int eolchar = serialmessage.indexOf('\r');
      String ducomessage = serialmessage.substring(0, eolchar);
      serialmessage = serialmessage.substring(eolchar + 1);
      DEBUG("Received from ducobox:%s\n", ducomessage.c_str());


      if (ducomessage.indexOf("  FanSpeed:") == 0)
      {
        // Read fanspeed
        ducovalue = ducomessage.substring(19);
        ducovalue = ducovalue.substring(0, ducovalue.indexOf(' '));
        DEBUG("DUCOBOX FANSPEED=%s\n", ducovalue.c_str());

        topic = "1/fanspeed";
        putdatamap(topic, ducovalue);
        returnvalue = 1;
        DEBUG("Requesting min fan speed from ducobox...\n");
        ducobox_writeserial("fanparaget\r\n");       // Request TEMPERATURE of node 2
        ducocmd  = 1;
      }

      if (ducomessage.indexOf("   2. MIN FAN SPEED") == 0)
      {
        ducovalue = ducomessage.substring(ducomessage.indexOf(":") + 2);
        ducovalue = ducovalue.substring(0, ducovalue.indexOf(" ["));
        topic = "1/minfanspeed";
        putdatamap(topic, ducovalue);
        DEBUG("DUCOBOX MINFANSPEED=%s\n", ducovalue.c_str());
      }

      if ((ducocmd == 1) && (ducomessage.indexOf("> ") == 0))
      {
        DEBUG("Requesting Temperature from node 2 from ducobox...\n");
        ducobox_writeserial("nodeparaget 2 73\r\n");       // Request TEMPERATURE of node 2
        ducocmd = 2;
      }

      if ((ducomessage.indexOf("  -->") == 0) || (ducomessage.indexOf("  Failed") == 0))
      {
        if (ducomessage.indexOf("  Failed") == 0)
        {
          ducomessage = "";
        }
        switch (ducocmd)
        {
          case 2:
            // Read node 2 temperature
            static uint8_t tempretry = 0;
            if (ducomessage != "")
            {
              ducovalue = String(float(ducomessage.substring(6).toInt()) / 10, 1);
              putdatamap("2/temperature", ducovalue);
              tempretry = 0;
            }
            else
            {
              tempretry++;
              if (tempretry == 0) tempretry--;
            }
            if (tempretry == 10) putdatamap("2/temperature", "-");
            putdatamap("2/temperature/retries", String(tempretry));
            DEBUG("DUCOBOX NODE 2 TEMPERATURE=%s\n", ducovalue.c_str());
            returnvalue = 1;

            DEBUG("Requesting CO2 from node 2 from ducobox...\n");
            ducobox_writeserial("nodeparaget 2 74\r\n");       // Request CO2 of sensor 2
            ducocmd = 3;
            break;
          case 3:
            // Read node 2 co2
            static uint8_t co2retry = 0;
            if (ducomessage != "")
            {
              ducovalue = ducomessage.substring(6);
              putdatamap("2/co2", ducovalue);
              co2retry = 0;
            }
            else
            {
              co2retry++;
              if (co2retry == 0) co2retry--;
            }
            if (co2retry == 10) putdatamap("2/co2", "-");
            putdatamap("2/co2/retries", String(co2retry));
            DEBUG("DUCOBOX NODE 2 CO2=%s\n", ducovalue.c_str());
            returnvalue = 1;

            ducocmd = 4;
            nextupdatetime = millis() + 5000; // Next update over 5 seconds
            break;
        }
      }

      if ((ducocmd == 4))// && (oldminfanspeed != setminfanspeed))
      {
        static uint16_t co2 = 0;
        static uint8_t fanspeed = 0;
        static uint8_t oldfanspeed = 255;
        if (getdatamap("2/co2") != "-")
        {
          co2 = getdatamap("2/co2").toInt();
          DEBUG ("CO2=%d\n", co2);
          switch (fanspeed)
          {
            case 0:
              if (co2 >= 1000) fanspeed = 1;
              break;
            case 1:
              if (co2 < 800) fanspeed = 0;
              if (co2 >= 1200) fanspeed = 2;
              break;
            case 2:
              if (co2 < 1000) fanspeed = 1;
              break;
          }
          if (oldfanspeed != fanspeed)
          {
            ducobox_setfan("co2", fanspeed);
            oldfanspeed = fanspeed;
          }
        }

        /* Setting fanspeed by serial command is disabled because it can break the onboard flash of the ducobox when used to many times (>10.000)
          oldminfanspeed = setminfanspeed;
          String cmd = "fanparaset 2 " + String(setminfanspeed) + "\r\n";
          ducobox_writeserial(cmd.c_str());
          nextupdatetime = millis() + 2000; // Next update over 2 seconds
        */
      }
    }
  }
  return returnvalue;
}

void ducobox_writeserial(const char *message)
{
  for (int i = 0; i < strlen(message); i++)
  {
    Serial.print (message[i]);
    yield();
    delay(10);
  }
}

#endif



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
  webpage += "</TABLE><BR><CENTER><input type=\"submit\" value=\"Save Settings\"></form><BR><BR><form action=\"/settings\" method=\"post\" autocomplete=\"off\"><input type=\"hidden\" name=\"rebootdevice\" value=\"1\"><input type=\"submit\" value=\"Reboot Device\"></form><BR><A HREF=\"/\">Return to main page</A></CENTER></div></BODY></HTML>";
  webserver.send(200, "text/html", webpage);
}

void handleJsonData() {
  String json = "{";
  for (int i = 0; i < dataMap->size(); i++)
  {
    json += "\"" + dataMap->getKey(i) + "\":\"" + dataMap->getData(i).data + "\",";
  }
  json.remove(json.length() - 1);
  json += "}";
  webserver.send(200, "text/html", json);
}

void handleWWWRoot() {
  DEBUG("New webclient connected...\n");
  webserver.send_P(200, "text/html", webpage_P);
}

#ifdef MAINPOWERMETER
void init_mainpowermeter()
{
  system_update_cpu_freq(160);
  initADS(ADS0_CS_PIN, ADS0_RDY_PIN);
  initADS(ADS1_CS_PIN, ADS1_RDY_PIN);
}

void update_mainpowermeter()
{
  //debugvoltread();
  //return;
  // Every round update a circuit channel
  static uint8_t i = 0;

  int32_t powermeter_mW = 0;
  int32_t powermeter_mVA = 0;
  int32_t powermeter_mA = 0;
  int32_t powermeter_mV = 0;

  // Read 10 samples from a circuit
  for (uint8_t y = 0; y < 10; y++)
  {
    int32_t powermeterpart_mW = 0;
    int32_t powermeterpart_mVA = 0;
    int32_t powermeterpart_mA = 0;
    int32_t powermeterpart_mV = 0;
    readADSpower(i, &powermeterpart_mA, &powermeterpart_mV, &powermeterpart_mW, &powermeterpart_mVA);
    powermeter_mW += powermeterpart_mW / 10;
    powermeter_mVA += powermeterpart_mVA / 10;
    powermeter_mA += powermeterpart_mA / 10;
    powermeter_mV += powermeterpart_mV / 10;
  }

  DEBUG("Circuit %d:%dmV, %dmA, %dmW, %dmVA\n", i + 1, powermeter_mV, powermeter_mA, powermeter_mW, powermeter_mVA);
  if (i==0) putdatamap("mainsvoltage", String(powermeter_mV / 1000));
  putdatamap("circuit/" + String((i + 1)) + "/mA", String(powermeter_mA));
  putdatamap("circuit/" + String((i + 1)) + "/W", String(powermeter_mW / 1000));
  putdatamap("circuit/" + String((i + 1)) + "/VA", String(powermeter_mVA / 1000));

  i++;
  if (i == 14) i = 0;
}

void initADS(byte cspin, byte drdypin)
{
  pinMode(cspin, OUTPUT);
  pinMode(drdypin, INPUT);

  SPI.begin();

  SPI.beginTransaction(SPISettings(ADS_SPISPEED, MSBFIRST, SPI_MODE1));
  delayMicroseconds(10);
  digitalWrite(cspin, LOW); // select ADS
  delayMicroseconds(50);

  //Reset to Power-Up Values (FEh)
  SPI.transfer(0xFE);
  delayMicroseconds(100);

  byte status_reg = 0 ;  // address (datasheet p. 30)
  byte status_data = 0x01; //status: Most Significant Bit First, Auto-Calibration Disabled, Analog Input Buffer Disabled
  //0x03; //to activate buffer
  SPI.transfer(0x50 | status_reg);
  SPI.transfer(0x00);   // 2nd command byte, write one register only
  SPI.transfer(status_data);   // write the databyte to the register
  delayMicroseconds(10);

  //PGA SETTING
  //1 ±5V        000 (1)
  //2 ±2.5V      001 (2)
  //4 ±1.25V     010 (3)
  //8 ±0.625V    011 (4)
  //16 ±312.5mV  100 (5)
  //32 ±156.25mV 101 (6)
  //64 ±78.125mV 110 (7) OR 111 (8)
  byte adcon_reg = 2; //A/D Control Register (Address 02h)
  byte adcon_data = 0x20; // 0 01 00 000 => Clock Out Frequency = fCLKIN, Sensor Detect OFF, gain 1
  //0x25 for setting gain to 32, 0x27 to 64
  SPI.transfer(0x50 | adcon_reg);
  SPI.transfer(0x00);   // 2nd command byte, write one register only
  SPI.transfer(adcon_data);   // write the databyte to the register
  delayMicroseconds(10);

  //Set sampling rate
  byte drate_reg = 3; // Choosing Data Rate register = third register.
  byte drate_data = 0b11110000; // 11110000 = 30,000SPS 11000000 = 3,750SPS
  SPI.transfer(0x50 | drate_reg);
  SPI.transfer(0x00);   // 2nd command byte, write one register only
  SPI.transfer(drate_data);   // write the databyte to the register
  delayMicroseconds(10);

  //done with settings, can close SPI transaction now
  digitalWrite(cspin, HIGH); //unselect ADS
  SPI.endTransaction();
  delayMicroseconds(50);

  DEBUG("ADS1256 configured\n");
}

void setADSch(byte cspin, byte drdypin, byte channel)
{
  SPI.beginTransaction(SPISettings(ADS_SPISPEED, MSBFIRST, SPI_MODE1));
  digitalWrite(cspin, LOW);
  delayMicroseconds(50);
  while (!digitalRead(drdypin)) {} ; // Wait for DRDY pin to become high..
  while (digitalRead(drdypin)) {} ; // Wait for DRDY pin to become low..

  // Stop Read Data Continuesly 0000  1111 (0Fh)
  SPI.transfer(0x0F);
  delayMicroseconds(50);

  byte data = (channel << 4) | 0b1000; //AIN-channel and AINCOM
  SPI.transfer(0x50 | 1); // write (0x50) MUX register (0x01)
  SPI.transfer(0x00);   // number of registers to be read/written − 1, write one register only
  SPI.transfer(data);   // write the databyte to the register
  delayMicroseconds(10);

  //SYNC command 1111 1100
  SPI.transfer(0xFC);
  delayMicroseconds(10);

  //WAKEUP 0000 0000
  SPI.transfer(0x00);
  delayMicroseconds(10);

  for (int i = 5; i--; i > 0) // Wait 5 drdy periods for analog input to settle (settling time)
  {
    while (!digitalRead(drdypin)) {} ; // Wait for DRDY pin to become high..
    while (digitalRead(drdypin)) {} ; // Wait for DRDY pin to become low..
  }

  // Read Data Continuesly 0000  0011 (03h)
  SPI.transfer(0x03);
  delayMicroseconds(50);

  //done, can close SPI transaction now
  digitalWrite(cspin, HIGH); //unselect ADS
  SPI.endTransaction();
}

long readADS(byte cspin, byte drdypin)
{
  long adc_val;

  SPI.beginTransaction(SPISettings(ADS_SPISPEED, MSBFIRST, SPI_MODE1));
  digitalWrite(cspin, LOW);

  while (!digitalRead(drdypin)) {}; // Wait for DRDY pin to become high..
  while (digitalRead(drdypin)) {} ; // Wait for DRDY pin to become low..

  adc_val = SPI.transfer(0);
  adc_val <<= 8; //shift to left
  adc_val |= SPI.transfer(0);
  adc_val <<= 8;
  adc_val |= SPI.transfer(0);

  //The ADS1255/6 output 24 bits of data in Binary Two’s
  //Complement format. The LSB has a weight of
  //2VREF/(PGA(223 − 1)). A positive full-scale input produces
  //an output code of 7FFFFFh and the negative full-scale
  //input produces an output code of 800000h.
  if (adc_val > 0x7fffff) { //if MSB == 1
    adc_val = 16777216ul - adc_val; //do 2's complement, discard sign
  }

  //done with settings, can close SPI transaction now
  digitalWrite(cspin, HIGH); //unselect ADS
  delayMicroseconds(50);
  SPI.endTransaction();

  return adc_val;
}

long readADSac(byte cspin, byte drdypin)
{
  uint64_t ac_val = 0;
  uint64_t acm_val = 0;
  long adc_val;
  int i = 0;

  SPI.beginTransaction(SPISettings(ADS_SPISPEED, MSBFIRST, SPI_MODE1));
  digitalWrite(cspin, LOW);

  for (int y = 0; y < 2; y++) // sample 2 sine waves (at 50hz)
  {
    ac_val = 0;
    cli(); // Disable interrupts
    for (i = 0; i < 600; i++)
    {
      while (!digitalRead(drdypin)) {}; // Wait for DRDY pin to become high..
      while (digitalRead(drdypin)) {} ; // Wait for DRDY pin to become low..

      adc_val = 0;
      adc_val = SPI.transfer(0);
      adc_val <<= 8; //shift to left
      adc_val |= SPI.transfer(0);
      adc_val <<= 8;
      adc_val |= SPI.transfer(0);
      ac_val += abs(adc_val - 0x3ffef0); // Center measurement
    }
    sei(); // Enable interrupts
    acm_val += ac_val / 600;
  }

  //done with settings, can close SPI transaction now
  digitalWrite(cspin, HIGH); //unselect ADS
  delayMicroseconds(50);
  SPI.endTransaction();

  return max(long(acm_val / 2) - 252, long(0)); // Filter noise
}

void debugvoltread()
{
   setADSch(ADS0_CS_PIN, ADS0_RDY_PIN, 7); // Volt
   while (!digitalRead(ADS0_RDY_PIN)) {}; // Wait for DRDY pin to become high..
   while (digitalRead(ADS0_RDY_PIN)) {} ; // Wait for DRDY pin to become low..

    uint32_t adc_val_volt = 0;
    digitalWrite(ADS0_CS_PIN, LOW);
    adc_val_volt = 0;
    adc_val_volt = SPI.transfer(0);
    adc_val_volt <<= 8; //shift to left
    adc_val_volt |= SPI.transfer(0);
    adc_val_volt <<= 8;
    adc_val_volt |= SPI.transfer(0);
    digitalWrite(ADS0_CS_PIN, HIGH);

    DEBUG("ADC READ=%d\n", adc_val_volt);
}

byte readADSpower(byte adchannel, int32 * mA, int32 * mV, int32 * mW, int32 * mVA)
{
#define NROFSAMPLES 200
  int32_t ac_mA[NROFSAMPLES]; // 200 samples is 20 ms
  int32_t ac_mA_offset = 0;
  int32_t ac_mV[NROFSAMPLES];
  int32_t ac_mV_offset = 0;
  double ac_mA_gain = 1;
  double ac_mV_gain = 1;

  int32_t adc_val_amp = 0;
  int32_t adc_val_volt = 0;

  int i = 0;
  byte adc_cs_amp_pin;
  byte adc_drdy_amp_pin;
  byte adc_cs_volt_pin;
  byte adc_drdy_volt_pin;
  // Set adc channels for measuring volt and amps
  if (adchannel < 7)
  {
    ac_mA_offset = 5; // Offset ad 0
    ac_mA_gain = 0.01168; // Gain ad 0 (calculated: 0.01168)
    adc_cs_amp_pin = ADS0_CS_PIN;
    adc_drdy_amp_pin = ADS0_RDY_PIN;
    setADSch(ADS0_CS_PIN, ADS0_RDY_PIN, adchannel); // Amps


    ac_mV_offset = 0; // Offset ad 1
    ac_mV_gain = 0.092; // Gain ad 1
    adc_cs_volt_pin = ADS1_CS_PIN;
    adc_drdy_volt_pin = ADS1_RDY_PIN;
    setADSch(ADS1_CS_PIN, ADS1_RDY_PIN, 7); // Volt
  }
  else if (adchannel < 14)
  {
    ac_mA_offset = 5; // Offset ad 1
    ac_mA_gain = 0.0145; // Gain ad 1
    adc_cs_amp_pin = ADS1_CS_PIN;
    adc_drdy_amp_pin = ADS1_RDY_PIN;
    setADSch(ADS1_CS_PIN, ADS1_RDY_PIN, adchannel - 7); // Amps

    ac_mV_offset = 0; // Offset ad 0 mV
    ac_mV_gain = 0.092; // Gain ad 0
    adc_cs_volt_pin = ADS0_CS_PIN;
    adc_drdy_volt_pin = ADS0_RDY_PIN;
    setADSch(ADS0_CS_PIN, ADS0_RDY_PIN, 7); // Volt
  }
  else return -1;

  //DEBUG ("Reading AD channel %d\n",adchannel);
  SPI.beginTransaction(SPISettings(ADS_SPISPEED, MSBFIRST, SPI_MODE1));

  cli(); // Disable interrupts
  for (i = 0; i < NROFSAMPLES; i++)
  {
    while (!digitalRead(adc_drdy_amp_pin)) {}; // Wait for DRDY pin to become high..
    while (digitalRead(adc_drdy_amp_pin)) {} ; // Wait for DRDY pin to become low..

    digitalWrite(adc_cs_amp_pin, LOW);
    adc_val_amp = 0;
    adc_val_amp = SPI.transfer(0);
    adc_val_amp <<= 8; //shift to left
    adc_val_amp |= SPI.transfer(0);
    adc_val_amp <<= 8;
    adc_val_amp |= SPI.transfer(0);
    digitalWrite(adc_cs_amp_pin, HIGH);

    ac_mA[i] = adc_val_amp;

    while (!digitalRead(adc_drdy_volt_pin)) {}; // Wait for DRDY pin to become high..
    while (digitalRead(adc_drdy_volt_pin)) {} ; // Wait for DRDY pin to become low..

    digitalWrite(adc_cs_volt_pin, LOW);
    adc_val_volt = 0;
    adc_val_volt = SPI.transfer(0);
    adc_val_volt <<= 8; //shift to left
    adc_val_volt |= SPI.transfer(0);
    adc_val_volt <<= 8;
    adc_val_volt |= SPI.transfer(0);
    digitalWrite(adc_cs_volt_pin, HIGH);

    ac_mV[i] = adc_val_volt;
  }
  sei(); // Enable interrupts

  //done with settings, can close SPI transaction now
  delayMicroseconds(50);
  SPI.endTransaction();

  int64_t sum_mA = 0;
  int64_t sum_mV = 0;
  int64_t sum_mW = 0;
  for (i = 0; i < NROFSAMPLES; i++)
  {
    double ac_mA_calc = 0;
    ac_mA_calc = (ac_mA[i] - 0x400000); // Center measurement and calculate mains current in mA
    ac_mA_calc += ac_mA_offset;
    ac_mA_calc *= ac_mA_gain;
    sum_mA += abs(ac_mA_calc);

    double ac_mV_calc = 0;
    ac_mV_calc = (ac_mV[i] - 0x400000);
    ac_mV_calc += ac_mV_offset;
    ac_mV_calc *= ac_mV_gain;
    sum_mV += abs(ac_mV_calc);

    ac_mV_calc = 230000; // Voltage reading is not working :-(
    double mW_calc = ac_mA_calc * ac_mV_calc;
    sum_mW += mW_calc;
    //ac_val_watt += ((adc_val_amp - 0x3ffef0) * (adc_val_volt - 0x3ffef0))/10000; // Calculate milliwatts
    //DEBUG ("%.3f,%.3f\n", ac_mA[i], ac_volt[i]);
    //DEBUG ("%d\n", ac_mA[i]);
    yield();
  }

  *mA = sum_mA / NROFSAMPLES;
  *mV = sum_mV / NROFSAMPLES;
  *mW = (sum_mW / NROFSAMPLES) / 1000;
  *mVA = (*mA * *mV) / 1000;
}


/*

//Maximum value of ADS
#define ADC_COUNTS 32768
#define PHASECAL 1.7
#define VCAL 0.6
#define ICAL 0.003

double filteredI;
double lastFilteredV, filteredV; //Filtered_ is the raw analog value minus the DC offset
int sampleV;                    //sample_ holds the raw analog read value
int sampleI;

double offsetV;                          //Low-pass filter output
double offsetI;                          //Low-pass filter output

double realPower,
       apparentPower,
       powerFactor,
       Vrms,
       Irms;
double phaseShiftedV; //Holds the calibrated phase shifted voltage.
int startV; //Instantaneous voltage at start of sample window.
double sqV, sumV, sqI, sumI, instP, sumP; //sq = squared, sum = Sum, inst = instantaneous
boolean lastVCross, checkVCross; //Used to measure number of times threshold is crossed.

double squareRoot(double fg)
{
  double n = fg / 2.0;
  double lstX = 0.0;
  while (n != lstX)
  {
    lstX = n;
    n = (n + fg / n) / 2.0;
  }
  return n;
}


void calcVI(unsigned int crossings, unsigned int timeout)
{

  unsigned int crossCount = 0;                             //Used to measure number of times threshold is crossed.
  unsigned int numberOfSamples = 0;                        //This is now incremented

  //-------------------------------------------------------------------------------------------------------------------------
  // 1) Waits for the waveform to be close to 'zero' (mid-scale adc) part in sin curve.
  //-------------------------------------------------------------------------------------------------------------------------
  boolean st = false;                                //an indicator to exit the while loop

  unsigned long start = millis();    //millis()-start makes sure it doesnt get stuck in the loop if there is an error.

  while (st == false)                                //the while loop...
  {
    //     startV = ads0.readADC_SingleEnded(0);                    //using the voltage waveform
    if ((abs(startV) < (ADC_COUNTS * 0.55)) && (abs(startV) > (ADC_COUNTS * 0.45))) st = true; //check its within range
    if ((millis() - start) > timeout) st = true;
  }

  //-------------------------------------------------------------------------------------------------------------------------
  // 2) Main measurement loop
  //-------------------------------------------------------------------------------------------------------------------------
  start = millis();

  while ((crossCount < crossings) && ((millis() - start) < timeout))
  {
    numberOfSamples++;                       //Count number of times looped.
    lastFilteredV = filteredV;               //Used for delay/phase compensation

    //-----------------------------------------------------------------------------
    // A) Read in raw voltage and current samples
    //-----------------------------------------------------------------------------
    //    sampleV = ads0.readADC_SingleEnded(0);                 //Read in raw voltage signal
    //   sampleI = ads0.readADC_SingleEnded(1);                 //Read in raw current signal

    //-----------------------------------------------------------------------------
    // B) Apply digital low pass filters to extract the 2.5 V or 1.65 V dc offset,
    //     then subtract this - signal is now centred on 0 counts.
    //-----------------------------------------------------------------------------
    offsetV = offsetV + ((sampleV - offsetV) / 1024);
    filteredV = sampleV - offsetV;
    offsetI = offsetI + ((sampleI - offsetI) / 1024);
    filteredI = sampleI - offsetI;

    //-----------------------------------------------------------------------------
    // C) Root-mean-square method voltage
    //-----------------------------------------------------------------------------
    sqV = filteredV * filteredV;                //1) square voltage values
    sumV += sqV;                                //2) sum

    //-----------------------------------------------------------------------------
    // D) Root-mean-square method current
    //-----------------------------------------------------------------------------
    sqI = filteredI * filteredI;                //1) square current values
    sumI += sqI;                                //2) sum

    //-----------------------------------------------------------------------------
    // E) Phase calibration
    //-----------------------------------------------------------------------------
    phaseShiftedV = lastFilteredV + PHASECAL * (filteredV - lastFilteredV);

    //-----------------------------------------------------------------------------
    // F) Instantaneous power calc
    //-----------------------------------------------------------------------------
    instP = phaseShiftedV * filteredI;          //Instantaneous Power
    sumP += instP;                              //Sum

    //-----------------------------------------------------------------------------
    // G) Find the number of times the voltage has crossed the initial voltage
    //    - every 2 crosses we will have sampled 1 wavelength
    //    - so this method allows us to sample an integer number of half wavelengths which increases accuracy
    //-----------------------------------------------------------------------------
    lastVCross = checkVCross;
    if (sampleV > startV) checkVCross = true;
    else checkVCross = false;
    if (numberOfSamples == 1) lastVCross = checkVCross;

    if (lastVCross != checkVCross) crossCount++;
  }

  DEBUG((String("Nr of samples:") + numberOfSamples + "\n").c_str());
  DEBUG((String("Nr of crosses:") + crossCount + "\n").c_str());

  //-------------------------------------------------------------------------------------------------------------------------
  // 3) Post loop calculations
  //-------------------------------------------------------------------------------------------------------------------------
  //Calculation of the root of the mean of the voltage and current squared (rms)
  //Calibration coefficients applied.
  float multiplier = 0.125F; // ADS1115 @ +/- 4.096V gain (16-bit results) 
  double V_RATIO = VCAL * multiplier;
  Vrms = V_RATIO * squareRoot(sumV / numberOfSamples);

  double I_RATIO = ICAL * multiplier;
  Irms = I_RATIO * squareRoot(sumI / numberOfSamples);

  //Calculation power values
  realPower = V_RATIO * I_RATIO * sumP / numberOfSamples;
  apparentPower = Vrms * Irms;
  powerFactor = realPower / apparentPower;

  //Reset accumulators
  sumV = 0;
  sumI = 0;
  sumP = 0;

  DEBUG((String("Vrms=") + Vrms + "\n").c_str());
  DEBUG((String("Irms=") + Irms + "\n").c_str());
  DEBUG((String("realPower (W)=") + realPower + "\n").c_str());
  DEBUG((String("apparentPower (VA)=") + apparentPower).c_str());
  DEBUG((String("powerFactor=") + powerFactor + "\n").c_str());
  //--------------------------------------------------------------------------------------
}

double calcIrms(unsigned int Number_of_Samples)
{
  // Be sure to update this value based on the IC and the gain settings!
  float multiplier = 0.125F;    // ADS1115 @ +/- 4.096V gain (16-bit results) 
  for (unsigned int n = 0; n < Number_of_Samples; n++)
  {
    //sampleI = ads0.readADC_Differential_0_1();

    // Digital low pass filter extracts the 2.5 V or 1.65 V dc offset,
    //  then subtract this - signal is now centered on 0 counts.
    offsetI = (offsetI + (sampleI - offsetI) / 1024);
    filteredI = sampleI - offsetI;
    //filteredI = sampleI * multiplier;

    // Root-mean-square method current
    // 1) square current values
    sqI = filteredI * filteredI;
    // 2) sum
    sumI += sqI;
  }

  Irms = squareRoot(sumI / Number_of_Samples) * multiplier;

  //Reset accumulators
  sumI = 0;
  //--------------------------------------------------------------------------------------

  return Irms;
}
*/
#endif


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
  init_sonoff();
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
  init_mainpowermeter();
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

#ifdef DUCOBOX
  ducoserver.begin();
  Serial.print("\r\n");
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
