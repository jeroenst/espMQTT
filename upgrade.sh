#!/bin/bash
trap "exit" INT

upgrade ()
{
echo "Upgrading $2..."

ping -c 1 $2 > /dev/null #wakeup device because sometimes it won't respond to espota

until python3 ~/Arduino/hardware/esp8266com/esp8266/tools/espota.py -i $2 -p 8266 --auth=$1 -f /tmp/espMQTT/$VERSION/$3_$VERSION.bin
do
	echo "Uploading failed, trying again...."
	sleep 2
done

printf "Waiting for $2 to become back online..."
sleep 10
while ! nc -w5 -z $2 80  &> /dev/null
do
    printf "."
done
echo ""
echo "Upgrade of $2 finished."
echo ""
}

VERSION=$(head -n 1 ~/Arduino/espMQTT/version)

if [ "$1" == "" ] 
then
	echo "Please give password as argument..."
	exit 1
fi

upgrade $1 esp_smartmeter ESPMQTT_SMARTMETER
upgrade $1 esp_sdm120 ESPMQTT_SDM120
upgrade $1 esp_growatt ESPMQTT_GROWATT
upgrade $1 esp_badkamer ESPMQTT_BATHROOM
upgrade $1 esp_slaapkamer2 ESPMQTT_BEDROOM2
upgrade $1 esp_opentherm ESPMQTT_OPENTHERM
upgrade $1 esp_pelletstove ESPMQTT_AMGPELLETSTOVE
upgrade $1 sonoff_tv ESPMQTT_SONOFFPOWR2
upgrade $1 sonoff_wasmachine ESPMQTT_SONOFFPOWR2
upgrade $1 esp_tuin ESPMQTT_GARDEN
upgrade $1 sonoff_luifel ESPMQTT_GARDEN
upgrade $1 esp_irrigation ESPMQTT_IRRIGATION
upgrade $1 esp_ducobox ESPMQTT_DUCOBOX
upgrade $1 sonoff_coffeelamp ESPMQTT_SONOFFS20
upgrade $1 sonoff_server ESPMQTT_SONOFFPOWR2
upgrade $1 sonoff_bulb ESPMQTT_SONOFFBULB
upgrade $1 sonoffs20_001 ESPMQTT_SONOFFS20
upgrade $1 sonoff_floorheating ESPMQTT_SONOFF_FLOORHEATING
upgrade $1 esp_watermeter ESPMQTT_WATERMETER
upgrade $1 esp_weather ESPMQTT_WEATHER
upgrade $1 QSWIFIDIMMER01 ESPMQTT_QSWIFIDIMMERD02






#python3 ~/Arduino/hardware/esp8266/esp8266/tools/espota.py -i esp_ -p 8266 --auth=$1 -f /tmp/espMQTT/$VERSION/ESPMQTT_SMARTMETER_$VERSION.bin
