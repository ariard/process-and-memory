#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/fs_struct.h>
#include <linux/uaccess.h>
#include <linux/path.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/dcache.h>

MODULE_LICENSE("GPL");

static int 	query_pid = 0;

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
	char			fullpath[512];
		
	pid = find_get_pid(query_pid);
	task = pid_task(pid, PIDTYPE_PID);
	if (task) {
		printk("name : %s\n", task->comm);
		printk("PID : %d\n", task->pid);
		printk("parent : %d\n", task->parent->pid);
		printk("stack : %p\n", task->stack);
		printk("state : %ld\n", task->state);
		printk("start_time : %llu\n", task->start_time);
		memset(fullpath, 0, 512);
		printk("root path : %s\n", dentry_path_raw(task->fs->root.dentry, fullpath, 512));
		memset(fullpath, 0, 512);
		printk("pwd path : %s\n", dentry_path_raw(task->fs->pwd.dentry, fullpath, 512)); 
	}
	/* copy all data */
	/* how to copy list ? */
	/* how to RO address stack */

	/* copy_to_user(buffer, &task, sizeof(pid_info) */
	return retval;
}

static ssize_t	pid_info_write(struct file *filp, const char __user *buffer,
		size_t length, loff_t *offset)
{
	ssize_t		retval = 0;
	char		wr_buf[8];

	memset(wr_buf, 0, 8);
	length = (length > 8) ? 8 : length;
	if (copy_from_user(wr_buf, buffer, length)) {
		retval = -EFAULT;
		goto out;
	}
	else
		retval = length;
	kstrtouint(wr_buf, 10, &query_pid);

	return retval;
out:
	return retval;
}

static struct file_operations pid_info_fops = {
	.read = pid_info_read,
	.write = pid_info_write,
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
