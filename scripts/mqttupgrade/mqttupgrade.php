<?php

require("phpMQTT/phpMQTT.php");


$devicetopics = array();

require("mqttupgradesettings.php");

$mqtt = new phpMQTT($server, $port, $client_id);
if ($mqtt->connect(true, NULL, $username, $password)) {
echo ("Connected to MQTT server\n");

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
    echo "Upgrading: ".$maintopic." with upgradekey ".$upgradekey." to ".$url."\n";
    $mqtt->publish($maintopic."/startfirmwareupgrade", '{"version":"'.$version.'", "url":"'.$url.'", "key":"'.$upgradekey.'"}', 0);
}
