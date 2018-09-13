This software for esp8266 can read data from several sensors and send the
data to an MQTT broker.

With the defines in the beginning of the espMQTT you can specify which kind
of target you want to build.

After first flash via serial you can press the flash button until the
blue led is always on. In this mode an open accesspoint will be started. You
can connect to it and open a browser and navigate to http://192.168.4.1

For opening the settings page the first time the username is admin and the
password is blank.

When pressing the flashbutton until the blue led changes blink sequence the
settings password will be reset until next reboot.

Features:
- (First) Setup through wifi access point
- Debug interface via telnet
- Read a Growatt Solar Inverter
- Read a watermeter with use of a hall sensor (needs external eeprom to store counter)
- Control the outputs of a sonoff device
- Read a MHZ19 co2 sensor
- Read a DHT22 temperature/humidity sensor
- Read a special board for measuring power consumption of up to 14 devices

