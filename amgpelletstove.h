/* 
 * Library used for AMG (Micronova/Duepi) pellet stoves. 
 * 
 * For schematic see: https://easyeda.com/jeroenst/duepi-pellet-stove-wifi
 * 
 */


void amgpelletstove_receivemqtt(String topicstring, String payloadstring);
void amgpelletstove_init(void(*callback)(String,String),void(*debugcallback)(String, String));
void amgpelletstove_handle();
