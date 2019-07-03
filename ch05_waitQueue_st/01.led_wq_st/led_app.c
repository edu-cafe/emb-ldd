/***************************************
 * Filename: led_app.c
 * Title: LED Device Application
 * Desc: Implementation of system call
 ***************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>

int main(void)
{
    int fd;
    int led_val=0, i;
	pid_t pid;
    
    switch(pid = fork())
    {
      case -1:    // Fork Error
        perror("LED_app");
        exit(-1);
      case 0:    // Chile Process
        for(i=0; ;i++)
        {
          printf("[C]Child_%d...\n", i);
          sleep(1);
        }
        exit(0);
    }

    // Parent Process
    for(i=1; i<=8; i++)
    {
    	fd = open("/dev/led", O_RDWR);
        //fd = open("/dev/led", O_RDWR|O_NONBLOCK);
        if (fd<0) {
            perror("/dev/led");
            exit(-1);
        }
        else printf("[APP]LED detected...\n");

        while(1)
        {
            printf("[P]Waiting LED Write Operation...\n");
			led_val = !led_val;		//2: Led Toggle
			if(write(fd, &led_val, sizeof(led_val)) > 0) break;
            sleep(1);
        }
        printf("[P]LED Write Ok(%d)!!\n", i);
        close(fd);
    }

	kill(pid, SIGKILL);
    
    return 0;
}
