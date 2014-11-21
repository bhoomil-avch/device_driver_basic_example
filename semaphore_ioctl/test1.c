#include <stdio.h>                                
#include <fcntl.h>                                
#include <unistd.h>                               
#include <linux/ioctl.h>                            
#define DEVICE_PATH "/dev/ioctl_device" 
#define IOCTL_MAGIC 's'      
#define IOCTL_READ _IOR(IOCTL_MAGIC, 1 , char *) //read driver data     
#define IOCTL_WRITE _IOW(IOCTL_MAGIC, 2 , char *) //write driver data      

char ioctl_data[20];

/*struct test_ioctl 
{        
        char ioctl_data[64];
};  */         
 
int main()
{
	
	        //struct test_ioctl ioc;
	        

		printf("Enter data\n");
		gets(ioctl_data);
	    	
		int fd = open(DEVICE_PATH, O_RDWR);
		
		
		ioctl(fd,IOCTL_WRITE,&ioctl_data);		
		
		
						
						

                                                                                           
                close(fd);                                      
                return 0;                                       
  }                                                 
 
