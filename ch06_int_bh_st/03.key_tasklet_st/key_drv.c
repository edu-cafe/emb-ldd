#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/uaccess.h>		//copy_from_user(). copy_to_user()
#include <linux/slab.h>			//kmalloc
#include <linux/cdev.h>			// struct cdev, ..
#include <linux/gpio.h>			//GPIO_BASE macro

#include <linux/wait.h>
#include <linux/interrupt.h>

//#include <linux/workqueue.h>

#define	GPIO_KEY	25

int key_major=0, key_minor=0;
dev_t key_dev;
struct cdev key_cdev;

volatile unsigned int flag_keyInt=0;
DECLARE_WAIT_QUEUE_HEAD(wq_key);
unsigned int irq_key;

//DECLARE_WORK(my_wq, func_workqueue, (void*)5);
//void func_workqueue(struct work_struct *dummy);
//DECLARE_WORK(my_wq, func_workqueue);

.... func_tasklet....
...............

int key_open (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[key_DD]open..\n");
	
	return 0;
}

int key_release (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[key_DD]release..\n");
	return 0;
}

ssize_t key_read (struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int k_data=0;
	int ret;

	printk(KERN_WARNING "[key_DD]read..\n");
	
#if 0
	while(!flag_keyInt);
#else
	while(!flag_keyInt) {
		if(filp->f_flags & O_NONBLOCK) return -EAGAIN;
			
		printk(KERN_WARNING "[KEY_DD]Waiting key Int..\n");
		//interruptible_sleep_on(&wq_timer);	//old version
		ret = wait_event_interruptible(wq_key, flag_keyInt);
		printk(KERN_WARNING "[KEY_DD]After key Int(ret:%d)..\n", ret);
	}
#endif
	printk(KERN_WARNING "[KEY_DD]After key Int..\n");

	k_data = gpio_get_value(GPIO_KEY);
	ret = copy_to_user(buf, &k_data, count);
	if(ret < 0) return -1;

	flag_keyInt = 0;
	printk(KERN_WARNING "[key_DD]read done(%d)..\n", k_data);

	return count;
}


ssize_t key_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	int k_data;
	int ret;

	printk(KERN_WARNING "[key_DD]write..\n");

	ret = copy_from_user(&k_data, buf, count);
	if(ret < 0) return -1;
	
	gpio_set_value(GPIO_KEY, k_data);
	//if(k_data) gpio_direction_output(GPIO_KEY, 1);
	//else gpio_direction_output(GPIO_KEY, 0);

	//write to device
	printk(KERN_WARNING "[key_DD]write done(%d)..\n", k_data);


	return count;
}

//int key_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
long key_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk(KERN_WARNING "[KEY_DD]ioctl(cmd:%d)..\n", cmd);
	return 0;
}

struct file_operations key_fops = {
    read:       key_read,
    write:      key_write,
    open:       key_open,
//    ioctl:		key_ioctl,
    unlocked_ioctl:		key_ioctl,
    release:    key_release
};

... func_tasklet(.....)
{
	int i;

	printk("*****Tasklet_Task Start!!\n");
	for(i=1; i<(int)data; i++) {
		printk("*****Tasklet_Task(%d)!!\n", i);
		//msleep(1000);
	}
	............
	printk("*****Tasklet_Task End!!\n");
}

irqreturn_t key_interrupt(int irq, void *dev_id)
{
	printk("[KEY_DD]MyKey Int(irq:%d)!!!!\n", irq);
	flag_keyInt = 1;

	...........
	
	printk("[KEY_DD]MyKey Int. End!!!\n");
	return IRQ_HANDLED;
}

int __init mykey_init(void)
{
	int error;

	printk("<1> KEY Module id Up....\n");

	if(key_major) {
		key_dev = MKDEV(key_major, key_minor);
		error = register_chrdev_region(key_dev, 1, "KEY");
	} else {
		error = alloc_chrdev_region(&key_dev, key_minor, 1, "KEY");
		key_major = MAJOR(key_dev);
	}
	if(error < 0) {
		printk(KERN_WARNING "KEY:alloc_major error!!\n");
		return error;
	}

	printk("KEY:alloc_major ok! => major_num:%d\n", key_major);


	// register_chrdev
	cdev_init(&key_cdev, &key_fops);
	key_cdev.owner = THIS_MODULE;
	error = cdev_add(&key_cdev, key_dev, 1);
	if(error < 0) {
		printk(KERN_WARNING "KEY:register_chrdev error!!\n");
		return error;
	}
	
	gpio_request(GPIO_KEY, "KEY");
	gpio_direction_input(GPIO_KEY);
	irq_key = gpio_to_irq(GPIO_KEY);
	irq_set_irq_type(irq_key, IRQ_TYPE_EDGE_FALLING);
	printk(KERN_WARNING "KEY IRQ_no : %d\n", irq_key);
	
#if 1
	//if(request_irq(irq_key, key_interrupt, IRQF_DISABLED, "MyKey", NULL)) {
	if(request_irq(irq_key, key_interrupt, IRQF_SHARED, "MyKey", "mykey")) {
		printk("request_irq fail!!\n");
		return -ENOENT;
    }
#endif
	printk("request_irq ok!!\n");

	printk(KERN_WARNING "KEY Module Insert Done!!\n");
	printk("You must execute next commands...\n");
	printk("mknod /dev/key c %d %d\n", key_major, key_minor);
	printk("chmod 666 /dev/key\n");	
	
	return 0;
}

void __exit mykey_exit(void)
{
	printk("<1> KEY Module id Down....\n");

	free_irq(irq_key, "mykey");
	gpio_set_value(GPIO_KEY, 0);
	gpio_free(GPIO_KEY);
	
	cdev_del(&key_cdev);
	unregister_chrdev_region(key_dev, 1);	

	printk(KERN_WARNING "KEY Module Delete Done!!\n");
}

module_init(mykey_init);
module_exit(mykey_exit);

MODULE_LICENSE("Dual BSD/GPL");
