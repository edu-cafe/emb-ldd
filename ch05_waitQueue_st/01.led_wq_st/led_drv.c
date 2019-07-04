#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/uaccess.h>		//copy_from_user(). copy_to_user()
#include <linux/slab.h>			//kmalloc
#include <linux/cdev.h>			// struct cdev, ..
#include <linux/gpio.h>			//GPIO_BASE macro

#include <linux/_____.h>
#include <linux/_____.h>

void mytimer_off(void);

#define	GPIO_LED	18

int led_major=0, led_minor=0;
dev_t led_dev;
struct cdev led_cdev;

..... mytimer;
volatile unsigned int flag_timeron=0;
volatile unsigned int flag_ledInt=0;
______(wq_timer);

.... mytimer_func(.....)
{
	printk("LED Kernel Timer Int....\n");
	flag_ledInt = 1;
#if 0
#else
	..........
#endif
}

void mytimer_on(void) {
	.....
	
	
	flag_timeron = 1;
}

void mytimer_off(void) {
	........
	flag_timeron = 0;
}

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

	k_data = gpio_get_value();
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

	if(!flag_timeron) mytimer_on(GPIO_LED);
#if 0
	while(!flag_ledInt);
#else
	while(!flag_ledInt) {
		.............
			
		printk(KERN_WARNING "[LED_DD]Waiting led Int..\n");
		.............
		printk(KERN_WARNING "[LED_DD]After led Int(ret:%d)..\n", ret);
	}
#endif
	printk(KERN_WARNING "[LED_DD]After led Int..\n");
	
	ret = copy_from_user(&k_data, buf, count);
	if(ret < 0) return -1;
	
	//write to device
	gpio_set_value(GPIO_LED, k_data);
	printk(KERN_WARNING "[led_DD]write done(%d)..\n", k_data);
	
	..........
	flag_timeron = 0;
	return count;
}

//int led_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
long led_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_WARNING "[LED_DD]ioctl(cmd:%d)..\n", cmd);
	return 0;
}

struct file_operations led_fops = {
    read:       led_read,
    write:      led_write,
    open:       led_open,
    unlocked_ioctl:		led_ioctl,
    release:    led_release
};

int __init led_init(void)
{
	int error;

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
	
	gpio_request(GPIO_LED, "LED");
	gpio_direction_output(GPIO_LED, 0);

	printk(KERN_WARNING "LED Module Insert Done!!\n");
	printk("You must execute next commands...\n");
	printk("mknod /dev/led c %d %d\n", led_major, led_minor);
	printk("chmod 666 /dev/led\n");	
	
	return 0;
}

void __exit led_exit(void)
{
	printk("<1> LED Module id Down....\n");
	mytimer_off();

	gpio_set_value(GPIO_LED, 0);
	gpio_free(GPIO_LED);
	
	cdev_del(&led_cdev);
	unregister_chrdev_region(led_dev, 1);	

	printk(KERN_WARNING "LED Module Delete Done!!\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("Dual BSD/GPL");
