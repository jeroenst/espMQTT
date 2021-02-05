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

  set -x
  
  $HOME/arduino_ide/arduino-builder \
  -compile \
  -hardware $HOME/arduino_ide/hardware \
  -hardware $HOME/.arduino15/packages \
  -hardware $HOME/Arduino/hardware \
  -tools $HOME/arduino_ide/tools-builder \
  -tools $HOME/arduino_ide/hardware/tools/avr \
  -tools $HOME/.arduino15/packages \
  -built-in-libraries $HOME/arduino_ide/libraries \
  -libraries $HOME/Arduino/libraries \
  -fqbn=$FQBN \
  -ide-version=10812 \
  -build-path $HOME/Arduino/espMQTT/builds/tmp \
  -warnings=none \
  -build-cache $HOME/Arduino/espMQTT/builds/cache \
  -prefs=build.warn_data_percentage=75 \
  -prefs="build.extra_flags=-DESP8266 -DWEBSOCKET_DISABLED=true -DASYNC_TCP_SSL_ENABLED -DUSE_HARDWARESERIAL -DESPMQTT_BUILDSCRIPT -DMYTZ=TZ_Europe_Amsterdam -DESPMQTT_VERSION=\"$VERSION\" -D$targetname" \
  \"$HOME/Arduino/espMQTT/espMQTT.ino\"
  
  { set +x; } 2>/dev/null

  if [ $? -ne 0 ]
  then 
  echo ''
  echo '###################################################################################################'
  echo BUILDING $targetname VERSION $VERSION FAILED!!!
  echo '###################################################################################################'
  echo ''
  exit $?
  fi

  echo $VERSION > $HOME/Arduino/espMQTT/builds/v$VERSION/$targetname.version
  mv $HOME/Arduino/espMQTT/builds/tmp/espMQTT.ino.bin $HOME'/Arduino/espMQTT/builds/v'$VERSION'/'$targetname'_'$VERSION.bin

  echo ''
  echo '###################################################################################################'
  echo BUILDING $targetname VERSION $VERSION FINISHED.
  echo '###################################################################################################'
  echo ''
}

VERSION=$(git describe --tags | sed 's/v//')
DIFFERENCE=$(git diff | wc -w)
TARGET=$1

if [ -z "$TRAVISBUILD" ]
then
	if [ $DIFFERENCE -eq 0 ] 
	then
		if [[ $1 == "release" ]]
		then
			TARGET=$2
			# IF CURRENT VERSION CONTAINS A - (SO NOT LATEST TAG) CREATE NEW VERSION TAG
			if [[ $VERSION == *"-"* ]]
			then
				VERSION=$(echo $VERSION | sed 's/-.*//' | sed 's/v//')
				VERSION=$(increment_version $VERSION)
				git tag v$VERSION
				git push --tags
				VERSION=$VERSION
			fi
		fi
	else
		VERSION=$VERSION-DIRTY-$(date +%s)
	fi
fi

VERSION=$VERSION

mkdir -p ./builds/v$VERSION
rm -rf ./builds/tmp
mkdir ./builds/tmp
rm -rf ./builds/cache
mkdir ./builds/cache

echo $VERSION > version
FQBN=esp8266com:esp8266:nodemcuv2:xtal=80,vt=flash,exception=legacy,ssl=all,eesz=4M,led=2,ip=lm2f,dbg=Disabled,lvl=None____,wipe=none
declare -a TARGETS=("ESPMQTT_DDM18SD" "ESPMQTT_WEATHER" "ESPMQTT_AMGPELLETSTOVE" "ESPMQTT_BATHROOM" "ESPMQTT_BEDROOM2" "ESPMQTT_OPENTHERM" "ESPMQTT_SMARTMETER" "ESPMQTT_GROWATT" "ESPMQTT_SDM120" "ESPMQTT_WATERMETER" "ESPMQTT_DDNS" "ESPMQTT_GENERIC8266" "ESPMQTT_MAINPOWERMETER" "ESPMQTT_NOISE" "ESPMQTT_SOIL" "ESPMQTT_DIMMER" "ESPMQTT_OBD2")
for targetname in "${TARGETS[@]}"
do
	if [ "$TARGET" == "" ]
	then
		build
	else
		if [ "$TARGET" == "$targetname" ]
		then
			build
		fi
	fi
done

declare -a TARGETS=("ESPMQTT_BHT002" "ESPMQTT_DUCOBOX" "ESPMQTT_SONOFFS20" "ESPMQTT_SONOFFBULB" "ESPMQTT_SONOFFPOWR2" "ESPMQTT_GARDEN" "ESPMQTT_SONOFF_FLOORHEATING" "ESPMQTT_IRRIGATION" "ESPMQTT_BLITZWOLF" "ESPMQTT_SONOFF4CH" "ESPMQTT_SONOFFDUAL" "ESPMQTT_SONOFFS20_PRINTER" "ESPMQTT_SONOFFPOW" "ESPMQTT_QSWIFIDIMMERD01" "ESPMQTT_QSWIFIDIMMERD02" "ESPMQTT_ZMAI90" "ESPMQTT_SONOFFTH" "ESPMQTT_RELAY" "ESPMQTT_TUYA_2GANGDIMMERV2")
#FQBN=esp8266com:esp8266:esp8285:xtal=80,exception=legacy,ssl=all,CrystalFreq=26,eesz=1M,led=2,ip=lm2f,dbg=Disabled,lvl=None____,wipe=none
FQBN=esp8266com:esp8266:esp8285:xtal=80,vt=flash,exception=legacy,ssl=all,ResetMethod=nodemcu,CrystalFreq=26,eesz=1M,led=2,ip=lm2f,dbg=Disabled,lvl=None____,wipe=none,baud=115200
for targetname in "${TARGETS[@]}"
do
	if [ "$TARGET" == "" ]
	then
		build
	else
		if [ "$TARGET" == "$targetname" ]
		then
			build
		fi
	fi
done



