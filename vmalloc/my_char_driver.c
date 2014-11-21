#include<linux/module.h>
#include<linux/init.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <asm/current.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include<linux/slab.h>
#include<linux/mm_types.h>
#include<linux/gfp.h>
#include<linux/mm.h>
#include<linux/vmalloc.h>




MODULE_AUTHOR("DEVANG");
MODULE_DESCRIPTION("A simple char device driver");

int times;
char *buff;
char *ptr;
char my_data[80]={}; /* our device */
struct page *pages;

static int r_init(void);
static void r_cleanup(void);
int my_open(struct inode *inode,struct file *filep);
int my_release(struct inode *inode,struct file *filep);
ssize_t my_read(struct file *filep,char *buff,size_t count,loff_t *offp );
ssize_t my_write(struct file *filep,const char *buff,size_t count,loff_t *offp );




struct file_operations my_fops={
	open: my_open,
	read: my_read,
	write: my_write,
	release:my_release,
};

static int r_init(void)
{
printk("<1>Device Registered\n");
if(register_chrdev(87,"My_Char",&my_fops)){
	printk("<1>failed to register");
}
return 0;
}
static void r_cleanup(void)
{
printk("<1>Device Unregistered\n");
unregister_chrdev(87,"My_Char");
return ;
}


int my_open(struct inode *inode,struct file *filep)
{	
	pages=alloc_pages(GFP_KERNEL,0);
	if(pages==NULL)
	{
		printk("Pages are not allocated\n");
	}
	printk("Pages allocated\n");
	ptr=(char *)page_address(pages);
	
	buff=vmalloc(20);
	if(buff==NULL)
	printk("Memory is not available any more\n");
	printk("Memory is available\n");
	
	/*MOD_INC_USE_COUNT;*/ /* increments usage count of module */
	times++;
	printk("char driver open %d time(s)\n",times);
	return 0;
}

int my_release(struct inode *inode,struct file *filep)
{	
	vfree(buff);
	if(buff==NULL)
	printk("Memory has freed\n");
	/*MOD_DEC_USE_COUNT;*/ /* decrements usage count of module */
	printk("char driver release\n");
	__free_pages(pages,0);
	return 0;
}
ssize_t my_read(struct file *filep,char *buff,size_t count,loff_t *offp )
{
	/* function to copy kernel space buffer to user space*/
	printk("Reading from the device\n");
	if ( copy_to_user(buff,my_data,strlen(my_data)) != 0 )
		printk( "Kernel -> userspace copy failed!\n" );
	return strlen(my_data);

}
ssize_t my_write(struct file *filep,const char *buff,size_t count,loff_t *offp )
{
	/* function to copy user space buffer to kernel space*/
	printk("Writing into the device\n");
	if ( copy_from_user(my_data,buff,count) != 0 )
		printk( "Userspace -> kernel copy failed!\n" );
	return 0;
}

module_init(r_init);
module_exit(r_cleanup);

