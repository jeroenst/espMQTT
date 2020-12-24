Latest Release (Tagged Version): ![Build Status](https://travis-ci.com/jeroenst/espMQTT.svg?branch=master)

This software for esp8266 can read data from several sensors and send the
data to an MQTT broker.

With the defines in the beginning of the espMQTT you can specify which kind
of target you want to build.

After first flash via serial you can press the flash button until the
blue led is always on. In this mode an accesspoint will be started with
the name of the firmware. You can connect to it using wifikey "esplogin" 
and open a browser and navigate to http://192.168.4.1

For opening the settings page the first time the username is admin and the
password is esplogin.

When pressing the flashbutton until the blue led changes blink sequence the
settings password will be reset until next reboot.

For devices without a flash button, 30 seconds after boot an accesspoint
will be enabled for a period of time.

After checkout run the following command: git submodule update --init
Run this command also in the hardware/esp8266com/esp8266 directory.

Create symbolic links in the Arduino directory to espMQTT/hardware and
espMQTT/libraries (this is needed because Arduino doesn't support libraries
and hardware per sketch folder (yet))

Binaries can be found on the releases page (https://github.com/jeroenst/espMQTT/releases)

Features:
- (First) Setup through wifi access point
- Debug interface via telnet
- Read a Growatt Solar Inverter
- Read a watermeter with use of a hall sensor (needs external eeprom to store counter)
- Read a Smartmeter via P1 port
- Communicate with a boiler and thermostat via an opentherm gateway
- Control the outputs of a sonoff device
- Read a MHZ19 co2 sensor
- Read a DHT22 temperature/humidity sensor
- Read a special board for measuring power consumption of up to 14 devices (deprecated)
- Read fanspeed from a ducobox ventilation unit and control it using a sonoff 2ch
- Measure sound pressure (not finished yet)
- Measure soil humidity (not finished yet) (SHT31)
- Control Wifi Dimmer QS-WIFI-D01 (MOESHOUSE) (https://aliexpress.com/item/33010332202.html)
- Control Wifi Dimmer QS-WIFI-D02 (MOESHOUSE)  (https://aliexpress.com/item/4000218700204.html)
- Read a rainsensor (https://nl.aliexpress.com/item/1000001838878.html)
- Read a SDM120(CT) kwh meter (https://aliexpress.com/item/32686672714.html)
