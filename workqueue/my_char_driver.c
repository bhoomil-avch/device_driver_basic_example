#include<linux/module.h>
#include<linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <asm/current.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include<linux/wait.h>
#include<linux/interrupt.h>
#include<linux/workqueue.h>



MODULE_LICENSE("GPL");
MODULE_AUTHOR("DEVANG");
MODULE_DESCRIPTION("A simple char device driver");


static DECLARE_WAIT_QUEUE_HEAD(wait);
static int flag = 0;



int times;
char my_data[80]={}; /* our device */


static int r_init(void);
static void r_cleanup(void);
int my_open(struct inode *inode,struct file *filep);
int my_release(struct inode *inode,struct file *filep);
ssize_t my_read(struct file *filep,char *buff,size_t count,loff_t *offp );
ssize_t my_write(struct file *filep,const char *buff,size_t count,loff_t *offp );

void my_interrupt(unsigned long);

struct  work_struct my_work =
{
	.func = my_interrupt,
};


struct file_operations my_fops={
	open: my_open,
	read: my_read,
	write: my_write,
	release:my_release,
};

static int r_init(void)
{
printk("<1>Device Registered\n");
if(register_chrdev(85,"my_driver",&my_fops)){
	printk("<1>failed to register");
	INIT_WORK(&my_work,my_interrupt);
}
return 0;
}
static void r_cleanup(void)
{
printk("<1>Device Unregistered\n");
unregister_chrdev(85,"my_driver");
//tasklet_kill( &my_tasklet );

return ;
}


void my_interrupt(unsigned long i)
{
	printk("INTERRUPT IS GENERATED\n");
	wake_up_interruptible(&wait);
}


int my_open(struct inode *inode,struct file *filep)
{
	/*MOD_INC_USE_COUNT;*/ /* increments usage count of module */
	times++;
	printk("char driver open %d time\n",times);
	return 0;
}

int my_release(struct inode *inode,struct file *filep)
{
	/*MOD_DEC_USE_COUNT;*/ /* decrements usage count of module */
	printk("char driver release\n");
	return 0;
}
ssize_t my_read(struct file *filep,char *buff,size_t count,loff_t *offp )
{
	/* function to copy kernel space buffer to user space*/
	printk("Reading from the device\n");
	struct task_struct *current;
	printk("Process %i going to sleep\n",current->pid);
	//wait_queue_head_t wait;
	//init_waitqueue_head(&wait);
	wait_event_interruptible(wait, flag != 0);
        flag = 0;
	printk("Process awakened: %i\n",current->pid);
	
	if ( copy_to_user(buff,my_data,strlen(my_data)) != 0 )
		printk( "Kernel -> userspace copy failed!\n" );
	return strlen(my_data);

}
ssize_t my_write(struct file *filep,const char *buff,size_t count,loff_t *offp )
{
	//tasklet_schedule( &my_tasklet );
	schedule_work(&my_work);

	/* function to copy user space buffer to kernel space*/
	printk("Writing into the device\n");
	struct task_struct *current;	
	printk("Process %i awoken\n",current->pid);
	//flag = 1;
        //wake_up_interruptible(&wait);


	if ( copy_from_user(my_data,buff,count) != 0 )
		printk( "Userspace -> kernel copy failed!\n" );
	
	return 0;
}

module_init(r_init);
module_exit(r_cleanup);

