/* 
 * Library used for AMG (Micronova/Duepi) pellet stoves. 
 * 
 * For schematic see: https://easyeda.com/jeroenst/duepi-pellet-stove-wifi
 * 
 */


void amgpelletstove_receivemqtt(const String& topicstring, const String& payloadstring);
void amgpelletstove_init();
void amgpelletstove_handle();
