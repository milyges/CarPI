#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define I2C_ADDRESS           0x30

#define REG_POWER_STATE       0x00

#define REG_POWER_STATE_MASK  0x01
#define REG_POWER_ENABLED     (1 << 0) /* Zasilanie RPi, R/W */
#define REG_POWER_RADIO       (1 << 1) /* Radio włączone, R */
#define REG_POWER_IGNITION    (1 << 2) /* Zapłon włączony, R */

int main(int argc, char * argv[]) {
	char * i2cdev = "/dev/i2c-1";
	int fd;
	char poweroff_cmd[1] = { 0x00 };
	
	if ((fd = open(i2cdev, O_RDWR)) < 0) {
		perror("I2C Open");
		return 1;
	}
	
	if (ioctl(fd, I2C_SLAVE, I2C_ADDRESS) < 0) {
		perror("I2C ioctl");
		close(fd);
		return 2;
	}
	
	/* Wysyłamy poweroff */
	write(fd, poweroff_cmd, 1);
	
	close(fd);
	return 0;
}