#include<linux/module.h>
#include<linux/string.h>
#include<linux/fs.h>
#include<asm/uaccess.h>
#include<linux/capability.h>
#include<linux/cdev.h>
#include <linux/major.h>
#include<linux/miscdevice.h>




MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bhoomil Chavda");
MODULE_DESCRIPTION("misc kernel module");

static int dev_open(struct inode *,struct file*);
static int dev_release(struct inode *,struct file *);
static ssize_t dev_read(struct file*,char *,size_t,loff_t*);
static ssize_t dev_write(struct file*,const char*,size_t,loff_t*);


static struct file_operations fops1 = {
	.open = dev_open,
	.release = dev_release,
	.read = dev_read,
	.write = dev_write,
};

struct miscdevice misc_driver={
	.minor = MISC_DYNAMIC_MINOR,
	.name = "misc_dev",
	.mode = 0666,
	.fops = &fops1,
};


#define sample_major_no 250
#define requested_device 1

static short count=0;
static short int readPos=0;
static char kbuff[100]={0};

//static struct cdev char_cdev;
//static dev_t dev;

int init_module(void)
{
	int ret=0;
	ret=misc_register(&misc_driver);
	if(ret<0)
	{
		printk(KERN_ALERT "Error in registering device\n"); //if error occurs in registering device
		return 0;
	}
	else
	{
		printk(KERN_ALERT "Device registered with major no:10 and minor no:");
		printk("%d\n",misc_driver.minor);
	}
	printk("Your device is registered\n");
return 0;	
}

void cleanup_module(void)
{
	misc_deregister(&misc_driver);
	printk(KERN_INFO "You have unregistered\n");
}


static int dev_open(struct inode* inode,struct file* fil)
{
	count++;
	printk(KERN_INFO "Device is opened %d\n",count);	
	return 0;
}

static int dev_release(struct inode* inode,struct file *fil)
{
	printk(KERN_INFO "Device is now closed\n");	
	return 0;
}

static ssize_t dev_read(struct file *filp,char *buff,size_t len,loff_t* offset)
{
	short count=0;
	char *temp;
	temp=buff;
	//short ind=0;
	printk("Now you are reading from device\n");
	
	while(len)
	{
		put_user(kbuff[readPos],buff++);	
		readPos++;
		len--;
		count++;
	}
	printk("%s\n",temp);
	return count;	

}

static ssize_t dev_write(struct file *filp,const char *buff,size_t len,loff_t *offset)
{
	short ind=0;
	short count=0;
	printk("Now you are writing into device\n");
	memset(kbuff,0,100);
	readPos=0;
	while(len>0)
	{
		kbuff[count++]=buff[ind];
		len--;
		ind++;
	}
	printk("%s\n",kbuff);
	return 0;
}




