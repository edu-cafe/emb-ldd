/***************************************
 * Filename: key_app.c
 * Title: KEY Device Application
 * Desc: Implementation of system call
 ***************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
    int fd_key, fd_led;
    int menu=0;
    int key_stat = 0xff, led_val=0;
    int i;
    
    fd_key = open("/dev/key", O_RDWR);
    if (fd_key<0) {
        perror("/dev/key");
        exit(-1);
    }
    else printf("[APP]KEY detected...\n");
	
	fd_led = open("/dev/led", O_RDWR);
    if (fd_led<0) {
        perror("/dev/led");
        exit(-1);
    }
    else printf("[APP]LED detected...\n");
    
    while(1)
    {
        printf("\n\n===== KEY Test Program ====\n");
        printf(" 1. KEY Check\n");
        printf(" 0. Program Quit\n");
        do {
            printf(" Select Menu => ");
            scanf("%d", &menu);
        } while(menu<0 || menu>1);

        if(menu == 0) break;
        printf("\n\nKEY Input Waiting...\n");
	read(fd_key, &key_stat, sizeof(key_stat));
	led_val = 1;
	write(fd_led, &led_val, sizeof(led_val)); 
	sleep(1);
	led_val = 0;
	write(fd_led, &led_val, sizeof(led_val)); 
    }

    close(fd_key);
    close(fd_led);
    
    return 0;
}
