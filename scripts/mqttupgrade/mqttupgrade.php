<?php

require("phpMQTT/phpMQTT.php");
require("mqttupgradesettings.php");

$mqtt = new phpMQTT($server, $port, $client_id);
if ($mqtt->connect(true, NULL, $username, $password)) {

$topics = array();
$topics['NL5527HM35/ESP_SMARTMETER/firmware/upgradekey'] = array("qos" => 0, "function" => 'upgrade_device');
$topics['NL5527HM35/SONOFF_GARAGE/firmware/upgradekey'] = array("qos" => 0, "function" => 'upgrade_device');
$topics['home/GENERIC8266_00068F98/firmware/upgradekey'] = array("qos" => 0, "function" => 'upgrade_device');
$topics['home/ESP_SMARTMETER/firmware/upgradekey'] = array("qos" => 0, "function" => 'upgrade_device');
$topics['home/ESP_WATERMETER/firmware/upgradekey'] = array("qos" => 0, "function" => 'upgrade_device');
$topics['home/ESP_GROWATT/firmware/upgradekey'] = array("qos" => 0, "function" => 'upgrade_device');
$topics['home/ESP_WEATHER/firmware/upgradekey'] = array("qos" => 0, "function" => 'upgrade_device');

$mqtt->subscribe($topics, 0);

while($mqtt->proc()){
                
}


$mqtt->close();
}


function upgrade_device($topic, $msg)
{
     echo "$topic=$msg\n";
    $topicarray = explode("/", $topic);
    $maintopic = $topicarray[0] . "/" . $topicarray[1];
    $devicetype = "";
    switch ($topicarray[1])
    {
        case "ESP_SMARTMETER":
            $devicetype = "ESPMQTT_SMARTMETER";
        break;

        case "ESP_WATERMETER":
            $devicetype = "ESPMQTT_WATERMETER";
        break;

        case "ESP_GROWATT":
            $devicetype = "ESPMQTT_GROWATT";
        break;

        case "ESP_WEATHER":
            $devicetype = "ESPMQTT_WEATHER";
        break;

        case "SONOFF_GARAGE":
            $devicetype = "ESPMQTT_SONOFF4CH";
        break;

        case "GENERIC8266_00068F98":
            $devicetype = "ESPMQTT_GENERIC8266";
        break;
    }
    if ($devicetype != "") upgrade($devicetype, $maintopic, $msg);
}

function upgrade ($devicetype, $maintopic, $upgradekey)
{
    global $firmwarepath;
    global $version;
    global $mqtt;
    $url = $firmwarepath.'v'.$version.'/'.$devicetype.'_'.$version.'.bin';
    echo "Upgrading: ".$maintopic." to ".$url."\n";
    $mqtt->publish($maintopic."/startfirmwareupgrade", '{"version":"'.$version.'", "url":"'.$url.'", "key":"'.$upgradekey.'"}', 0);
}
