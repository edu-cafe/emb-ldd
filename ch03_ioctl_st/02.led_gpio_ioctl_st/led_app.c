#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "led.h"
#define	DEVICE_FILENAME	"/dev/led"

int main(void)
{
	int fd;
	int tx_data, rx_data;
	int ret;
	unsigned int flag;
	LED_INFO info;

	fd = open(DEVICE_FILENAME, O_RDWR);
	getchar();
	if(fd < 0) {
		perror("/dev/led file open error!!");
		exit(-1);
	} else printf("/dev/led file open ok!!\n");

	tx_data = 1;
	printf("app => write request(tx_data:%d)!!\n", tx_data);
	ret = write(fd, &tx_data, sizeof(int));
	getchar();
	printf("app => write done(ret:%d)!!\n", ret);

	printf("app => read request!!\n");
	ret = read(fd, &rx_data, sizeof(int));
	getchar();
	printf("app => read done(rx_data:%d, ret:%d)!!\n", rx_data, ret);

	tx_data = 0;
	printf("app => write request(tx_data:%d)!!\n", tx_data);
	ret = write(fd, &tx_data, sizeof(int));
	getchar();
	printf("app => write done(ret:%d)!!\n", ret);
	
	printf("app => ioctl request(cmd:LED_ON)!!\n");
	tx_data = 1;
	ioctl(fd, LED_ON, &tx_data);
	printf("Press Enter to continue...\n");
	getchar();
	printf("app => ioctl done(ret:%d)!!\n", ret);

	printf("app => ioctl request(cmd:LED_OFF)!!\n");
	tx_data = 0;
	ioctl(fd, LED_OFF, &tx_data);
	printf("Press Enter to continue...\n");
	getchar();
	printf("app => ioctl done(ret:%d)!!\n", ret);

	close(fd);
	return 0;
}

