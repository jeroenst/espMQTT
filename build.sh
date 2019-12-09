#!/bin/bash

increment_version ()
{
  declare -a part=( ${1//\./ } )
  declare    new
  declare -i carry=1

  for (( CNTR=${#part[@]}-1; CNTR>=0; CNTR-=1 )); do
    len=${#part[CNTR]}
    new=$((part[CNTR]+carry))
    part[CNTR]=${new}
#    [ ${#new} -gt $len ] && carry=1 || carry=0
#    [ $CNTR -gt 0 ] && part[CNTR]=${new: -len} || part[CNTR]=${new}
    break;
  done
  new="${part[*]}"
  new="${new// /.}"
  echo $new
} 

VERSION=$(head -n 1 ~/Arduino/espMQTT/version)

mkdir /tmp/espMQTT
rm -rf /tmp/espMQTT_build
mkdir /tmp/espMQTT_build

VERSION=$(increment_version $VERSION)
echo $VERSION > ~/Arduino/espMQTT/version

declare -a TARGETS8266=("ESPMQTT_WEATHER" "ESPMQTT_AMGPELLETSTOVE" "ESPMQTT_BATHROOM" "ESPMQTT_BEDROOM2" "ESPMQTT_OPENTHERM" "ESPMQTT_SMARTMETER" "ESPMQTT_GROWATT" "ESPMQTT_SDM120" "ESPMQTT_WATERMETER" "ESPMQTT_DDNS" "ESPMQTT_GENERIC8266" "ESPMQTT_MAINPOWERMETER" "ESPMQTT_NOISE" "ESPMQTT_SOIL")

for targetname in "${TARGETS8266[@]}"
do

echo ''
echo '########################################################'
echo BUILDING $targetname VERSION $VERSION...
echo '########################################################'
echo ''
arduino --board esp8266git:esp8266:nodemcuv2 --verify ~/Arduino/espMQTT/espMQTT.ino --pref build.path=/tmp/espMQTT_build/ --pref build.extra_flags='-D'$targetname' -DESP8266 -DESPMQTT_BUILDSCRIPT -DESPMQTT_VERSION="'$VERSION'"'

if [ $? -ne 0 ]
then 
echo ''
echo '########################################################'
echo BUILDING $targetname VERSION $VERSION FAILED!!!
echo '########################################################'
echo ''
exit $?
fi

echo $VERSION > /tmp/espMQTT/$targetname.version
mv /tmp/espMQTT_build/espMQTT.ino.bin '/tmp/espMQTT/'$targetname'_'$VERSION.bin

echo ''
echo '########################################################'
echo BUILDING $targetname VERSION $VERSION FINISHED.
echo '########################################################'
echo ''

done

rm -rf /tmp/espMQTT_build

