#ifndef __PILOT_H
#define __PILOT_H

#include <stdint.h>

#define PILOT_KEY_SOURCE_L   (1 << 0)
#define PILOT_KEY_SOURCE_R   (1 << 1)
#define PILOT_KEY_LOAD       (1 << 2)
#define PILOT_KEY_VOLUP      (1 << 3)
#define PILOT_KEY_VOLDOWN    (1 << 4)
#define PILOT_KEY_PAUSE      (PILOT_KEY_VOLUP | PILOT_KEY_VOLDOWN)
#define PILOT_ROLL_NEXT      (1 << 5)
#define PILOT_ROLL_PREV      (1 << 6)

void pilot_init(void);
uint8_t pilot_loop(void);

#endif /* __PILOT_H */
