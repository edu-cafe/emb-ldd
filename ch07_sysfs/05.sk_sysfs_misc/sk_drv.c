#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/uaccess.h>		//copy_from_user(). copy_to_user()
#include <linux/slab.h>			//kmalloc

#include "sk.h"

#include <linux/miscdevice.h>

int sk_major=0, sk_minor=0;

int sk_open (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[SK_DD]open..\n");
	
	return 0;
}

int sk_release (struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "[SK_DD]release..\n");
	return 0;
}

ssize_t sk_read (struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	char *k_data;
	int ret;
	int i;

	printk(KERN_WARNING "[SK_DD]read..\n");

	k_data = kmalloc(count+1, GFP_KERNEL);
	if(k_data==NULL) return -1;

	//read from device
	for(i=0; i<count; i++) k_data[i] = 'A'+i;
	k_data[i] = '\0';

	ret = copy_to_user(buf, k_data, count);
	if(ret < 0) return -1;

	printk(KERN_WARNING "[SK_DD]read done(%s)..\n", k_data);

	return count;
}


ssize_t sk_write (struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	char *k_data;
	int ret;

	printk(KERN_WARNING "[SK_DD]write..\n");

	k_data = kmalloc(count+1, GFP_KERNEL);
	if(k_data==NULL) return -1;

	ret = copy_from_user(k_data, buf, count);
	if(ret < 0) return -1;
	k_data[count] = '\0';

	//write to device
	printk(KERN_WARNING "[SK_DD]write done(%s)..\n", k_data);


	return count;
}


//int sk_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
long sk_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	SK_INFO	*info = (SK_INFO*)arg;
	int size;
	int err;

	printk(KERN_WARNING "[SK_DD]ioctl(cmd:%x)..\n", cmd);

	if(_IOC_TYPE(cmd) != SK_MAGIC) return -EINVAL;
	if(_IOC_NR(cmd) > SK_MAXNR) return -EINVAL;

	size = _IOC_SIZE(cmd);
	if(size) {
		err = 0;
		if(_IOC_DIR(cmd) & _IOC_READ) {
			//err = verify_area(VERIFY_READ, (void*)arg, size);
			err = access_ok(VERIFY_READ, (void*)arg, size);
		} else if(_IOC_DIR(cmd) & _IOC_WRITE) {
			//err = verify_area(VERIFY_WRITE, (void*)arg, size);
			err = access_ok(VERIFY_WRITE, (void*)arg, size);
		}
		if(!err) return -1;
	}

	switch(cmd) {
	case SK_LED_OFF :
		printk("[SK_DD] ioctl => SK_LED_OFF..\n");
		break;
	case SK_LED_ON :
		printk("[SK_DD] ioctl => SK_LED_ON..\n");
		break;
	case SK_GETSTATE :
		printk("[SK_DD] ioctl => SK_GETSTATE..\n");
		break;
	case SK_READ :
		printk("[SK_DD] ioctl => SK_READ..\n");
		sk_read(filp, info->buff, info->size, NULL);
		break;
	case SK_WRITE :
		printk("[SK_DD] ioctl => SK_WRITE(size:%d, msg:%s)..\n", info->size, info->buff);
		sk_write(filp, info->buff, info->size, NULL);

		break;
	case SK_RW :
		printk("[SK_DD] ioctl => SK_RW..\n");
		break;
	}
	return 0;
}

struct file_operations sk_fops = {
    read:       sk_read,
    write:      sk_write,
    open:       sk_open,
//    ioctl:		sk_ioctl,
    unlocked_ioctl:		sk_ioctl,
    release:    sk_release
};

struct miscdevice sk_miscdev = {
	.name		= "sk0",
//	.minor		= 253,
	.minor		= MISC_DYNAMIC_MINOR,
	.fops		= &sk_fops,
};

int sk_init(void)
{
	int result;

	printk("<1> SK Module id Up....\n");

	result = misc_register(&sk_miscdev);
	if(result < 0) {
		printk(KERN_WARNING "sk:misc_register Fail!!\n");
		return result;
	}

	printk(KERN_WARNING "Module(sk) Register OK!!\n");

	printk(KERN_WARNING "SK Module Insert Done!!\n");
	return 0;
}

void sk_exit(void)
{
	printk("<1> SK Module id Down....\n");

	misc_deregister(&sk_miscdev);

	printk(KERN_WARNING "SK Module Delete Done!!\n");
}

module_init(sk_init);
module_exit(sk_exit);

MODULE_LICENSE("Dual BSD/GPL");
