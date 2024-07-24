#ifndef _espMQTT_H
#define _espMQTT_H


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
// #define ESPMQTT_GROWATT
// #define ESPMQTT_GROWATT_MODBUS_1
// #define ESPMQTT_GROWATT_MODBUS_2
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

#define ESPMQTT_VERSION __DATE__
#endif

#include <ESP8266WiFi.h>

#include <RemoteDebug.h>
extern RemoteDebug Debug;

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

//#define _gFmtBufSize 100+1

// ERB - Force format strings and string constants into FLASH Memory
#define sF(x) String( F(x) )
         // Used as an F() when being used as the first Element
         // of a Multi-Element Expression
//extern char _gFmtBuf[_gFmtBufSize];
         // Buffer, Used with cF() to store constants in program space (FLASH)
//#define cF(x) strncpy_P(_gFmtBuf, (PGM_P)PSTR(x), sizeof(_gFmtBuf)) 
#define cF(x)  String( F(x) ).c_str()
// Used with printf() for the char format string

void putdatamap(const String& topic, const String& value, bool sendupdate = true, bool forceupdate = false, bool publishregular = true);
void putdatamap(const char *topic, const String& value, bool sendupdate = true, bool forceupdate = false, bool publishregular = true);
void putdatamap(const char *topic, int value, bool sendupdate = true, bool forceupdate = false, bool publishregular = true);

#endif
