#include<linux/init.h>
#include<linux/module.h>
#include<linux/string.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<asm/uaccess.h>
#include <linux/ioctl.h> 
#include<linux/wait.h>
#include<linux/sched.h>
#include<linux/interrupt.h>
//#define WRITE 0
//#define READ 1
//#define IOC_WR 2
#define IOCTL_MAGIC 's'            
#define IOCTL_READ _IOR(IOCTL_MAGIC, 1 , char *) //read driver data     
#define IOCTL_WRITE _IOW(IOCTL_MAGIC, 2 , char *) //write driver data     


#define DEVICE_NAME "ioctl_driver"
#define DEVICE_PATH "/dev/ioctl_driver"

char my_tasklet_data[]="my_tasklet_function was called";
static DECLARE_WAIT_QUEUE_HEAD(wait);
static int flag = 0;

void my_tasklet_function( unsigned long data )
{
  printk( "%s\n", (char *)data );
  return;
}

static DECLARE_TASKLET( my_tasklet, my_tasklet_function,(unsigned long) &my_tasklet_data );

char ioctl_data[20]={};
static int major_no;
static int device_open(struct inode *inode, struct file *);
static int device_release(struct inode *inode, struct file *);
static int device_ioctl(struct file *filp, unsigned int cmd, unsigned long args);


static struct class *my_class;

static struct file_operations fops={
.open = device_open,
.release = device_release,
.unlocked_ioctl = device_ioctl,
};
static int hello_init(void)
{
major_no = register_chrdev(0, DEVICE_NAME, &fops);
printk("\n Major_no : %d", major_no);

my_class = class_create(THIS_MODULE, DEVICE_NAME);
device_create(my_class, NULL, MKDEV(major_no,0), NULL, DEVICE_NAME);
printk("\n Device Initialized in kernel ....!!!");
return 0;
}


static void hello_exit(void)
{
printk("\n Device is Released or closed \n");
device_destroy(my_class,MKDEV(major_no,0));
class_unregister(my_class);
class_destroy(my_class);
unregister_chrdev(major_no, DEVICE_NAME);
printk("\n===============================================================\n");
tasklet_kill( &my_tasklet );
}

static int device_open(struct inode *inode, struct file *file)
{
printk("\n Open \n");
	
return 0;
}

static int device_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
	//printk("IOCTL working\n");
	
	struct task_struct *current;	
	

	//struct test_ioctl buff;
	char buff[20];
	
	switch(cmd)
	{
	case IOCTL_WRITE:
	tasklet_schedule( &my_tasklet );
	printk("\n Write \n");
	printk("Process %i awoken\n",current->pid);
	flag = 1;
        wake_up_interruptible(&wait);
	copy_from_user(&buff,&ioctl_data,20);
	//printk("Data written is : %s",buff);
	break;

	case IOCTL_READ:
	printk("Process awakening: %i\n",current->pid);
	printk("\n Reading the data \n");
	//struct task_struct *current;
	printk("Process %i going to sleep\n",current->pid);
	//wait_queue_head_t wait;
	//init_waitqueue_head(&wait);
	wait_event_interruptible(wait, flag != 0);
        flag = 0;
	
	copy_to_user(&ioctl_data,&buff,strlen(buff));			
	break;
	}
	return 0;
}


static int device_release(struct inode *inode, struct file *file)
{
printk("\n Release \n");
return 0;
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
