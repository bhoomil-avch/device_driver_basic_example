// String reversal done by device driver

#include<stdio.h>
#include <fcntl.h>
#include <linux/ioctl.h>

#define IOCTL_MAGIC_NUMBER 'm'
#define SAMPLE_WRITE \
        _IOW(IOCTL_MAGIC_NUMBER, 0x1,struct test_ioctl)

#define SAMPLE_READ \
        _IOR(IOCTL_MAGIC_NUMBER, 0x2,struct test_ioctl)

//#define WRITE 0
//#define READ 1

struct test_ioctl 
{        
        char data[64];
};


int main()
{
	
	struct test_ioctl i1;
	char ch ;

		printf("Enter data\n");
		gets(i1.data);
	    	
		int fd = open("/dev/deepdev", O_RDWR);		
		
		ioctl(fd,SAMPLE_WRITE,&i1);
		ioctl(1,SAMPLE_READ,&i1);				
		//ioctl(1,SAMPLE_READ,&i1);				
close(fd);
		
}
