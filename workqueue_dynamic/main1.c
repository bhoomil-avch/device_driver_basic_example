#include <stdio.h>	// standard input oputput.
#include <fcntl.h>	// function control operation like open close.
#include <string.h> 	// for memset and strlen
#include <assert.h>	// for assert function
#include <stdlib.h>	// for exit function

int main()
{
	char buff1[100]={};

	int fd = open("/dev/my_driver", O_RDWR);
	
	read(fd,buff1,100);
	printf("Reading from Device : ");
	printf("%s\n",buff1);
	
}
