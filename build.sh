#!/bin/bash
VERSION=1.0.1

mkdir /tmp/espMQTT
rm -rf /tmp/espMQTT_build

mkdir /tmp/espMQTT_build
arduino -v --board esp8266git:esp8266:nodemcuv2 --verify /home/jeroen/Arduino/espMQTT/espMQTT.ino --pref build.path=/tmp/espMQTT_build/ --pref build.extra_flags='-DESPMQTT_SMARTMETER -DESP8266 -DESPMQTT_VERSION='$VERSION
mv /tmp/espMQTT_build/espMQTT.ino.bin /tmp/espMQTT/smartmeter_$VERSION.bin

mkdir /tmp/espMQTT_build
arduino -v --board esp8266git:esp8266:nodemcuv2 --verify /home/jeroen/Arduino/espMQTT/espMQTT.ino --pref build.path=/tmp/espMQTT_build/ --pref build.extra_flags='-DESPMQTT_AMGPELLETSTOVE -DESP8266 -DESPMQTT_VERSION='$VERSION
mv /tmp/espMQTT_build/espMQTT.ino.bin /tmp/espMQTT/amgpelletstove_$VERSION.bin

