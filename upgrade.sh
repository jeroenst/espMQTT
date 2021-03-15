#!/bin/bash
trap "exit" INT

upgrade ()
{
echo "Upgrading $2..."

ping -c 1 $2 > /dev/null #wakeup device because sometimes it won't respond to espota

until python3 ~/Arduino/hardware/esp8266com/esp8266/tools/espota.py -d -r -i $2 -p 8266 --auth=$1 -f ./builds/v$VERSION/$3_$VERSION.bin
do
	echo "Uploading failed, trying again...."
	sleep 2
done

printf "Waiting for $2 to become back online..."
sleep 5
while ! nc -w2 -z $2 80  &> /dev/null
do
    printf "."
done
echo ""
echo "Upgrade of $2 finished."
echo ""
}

if [ "$4" == "" ]
then
	VERSION=$(head -n 1 ~/Arduino/espMQTT/version)
else
	VERSION=$4
fi

if [ "$1" == "" ] 
then
	echo "Please give password or keyword serial for serial flashing as argument..."
	echo "Or password device firmwarename..."
	exit 1
fi

if [ "$1" == "serial" ] 
then
	if [ "$2" == "" ] 
	then
		echo "Please give firmwarename as second argument..."
		exit 1
	fi
	python3 ~/Arduino/hardware/esp8266com/esp8266/tools/upload.py write_flash 0 $2
fi

if [ "$2" == "" ] 
then
	echo "Upgrading all devices..."
#	upgrade $1 esp_odb1 ESPMQTT_OBD2
	upgrade $1 esp_smartmeter ESPMQTT_SMARTMETER
	upgrade $1 esp_sdm120 ESPMQTT_SDM120
	upgrade $1 esp_growatt ESPMQTT_GROWATT
	upgrade $1 esp_bathroom ESPMQTT_BATHROOM
	upgrade $1 esp_bedroom2 ESPMQTT_BEDROOM2
	upgrade $1 esp_opentherm ESPMQTT_OPENTHERM
	upgrade $1 esp_pelletstove ESPMQTT_AMGPELLETSTOVE
	upgrade $1 sonoff_tv ESPMQTT_SONOFFPOW
	upgrade $1 sonoff_washingmachine ESPMQTT_SONOFFPOWR2
	upgrade $1 sonoff_pond ESPMQTT_GARDEN
	upgrade $1 sonoff_shelter ESPMQTT_GARDEN
	upgrade $1 sonoff_irrigation ESPMQTT_IRRIGATION
	upgrade $1 sonoff_ducobox ESPMQTT_DUCOBOX
	upgrade $1 sonoff_coffeelamp ESPMQTT_SONOFFS20
	upgrade $1 sonoff_server ESPMQTT_SONOFFPOWR2
#	upgrade $1 sonoff_bulb ESPMQTT_SONOFFBULB
	upgrade $1 sonoffs20_001 ESPMQTT_SONOFFS20
	upgrade $1 sonoff_floorheating ESPMQTT_SONOFF_FLOORHEATING
	upgrade $1 esp_watermeter ESPMQTT_WATERMETER
	upgrade $1 esp_weather ESPMQTT_WEATHER
	upgrade $1 QSWIFIDIMMER01 ESPMQTT_QSWIFIDIMMERD02
	upgrade $1 QSWIFIDIMMER02 ESPMQTT_QSWIFIDIMMERD02
	upgrade $1 QSWIFIDIMMER03 ESPMQTT_QSWIFIDIMMERD02
	upgrade $1 QSWIFIDIMMER04 ESPMQTT_QSWIFIDIMMERD01
	upgrade $1 QSWIFIDIMMER05 ESPMQTT_QSWIFIDIMMERD02
	upgrade $1 sonoff_dishwasher ESPMQTT_SONOFFPOWR2
	upgrade $1 blitzwolf_coffee ESPMQTT_BLITZWOLF
	upgrade $1 QSWIFISWITCH01 ESPMQTT_QSWIFISWITCH2C
	upgrade $1 esp_livingroom ESPMQTT_LIVINGROOM
	exit 0
fi

if [ "$3" == "" ] 
then
	echo "Please specify firmware target..."
	exit 1
fi

upgrade $1 $2 $3
exit 0
