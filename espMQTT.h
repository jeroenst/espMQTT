#ifndef _espMQTT_H
#define _espMQTT_H

#include <ESP8266WiFi.h>

#include <RemoteDebug.h>
extern RemoteDebug Debug;

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

// ERB - Force format stings and string constants into FLASH Memory
#define sF(x) String( F(x) )
         // Used as an F() when being used as the first Element
         // of a Multi-Element Expression
extern char _gFmtBuf[100+1];
         // Buffer, Used with cF() to store constants in program space (FLASH)
#define cF(x) strncpy_P(_gFmtBuf, (PGM_P)PSTR(x), sizeof(_gFmtBuf)) 
// Used with printf() for the char format string



void putdatamap(const char *topic, String value, bool sendupdate = true, bool forceupdate = false, bool publishregular = true);
void putdatamap(const char *topic, int value, bool sendupdate = true, bool forceupdate = false, bool publishregular = true);

#endif
