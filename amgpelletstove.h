/* 
 * Library used for AMG (Micronova/Duepi) pellet stoves. 
 * 
 * For schematic see: https://easyeda.com/jeroenst/duepi-pellet-stove-wifi
 * 
 */


void amgpelletstove_receivemqtt(String topicstring, String payloadstring);
void amgpelletstove_init(void(*callback)(char *,String));
void amgpelletstove_handle();
