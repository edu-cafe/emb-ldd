#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

int MY_NUM;

char * my_url="www.naver.com";
void my_favorate_url(void)
{
	printk(KERN_INFO "MY favorate URL : %s\n", my_url);
}

int my_init(void)
{
	MY_NUM++;
	printk(KERN_INFO "MY Module_1 is Loaded1(MY_NUM:%d)!! ....\n", MY_NUM);
	my_favorate_url();
	printk(KERN_EMERG "Hi,Hi, I'm my_module....\n");
	return 0;
}

void my_exit(void)
{
	printk(KERN_INFO "MY Module_1 is Unloaded(MY_NUM:%d)....\n", MY_NUM);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("Dual BSD/GPL");
