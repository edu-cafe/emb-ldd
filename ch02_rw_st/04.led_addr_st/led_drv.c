#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/uaccess.h>		//copy_from_user(). copy_to_user()
#include <linux/slab.h>			//kmalloc
#include <linux/cdev.h>			//struct cdev, ..
#include <linux/_____.h>			//ioremap(), iounmap(), ..

#ifdef PI1
#define	BCM_IO_BASE	0x20000000
#else
#define	BCM_IO_BASE	0x3F000000
#endif
#define	GPIO_BASE	(BCM_IO_BASE + 0x200000)
#define	GPIO_SIZE	(256)


//GPFSEL0~5 : 0x00~0x14 ManP.90
#define	GPIO_IN(g)		( *(gpio+((g)/10)) &= ~(7 << (((g)%10)*3)) )
#define	GPIO_OUT(g)		( *(gpio+((g)/10)) |= (1 << (((g)%10)*3)) )

//GPSET0 : 0x1C(28)
#define	GPIO_SET(g)		( *(gpio+7) = 1<<g )

//GPCLR0 : 0x28(40)
#define	GPIO_CLR(g)		( *(gpio+10) = 1<<g )

//GPLEV0 : 0x34(52)
#define	GPIO_GET(g)		( *(gpio+13) & (1<<g) )

#define	GPIO_LED	18

volatile unsigned int *gpio;
volatile unsigned int *t_addr;

int led_major=0, led_minor=0;
dev_t led_dev;
struct cdev led_cdev;

int led_open (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[led_DD]open..\n");
	
	return 0;
}

int led_release (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[led_DD]release..\n");
	return 0;
}

ssize_t led_read (struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int k_data=0;
	int ret;

	printk(KERN_WARNING "[led_DD]read..\n");


#ifdef MYMACRO
	k_data = ...........
#else
	t_addr = gpio + 13;	//0x34(52) : GPLEV0
	k_data = *(t_addr);
#endif

	ret = copy_to_user(buf, &k_data, count);
	if(ret < 0) return -1;

	printk(KERN_WARNING "[led_DD]read done(%d)..\n", k_data);

	return count;
}


ssize_t led_write (struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	int k_data;
	int ret;

	printk(KERN_WARNING "[led_DD]write..\n");

	ret = copy_from_user(&k_data, buf, count);
	if(ret < 0) return -1;
	
#ifdef MYMACRO
	printk("===> Using MACRO!!\n");
	if(k_data) .........
	else ............
#else
	printk("---> Not Using MACRO!!\n");
	if(k_data) {
		t_addr = gpio + 7;	//0x1C(28) : GPSET0
		*(t_addr) = 1 << GPIO_LED;
	} else {
		t_addr = gpio + 10;	//0x28(40) : GPCLR0
		*(t_addr) = 1 << GPIO_LED;
	}
#endif

	//write to device
	printk(KERN_WARNING "[led_DD]write done(%d)..\n", k_data);


	return count;
}


//int led_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
long led_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_WARNING "[led_DD]ioctl(cmd:%x)..\n", cmd);

	return 0;
}

struct file_operations led_fops = {
    read:       led_read,
    write:      led_write,
    open:       led_open,
//    ioctl:		led_ioctl,
    unlocked_ioctl:		led_ioctl,
    release:    led_release
};

int __init led_init(void)
{
	int error;
	void *map;

	printk("<1> LED Module id Up....\n");

	if(led_major) {
		led_dev = MKDEV(led_major, led_minor);
		error = register_chrdev_region(led_dev, 1, "LED");
	} else {
		error = alloc_chrdev_region(&led_dev, led_minor, 1, "LED");
		led_major = MAJOR(led_dev);
	}
	if(error < 0) {
		printk(KERN_WARNING "LED:alloc_major error!!\n");
		return error;
	}

	printk("LED:alloc_major ok! => major_num:%d\n", led_major);


	// register_chrdev
	cdev_init(&led_cdev, &led_fops);
	led_cdev.owner = THIS_MODULE;
	error = cdev_add(&led_cdev, led_dev, 1);
	if(error < 0) {
		printk(KERN_WARNING "LED:register_chrdev error!!\n");
		return error;
	}
	
	map = .............
	if(!map) {
		printk(KERN_WARNING "LED:mapping GPIO memory error!!\n");
		return -EBUSY;
	}
	gpio = (volatile unsigned int *)_____;
	
#ifdef MYMACRO
	..........
	..........
#else
	t_addr = gpio + (GPIO_LED/10);	//GPFSEL0~5
	*(t_addr) &= ~(7 << ((GPIO_LED%10)*3) );
	*(t_addr) |= (1 << ((GPIO_LED%10)*3) );
#endif

	printk(KERN_WARNING "LED Module Insert Done!!\n");
	printk("You must execute next commands...\n");
	printk("mknod /dev/led c %d %d\n", led_major, led_minor);
	printk("chmod 666 /dev/led\n");	
	
	return 0;
}

void __exit led_exit(void)
{
	printk("<1> LED Module id Down....\n");

	cdev_del(&led_cdev);
	unregister_chrdev_region(led_dev, 1);
	
	if(gpio) {
		...........
	}

	printk(KERN_WARNING "LED Module Delete Done!!\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("Dual BSD/GPL");
