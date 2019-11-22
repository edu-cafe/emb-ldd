#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/uaccess.h>		//copy_from_user(). copy_to_user()
#include <linux/slab.h>			//kmalloc
#include <linux/delay.h>		//msleep

#include <linux/cdev.h>			// struct cdev, ..

#include <linux/gpio.h>			//GPIO_BASE macro
#define	GPIO_LED0	18
#define	GPIO_LED1	17
#define	GPIO_LED2	23

int led_major=0, led_minor=0;
dev_t led_dev;
struct cdev led_cdev;

struct led_dev_data {
	unsigned int led_no;
	unsigned int period;
	unsigned int pid;
};

unsigned int g_led_no = 0;

int led_open (struct inode *inode, struct file *filp)
{
	struct led_dev_data *t_led_devp;
	
	printk(KERN_WARNING "[led_DD]open..\n");
	if(!try_module_get(THIS_MODULE)) return -ENODEV;

	if(!(t_led_devp = kzalloc(sizeof(struct led_dev_data), GFP_KERNEL))) {
		printk("[led_DD]kzalloc error!!\n");
		return -1;
	}
	switch(g_led_no) {
	  case 0 : t_led_devp->led_no = GPIO_LED0;  break;
	  case 1 : t_led_devp->led_no = GPIO_LED1;  break;
	  case 2 : t_led_devp->led_no = GPIO_LED2;  break;
	}
	t_led_devp->period = 2000*(g_led_no+1);
	t_led_devp->pid = current->pid;
	filp->private_data = t_led_devp;
	if(++g_led_no == 3) g_led_no=0;
	
	printk("[led_DD]open_ok(led-no:%d)..\n", t_led_devp->led_no);
	return 0;
}

int led_release (struct inode *inode, struct file *filp)
{
	struct led_dev_data *t_led_devp;
	
	printk(KERN_WARNING "[led_DD]release..\n");

	t_led_devp = filp->private_data;
	kfree(t_led_devp);

	module_put(THIS_MODULE);
	return 0;
}

ssize_t led_read (struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	int k_data=0;
	int ret;

	printk(KERN_WARNING "[led_DD]read..\n");

	k_data = gpio_get_value(GPIO_LED0);
	ret = copy_to_user(buf, &k_data, count);
	if(ret < 0) return -1;

	printk(KERN_WARNING "[led_DD]read done(%d)..\n", k_data);

	return count;
}


ssize_t led_write (struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
	int k_data;
#if 0	
	int ret;

	printk(KERN_WARNING "[led_DD]write..\n");

	ret = copy_from_user(&k_data, buf, count);
	if(ret < 0) return -1;	

	//write to device
	gpio_set_value(GPIO_LED, k_data);
	printk(KERN_WARNING "[led_DD]write done(%d)..\n", k_data);
#else
	struct led_dev_data *t_led_devp;

	t_led_devp = filp->private_data;
	printk("[led_DD]write_on(%d_%d)..\n", t_led_devp->pid, t_led_devp->led_no);
	k_data = 1;
	gpio_set_value(t_led_devp->led_no, k_data);
	msleep(t_led_devp->period);
	t_led_devp = filp->private_data;
	printk("[led_DD]write_off(%d_%d)..\n", t_led_devp->pid, t_led_devp->led_no);
	k_data = 0;
	gpio_set_value(t_led_devp->led_no, k_data);
#endif
	

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
//    ioctl:		led_ioctl,
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
	
	gpio_request(GPIO_LED0, "LED0");
	gpio_direction_output(GPIO_LED0, 0);
	gpio_request(GPIO_LED1, "LED1");
	gpio_direction_output(GPIO_LED1, 0);
	gpio_request(GPIO_LED2, "LED2");
	gpio_direction_output(GPIO_LED2, 0);

	printk(KERN_WARNING "LED Module Insert Done!!\n");
	printk("You must execute next commands...\n");
	printk("mknod /dev/led c %d %d\n", led_major, led_minor);
	printk("chmod 666 /dev/led\n");	
	
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
	
	cdev_del(&led_cdev);
	unregister_chrdev_region(led_dev, 1);	

	printk(KERN_WARNING "LED Module Delete Done!!\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("Dual BSD/GPL");
