#ifndef __POWER_H
#define __POWER_H

char power_radio_state(void);
char power_ignition_state(void);

void power_sleep(void);
void power_set_monitor(char enabled);
void power_set_main(char enabled);
void power_init(void);

#endif /* __POWER_H */
