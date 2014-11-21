#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<assert.h>
#include<string.h>
#include<stdlib.h>


int main(int argc,char* argv[])
{
assert(argc>1);
char buff[100]={};
int fd;
printf("INPUT: %s\n",argv[1]);
fd=open("/dev/misc_dev",O_RDWR);
if(fd<0)
	{
		printf("Error in opening file\n");
		exit(0);
	}
write(fd,argv[1],100);
printf("\nEnter you have written in device file\n");
read(fd,buff,100);
printf("%s\n",buff);

}
