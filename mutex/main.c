#include <stdio.h>	// standard input oputput.
#include <fcntl.h>	// function control operation like open close.
#include <string.h> 	// for memset and strlen
#include <assert.h>	// for assert function
#include <stdlib.h>	// for exit function

int main(int argc ,char *argv[])
{

	assert(argc > 1);
	char buf[100] ;
	char i = 0;
	memset(buf, 0, 100);
	printf("Input string: %s\n", argv[1]);

	int fp = open("/dev/My_Char_Device", O_RDWR);
	write(fp, argv[1], strlen(argv[1]));
	
	memset(buf,0,100);
	read(fp,buf,100);
	printf("Now reading from device : ");
	printf("%s\n",buf);
}
