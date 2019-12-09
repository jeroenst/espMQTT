#!/bin/bash

increment_version ()
{
  declare -a part=( ${1//\./ } )
  declare    new
  declare -i carry=1

  for (( CNTR=${#part[@]}-1; CNTR>=0; CNTR-=1 )); do
    len=${#part[CNTR]}
    new=$((part[CNTR]+carry))
    [ ${#new} -gt $len ] && carry=1 || carry=0
    [ $CNTR -gt 0 ] && part[CNTR]=${new: -len} || part[CNTR]=${new}
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

array=( SMARTMETER AMGPELLETSTOVE )

for targetname in "${array[@]}"
do

echo ''
echo '########################################################'
echo BUILDING $targetname VERSION $VERSION...
echo '########################################################'
echo ''
arduino --board esp8266git:esp8266:nodemcuv2 --verify ~/Arduino/espMQTT/espMQTT.ino --pref build.path=/tmp/espMQTT_build/ --pref build.extra_flags='-DESPMQTT_'$targetname' -DESP8266 -DESPMQTT_VERSION="'$VERSION'"'

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

