<?php

require("phpMQTT/phpMQTT.php");
require("mqttupgradesettings.php");

$mqtt = new phpMQTT($server, $port, $client_id);
if ($mqtt->connect(true, NULL, $username, $password)) {
echo ("Connected to MQTT server\n");
$devicetopics = array();

$devicetopics[] = "home/SONOFF_WASHINGMACHINE";
$devicetopics[] = "home/SONOFF_SERVER";
$devicetopics[] = "home/SONOFF_POND";
$devicetopics[] = "home/SONOFF_SHELTER";
$devicetopics[] = "home/SONOFF_IRRIGATION";
$devicetopics[] = "home/SONOFF_DUCOBOX";
$devicetopics[] = "home/SONOFF_COFFEELAMP";
$devicetopics[] = "home/SONOFF_FLOORHEATING";
$devicetopics[] = "home/SONOFFS20_001";
$devicetopics[] = "home/ESP_WEATHER";
$devicetopics[] = "home/ESP_SDM120";
$devicetopics[] = "home/ESP_PELLETSTOVE";
$devicetopics[] = "home/BLITZWOLF_COFFEE";
$devicetopics[] = "home/ESP_BEDROOM2";
$devicetopics[] = "home/ESP_OPENTHERM";
$devicetopics[] = "home/ESP_BATHROOM";
$devicetopics[] = "home/ESP_WATERMETER";
$devicetopics[] = "home/SONOFF_DISHWASHER";
$devicetopics[] = "home/SONOFF_TV";
$devicetopics[] = "home/QSWIFIDIMMER01";
$devicetopics[] = "home/QSWIFIDIMMER02";
$devicetopics[] = "home/QSWIFIDIMMER03";
$devicetopics[] = "home/QSWIFIDIMMER04";
$devicetopics[] = "home/QSWIFIDIMMER05";

$devicetopics[] = "0002/ESP_SMARTMETER";
$devicetopics[] = "0002/RELAY-CV";
$devicetopics[] = "0002/BHT002-LIVINGROOM";
$devicetopics[] = "0002/SONOFF_PONDTEMP";

$devices = array();

$topics = array();

foreach ($devicetopics as $devicetopic)
{
    $topics[$devicetopic.'/firmware/upgradekey'] = array("qos" => 0, "function" => 'add_device_upgradekey');
    $topics[$devicetopic.'/firmware/target'] = array("qos" => 0, "function" => 'add_device_target');
}

$mqtt->subscribe($topics, 0);

while($mqtt->proc()){
                
}


$mqtt->close();
}


function add_device_upgradekey($topic, $msg)
{
    global $devices;
//    echo ("add_device_upgradekey $msg\n");
    $topicarray = explode("/", $topic);
    $maintopic = $topicarray[0] . "/" . $topicarray[1];
    $devices[$maintopic]["upgradekey"] = $msg;
    if (isset($devices[$maintopic]["devicetype"]))
    {
        upgrade($maintopic, $devices[$maintopic]["upgradekey"], $devices[$maintopic]["devicetype"]);
    }
}

function add_device_target($topic, $msg)
{
    global $devices;
//    echo ("add_device_target $msg\n");
    $topicarray = explode("/", $topic);
    $maintopic = $topicarray[0] . "/" . $topicarray[1];
    $devices[$maintopic]["devicetype"] = $msg;
    if (isset($devices[$maintopic]["upgradekey"]))
    {
        upgrade($maintopic, $devices[$maintopic]["upgradekey"], $devices[$maintopic]["devicetype"]);
    }
}

function upgrade ($maintopic, $upgradekey, $devicetype)
{
    global $firmwarepath;
    global $version;
    global $mqtt;
    $url = $firmwarepath.'/v'.$version.'/ESPMQTT_'.$devicetype.'_'.$version.'.bin';
    echo "Upgrading: ".$maintopic." to ".$url."\n";
    $mqtt->publish($maintopic."/startfirmwareupgrade", '{"version":"'.$version.'", "url":"'.$url.'", "key":"'.$upgradekey.'"}', 0);
}
