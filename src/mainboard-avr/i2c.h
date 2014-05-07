#ifndef __I2C_H
#define __I2C_H

#define I2C_ADDRESS           0x30

#define REG_POWER_STATE       0x00
#define REG_TEMP_HIGH         0x01
#define REG_TEMP_LOW          0x02
#define REG_KEY_CNT           0x03
#define REG_KEY_0             0x04
#define REG_KEY_1             0x05
#define REG_KEY_2             0x06
#define REG_KEY_3             0x07
#define REG_KEY_4             0x08
#define REG_KEY_5             0x09
#define REG_KEY_6             0x0A
#define REG_KEY_7             0x0B

#define REG_MAX               0x0C

#define REG_POWER_STATE_MASK  0x01
#define REG_POWER_ENABLED     (1 << 0) /* Zasilanie RPi, R/W */
#define REG_POWER_RADIO       (1 << 1) /* Radio włączone, R */
#define REG_POWER_IGNITION    (1 << 2) /* Zapłon włączony, R */

void i2c_init(void);

#endif /* __I2C_H */
