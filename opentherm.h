void opentherm_init(void(*callback)(char *,String));
int opentherm_handle();
void opentherm_setthermosttattemporary(double value);
void opentherm_setthermosttatcontinue(double value);
void opentherm_setchwatertemperature(int8_t value);
void opentherm_setmaxmodulationlevel(int8_t value);
void opentherm_setoutsidetemperature(double value);
void opentherm_reset();
