#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/uaccess.h>		//copy_from_user(). copy_to_user()
#include <linux/slab.h>			//kmalloc
#include <linux/cdev.h>			// struct cdev, ..
#include <linux/gpio.h>			//GPIO_BASE macro
#define	GPIO_LED0	18
#define	GPIO_LED1	17
#define	GPIO_LED2	23

int led_major=0, led_minor=0;
dev_t led_dev;
struct cdev led_cdev;

int led0_open (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[led0_DD]open..\n");
	
	return 0;
}

int led0_release (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[led0_DD]release..\n");
	return 0;
}

ssize_t led0_read (struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int k_data=0;
	int ret;

	printk(KERN_WARNING "[led0_DD]read..\n");

	k_data = gpio_get_value(GPIO_LED0);
	ret = copy_to_user(buf, &k_data, count);
	if(ret < 0) return -1;

	printk(KERN_WARNING "[led0_DD]read done(%d)..\n", k_data);

	return count;
}


ssize_t led0_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	int k_data;
	int ret;

	printk(KERN_WARNING "[led0_DD]write..\n");

	ret = copy_from_user(&k_data, buf, count);
	if(ret < 0) return -1;
	
	gpio_set_value(GPIO_LED0, k_data);

	//write to device
	printk(KERN_WARNING "[led0_DD]write done(%d)..\n", k_data);

	return count;
}

long led0_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_WARNING "[led0_DD]ioctl(cmd:%d)..\n", cmd);
	switch(cmd) {
		case 3 :
			gpio_set_value(GPIO_LED0, 0);
			gpio_set_value(GPIO_LED1, 0);
			gpio_set_value(GPIO_LED2, 0);
			break;
	}
	return 0;
}


int led1_open (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[led1_DD]open..\n");
	
	return 0;
}

int led1_release (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[led1_DD]release..\n");
	return 0;
}

ssize_t led1_read (struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int k_data=0;
	int ret;

	printk(KERN_WARNING "[led1_DD]read..\n");

	k_data = gpio_get_value(GPIO_LED1);
	ret = copy_to_user(buf, &k_data, count);
	if(ret < 0) return -1;

	printk(KERN_WARNING "[led1_DD]read done(%d)..\n", k_data);

	return count;
}


ssize_t led1_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	int k_data;
	int ret;

	printk(KERN_WARNING "[led1_DD]write..\n");

	ret = copy_from_user(&k_data, buf, count);
	if(ret < 0) return -1;
	
	gpio_set_value(GPIO_LED1, k_data);

	//write to device
	printk(KERN_WARNING "[led1_DD]write done(%d)..\n", k_data);

	return count;
}

long led1_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_WARNING "[led1_DD]ioctl(cmd:%d)..\n", cmd);
	switch(cmd) {
		case 3 :
			gpio_set_value(GPIO_LED0, 0);
			gpio_set_value(GPIO_LED1, 0);
			gpio_set_value(GPIO_LED2, 0);
			break;
	}
	return 0;
}


int led2_open (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[led2_DD]open..\n");
	
	return 0;
}

int led2_release (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[led2_DD]release..\n");
	return 0;
}

ssize_t led2_read (struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int k_data=0;
	int ret;

	printk(KERN_WARNING "[led2_DD]read..\n");

	k_data = gpio_get_value(GPIO_LED2);
	ret = copy_to_user(buf, &k_data, count);
	if(ret < 0) return -1;

	printk(KERN_WARNING "[led2_DD]read done(%d)..\n", k_data);

	return count;
}


ssize_t led2_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	int k_data;
	int ret;

	printk(KERN_WARNING "[led2_DD]write..\n");

	ret = copy_from_user(&k_data, buf, count);
	if(ret < 0) return -1;
	
	gpio_set_value(GPIO_LED2, k_data);

	//write to device
	printk(KERN_WARNING "[led2_DD]write done(%d)..\n", k_data);

	return count;
}

long led2_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_WARNING "[led2_DD]ioctl(cmd:%d)..\n", cmd);
	switch(cmd) {
		case 3 :
			gpio_set_value(GPIO_LED0, 0);
			gpio_set_value(GPIO_LED1, 0);
			gpio_set_value(GPIO_LED2, 0);
			break;
	}
	return 0;
}


struct file_operations led0_fops = {
    read:       led0_read,
    write:      led0_write,
    open:       led0_open,
    unlocked_ioctl:		led0_ioctl,
    release:    led0_release
};
struct file_operations led1_fops = {
    read:       led1_read,
    write:      led1_write,
    open:       led1_open,
    unlocked_ioctl:		led1_ioctl,
    release:    led1_release
};
struct file_operations led2_fops = {
    read:       led2_read,
    write:      led2_write,
    open:       led2_open,
    unlocked_ioctl:		led2_ioctl,
    release:    led2_release
};

int led_open (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[LED_DD]open..\n");
	
	switch(MINOR(inode->i_rdev)) {
		case 0:	filp->f_op = &led0_fops;	break;
		case 1: filp->f_op = &led1_fops;	break;
		case 2: filp->f_op = &led2_fops;	break;
		default :	return -ENXIO;
	}
    
	if(filp->f_op && filp->f_op->open)
		return filp->f_op->open(inode, filp);
	
	return -1;
}
struct file_operations led_fops = {
    open:       led_open,
};

int __init led_init(void)
{
	int error;

	printk("<1> LED Module id Up....\n");
	error = register_chrdev(led_major, "led", &led_fops);
	if(error < 0) {
		printk(KERN_WARNING "led:register_chrdev() error!!\n");
		return error;
	} else if(led_major==0) led_major = error;
	printk("<1> led:register_chrdev() ok! => major_num:%d\n", led_major);
	
	gpio_request(GPIO_LED0, "LED0");
	gpio_direction_output(GPIO_LED0, 0);
	gpio_request(GPIO_LED1, "LED1");
	gpio_direction_output(GPIO_LED1, 0);
	gpio_request(GPIO_LED2, "LED2");
	gpio_direction_output(GPIO_LED2, 0);	
	
	printk(KERN_WARNING "LED Module Insert Done!!\n");
	printk("You must execute next commands...\n");
	printk("mknod /dev/led0 c %d %d\n", led_major, led_minor);
	printk("mknod /dev/led1 c %d %d\n", led_major, led_minor+1);
	printk("mknod /dev/led2 c %d %d\n", led_major, led_minor+2);
	printk("chmod 666 /dev/led0\n");	
	printk("chmod 666 /dev/led1\n");	
	printk("chmod 666 /dev/led2\n");	
	
	return 0;
}

void __exit led_exit(void)
{
	printk("<1> LED Module id Down....\n");

	gpio_set_value(GPIO_LED0, 0);
	gpio_free(GPIO_LED0);
	gpio_set_value(GPIO_LED1, 0);
	gpio_free(GPIO_LED1);
	gpio_set_value(GPIO_LED2, 0);
	gpio_free(GPIO_LED2);
	
	unregister_chrdev(led_major, "led");	

	printk(KERN_WARNING "LED Module Delete Done!!\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("Dual BSD/GPL");
