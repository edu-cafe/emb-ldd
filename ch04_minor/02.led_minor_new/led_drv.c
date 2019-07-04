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
struct cdev led_cdev[3];

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


struct file_operations led_fops[3] = {
{
    read:       led0_read,
    write:      led0_write,
    open:       led0_open,
    unlocked_ioctl:		led0_ioctl,
    release:    led0_release
},
{
    read:       led1_read,
    write:      led1_write,
    open:       led1_open,
    unlocked_ioctl:		led1_ioctl,
    release:    led1_release
},
{
    read:       led2_read,
    write:      led2_write,
    open:       led2_open,
    unlocked_ioctl:		led2_ioctl,
    release:    led2_release
}
};


int __init led_init(void)
{
	int error, i;

	printk("<1> LED Module id Up....\n");

	if(led_major) {
		led_dev = MKDEV(led_major, led_minor);
		error = register_chrdev_region(led_dev, 3, "LED");
	} else {
		error = alloc_chrdev_region(&led_dev, led_minor, 3, "LED");
		led_major = MAJOR(led_dev);
	}
	if(error < 0) {
		printk(KERN_WARNING "LED:alloc_major error!!\n");
		return error;
	}

	printk("LED:alloc_major ok! => major_num:%d\n", led_major);

	
	for(i=0; i<3; i++) {
		// register_chrdev
		cdev_init(&led_cdev[i], &led_fops[i]);
		led_cdev[i].owner = THIS_MODULE;
		error = cdev_add(&led_cdev[i], led_dev+i, 1);
		if(error < 0) {
			printk(KERN_WARNING "led:register_chrdev error!!\n");
			return error;
		}
	}
	
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
	int i;
	
	printk("<1> LED Module id Down....\n");

	gpio_set_value(GPIO_LED0, 0);
	gpio_free(GPIO_LED0);
	gpio_set_value(GPIO_LED1, 0);
	gpio_free(GPIO_LED1);
	gpio_set_value(GPIO_LED2, 0);
	gpio_free(GPIO_LED2);
	
	for(i=0; i<3; i++) {
		cdev_del(&led_cdev[i]);
	}
	unregister_chrdev_region(led_dev, 3);	

	printk(KERN_WARNING "LED Module Delete Done!!\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("Dual BSD/GPL");
