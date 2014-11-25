#include <linux/module.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>

#define IOCTL_MAGIC_NUMBER 'm'
#define SAMPLE_WRITE \
        _IOW(IOCTL_MAGIC_NUMBER, 0x1,struct test_ioctl)

#define SAMPLE_READ \
        _IOR(IOCTL_MAGIC_NUMBER, 0x2,struct test_ioctl)

//#define WRITE 0
//#define READ 1
// module attributes
MODULE_LICENSE("GPL"); 
MODULE_DESCRIPTION("ioctl test");
MODULE_AUTHOR("Bhoomil Chavda");

struct test_ioctl {
        char data[64];
};


static int device_open(struct inode *, struct file *);
static int device_rls(struct inode *, struct file *);
static long device_ioctl(struct file *, unsigned int, unsigned long);

// structure containing callbacks
static struct file_operations fops = 
{

	.unlocked_ioctl = device_ioctl,

	.open = device_open,  // address of device_open

	.release = device_rls, // address of device_rls

};


// called when module is loaded, similar to main()
int init_module(void)
{
	int t = register_chrdev(86,"deepdev",&fops); //register driver with major:86
	
	if (t<0)
	 printk(KERN_INFO "Device registration failed..\n");
	else 
	 printk(KERN_INFO "Device registered...\n");

	return t;
}


// called when module is unloaded
void cleanup_module(void)
{
	
	unregister_chrdev(86,"deepdev");
	printk(KERN_INFO "Device unregistered...");

}


// called when 'open' system call is done on the device file
int device_open(struct inode *inod,struct file *fil)
{	
	
	printk(KERN_INFO"Device opened \n");
	return 0;
}

// called when 'ioctl' system call is done on the device file

long device_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
	
	struct test_ioctl kbuff;

	switch(cmd)
	{
	case SAMPLE_WRITE:

		copy_from_user(&kbuff,(struct test_ioctl *)args,sizeof(struct test_ioctl));		
		printk("Data is :- %s",kbuff.data);

	break;

	case SAMPLE_READ:

		printk("Reading process is done....");
		copy_to_user((struct test_ioctl *)args,&kbuff,sizeof(struct test_ioctl));			
	break;

	default:
		//ret = -EINVAL;
		printk("Invalid opton");	
}
return 0;
}

// called when 'close' system call is done on the device file
 int device_rls(struct inode *inod,struct file *fil)
{
	printk(KERN_INFO"Device closed\n");
	return 0;
}	
