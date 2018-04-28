#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <asm/current.h>

struct pid_info {
	int		pid;	
	int		parent;
	void 		*stack;
	long		state;
	u64		start_time;
	int		*children;	
	char		*root;
	char		*pwd;
};

static ssize_t	pid_info_read(struct file *filp, char __user *buffer,
		size_t length, loff_t *offset)
{
	ssize_t			retval = 0;
	struct pid		*pid;
	struct task_struct	*task;
		
	pid = find_get_pid(1);	
	task = pid_task(pid, PIDTYPE_ID);
	if (task) {
		printk("PID : %d\n", task->pid);
		
	return retval;
err:
	return retval;
}

static struct file_operations pid_info_fops = {
	.read = pid_info_read,
};

static struct miscdevice pid_info_driver = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "pid_info",
	.fops = &pid_info_fops,
};

static int __init pid_info_init(void)
{
	int	result;

	result = misc_register(&pid_info_driver);
	if (result)
		printk(KERN_INFO "show_pid module : failed with %d\n", result);
	else
		printk(KERN_INFO "show_pid module : successful\n");

	return result;
}

static void  __exit pid_info_cleanup(void)
{
	misc_deregister(&pid_info_driver);
	printk(KERN_INFO "show_pid module\n");
}

module_init(pid_info_init);
module_exit(pid_info_cleanup);

/*	struct task_struct	*tmp;

	list_for_each_entry(tmp, &current->tasks, tasks) {
		printk(KERN_INFO "thread id %d\n", tmp->pid);
	}
*/
