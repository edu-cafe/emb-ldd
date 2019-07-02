#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>

#define	DEVICE_FILENAME	"________"

#ifdef PI1
#define	BCM_IO_BASE	0x20000000
#else
#define	BCM_IO_BASE	0x3F000000
#endif
#define	GPIO_BASE	(BCM_IO_BASE + 0x200000)
#define	GPIO_SIZE	(256)

#define	GPIO_IN(g)		( *(gpio+((g)/10)) &= ~(7 << (((g)%10)*3)) )
#define	GPIO_OUT(g)		( *(gpio+((g)/10)) |= (1 << (((g)%10)*3)) )

#define	GPIO_SET(g)		( *(gpio+7) = 1<<g )
#define	GPIO_CLR(g)		( *(gpio+10) = 1<<g )
#define	GPIO_GET(g)		( *(gpio+13) & (1<<g) )

#define	GPIO_LED	18

volatile unsigned int *gpio;

int main(void)
{
	int fd;
	void *gpio_map;
	int i;

	fd = open(DEVICE_FILENAME, O_RDWR|O_SYNC);
	if(fd < 0) {
		perror("/dev/mem file open error!!");
		exit(-1);
	} else printf("/dev/mem file open ok!!\n");

	gpio_map = _____(NULL, GPIO_SIZE, PROT_READ|PROT_WRITE, 
				MAP_SHARED, fd, GPIO_BASE);
	if(gpio_map == MAP_FAILED) {
		printf("[APP]gpio_mmap error!!\n");
		exit(-2);
	}
	gpio = (volatile unsigned int *)________;
	printf("gpio mmap ok(addr:%p)!!\n", gpio);
	
	............
	for(i=0; i<5; i++) {
		..........
		sleep(1);
		..........
		sleep(1);
	}	
	
	...........
	close(fd);
	printf("led end...\n");
	return 0;
}

