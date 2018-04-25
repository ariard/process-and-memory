#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <asm/current.h>

static ssize_t	showpid_read(struct file *filp, char __user *buffer,
		size_t length, loff_t *offset)
{
	ssize_t			retval = 0;
	struct task_struct	*tmp;
		
	list_for_each_entry(tmp, &current->tasks, tasks) {
		printk(KERN_INFO "thread id %d\n", tmp->pid);
	}
		
	return retval;
}

static struct file_operations showpid_fops = {
	.read = showpid_read,
};

static struct miscdevice showpid_driver = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "showpid",
	.fops = &showpid_fops,
};

static int __init showpid_init(void)
{
	int	result;

	result = misc_register(&showpid_driver);
	if (result)
		printk(KERN_INFO "show_pid module : failed with %d\n", result);
	else
		printk(KERN_INFO "show_pid module : successful\n");

	return result;
}

static void  __exit showpid_cleanup(void)
{
	misc_deregister(&showpid_driver);
	printk(KERN_INFO "show_pid module\n");
}

module_init(showpid_init);
module_exit(showpid_cleanup);
