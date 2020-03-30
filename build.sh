#!/bin/bash
set -e

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

build ()
{
  echo ''
  echo '###################################################################################################'
  echo BUILDING $targetname VERSION $VERSION...
  echo '###################################################################################################'
  echo ''
  
  echo '#define '$targetname > espMQTT_buildscript.h
  arduino --board $BOARD --verify --pref build.path=./builds/tmp --pref build.extra_flags=' -DESP8266 -DWEBSOCKET_DISABLED=true -DASYNC_TCP_SSL_ENABLED -DUSE_HARDWARESERIAL -DESPMQTT_BUILDSCRIPT -DESPMQTT_VERSION="'$VERSION'"' --preserve-temp-files espMQTT.ino

  if [ $? -ne 0 ]
  then 
  echo ''
  echo '###################################################################################################'
  echo BUILDING $targetname VERSION $VERSION FAILED!!!
  echo '###################################################################################################'
  echo ''
  exit $?
  fi

  echo $VERSION > ./builds/$VERSION/$targetname.version
  mv ./builds/tmp/espMQTT.ino.bin './builds/'$VERSION'/'$targetname'_'$VERSION.bin

  echo ''
  echo '###################################################################################################'
  echo BUILDING $targetname VERSION $VERSION FINISHED.
  echo '###################################################################################################'
  echo ''
}

git update-index --assume-unchanged espMQTT_buildscript.h

VERSION=$(git describe --tags | sed 's/v//')
DIFFERENCE=$(git diff | wc -w)

if [ -z "$TRAVISBUILD" ]
then
	if [ $DIFFERENCE -eq 0 ]
	then
		if [[ $VERSION == *"-"* ]]
		then
			VERSION=$(echo $VERSION | sed 's/-.*//' | sed 's/v//')
			VERSION=$(increment_version $VERSION)
			git tag v$VERSION
			git push --tags
			VERSION=$VERSION
		fi
	else
		VERSION=$VERSION-DIRTY-$(date +%s)
	fi
fi

VERSION=v$VERSION

mkdir -p ./builds/$VERSION
rm -rf ./builds/tmp
mkdir ./builds/tmp

echo $VERSION > version

declare -a TARGETS=("ESPMQTT_WEATHER" "ESPMQTT_AMGPELLETSTOVE" "ESPMQTT_BATHROOM" "ESPMQTT_BEDROOM2" "ESPMQTT_OPENTHERM" "ESPMQTT_SMARTMETER" "ESPMQTT_GROWATT" "ESPMQTT_SDM120" "ESPMQTT_WATERMETER" "ESPMQTT_DDNS" "ESPMQTT_GENERIC8266" "ESPMQTT_MAINPOWERMETER" "ESPMQTT_NOISE" "ESPMQTT_SOIL" "ESPMQTT_DIMMER" "ESPMQTT_OBD2")
BOARD=esp8266com:esp8266:nodemcuv2
for targetname in "${TARGETS[@]}"
do
	if [ "$1" == "" ]
	then
		build
	else
		if [ "$1" == "$targetname" ]
		then
			build
		fi
	fi
done

declare -a TARGETS=("ESPMQTT_DUCOBOX" "ESPMQTT_SONOFFS20" "ESPMQTT_SONOFFBULB" "ESPMQTT_SONOFFPOWR2" "ESPMQTT_GARDEN" "ESPMQTT_SONOFF_FLOORHEATING" "ESPMQTT_IRRIGATION" "ESPMQTT_BLITZWOLF" "ESPMQTT_SONOFF4CH" "ESPMQTT_SONOFFDUAL" "ESPMQTT_SONOFFS20_PRINTER" "ESPMQTT_SONOFFPOW" "ESPMQTT_QSWIFIDIMMERD01" "ESPMQTT_QSWIFIDIMMERD02")
BOARD=esp8266com:esp8266:esp8285
for targetname in "${TARGETS[@]}"
do
	if [ "$1" == "" ]
	then
		build
	else
		if [ "$1" == "$targetname" ]
		then
			build
		fi
	fi
done


git checkout -f espMQTT_buildscript.h

