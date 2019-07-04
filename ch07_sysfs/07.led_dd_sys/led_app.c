/***************************************
 * Filename: led_app.c
 * Title: LED Device Application
 * Desc: Implementation of system call
 ***************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
    int fd;

    int menu=0, led_val=0;
    
    fd = open("/dev/led", O_RDWR);
    if (fd<0) {
        perror("/dev/led");
        exit(-1);
    }
    else
        printf("[APP]LED detected...\n");
    
    while(1)
    {
        printf("\n\n===== LED Test Program ====\n");
        printf(" 1. LED On\n");
        printf(" 2. LED Off\n");
        printf(" 0. Program Quit\n");
        do {
            printf(" Select Menu => ");
            scanf("%d", &menu);
        } while(menu<0 || menu>2);

        if(menu == 0) break;

		if(menu == 1) led_val = 1;
		else if(menu == 2) led_val = 0;
      	printf("write call(led_val:%d)\n", led_val);
		write(fd, &led_val, sizeof(led_val));
    }

    close(fd);
    
    return 0;
}
