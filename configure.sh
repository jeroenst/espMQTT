#!/bin/sh
git submodule update --init --recursive
git submodule foreach git reset --hard
git reset --hard
patch hardware/esp8266com/esp8266/platform.txt < hardware/esp8266com_platform.txt.patch
mv ../hardware ../hardware_espmqtt_backup
mv ../libraries ../include_espmqtt_backup
ln -s ~/Arduino/espMQTT/hardware ~/Arduino/hardware
ln -s ~/Arduino/espMQTT/libraries ~/Arduino/libraries
