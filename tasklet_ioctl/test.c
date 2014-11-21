#include <stdio.h>                                
#include <fcntl.h>                                
#include <unistd.h>                               
#include <linux/ioctl.h>                            
#define DEVICE_PATH "/dev/ioctl_driver" 
#define IOCTL_MAGIC 's'      
#define IOCTL_READ _IOR(IOCTL_MAGIC, 1 , char *) //read driver data     
#define IOCTL_WRITE _IOW(IOCTL_MAGIC, 2 , char *) //write driver data      

char ioctl_data[20];


int main()
{
	        

		printf("Enter data\n");
		gets(ioctl_data);
	    	
		int fd = open(DEVICE_PATH, O_RDWR);
		
		ioctl(fd,IOCTL_READ,&ioctl_data);		
		                                                                       
                close(fd);                                      
                return 0;                                       
  }                                                 
 
