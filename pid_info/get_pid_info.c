#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/fs.h>
#include <linux/pid.h>
#include <linux/uaccess.h>
#include <linux/path.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/dcache.h>

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

SYSCALL_DEFINE2(get_pid_info, struct pid_info __user *, info, int, pid)
{
	ssize_t			retval = 0;
	ssize_t			r = 0;
	struct pid		*p;
	struct task_struct	*task;
	struct pid_info		tmp;
/*	char			fullpath[512]; */

	p = find_get_pid(pid);
	task = pid_task(p, PIDTYPE_PID);
	if (task) {
		tmp.pid = task->pid;
		tmp.parent = task->parent->pid;
		tmp.stack = task->stack;
		tmp.state = task->state;
		tmp.start_time = task->start_time;
	}
	if ((r = copy_to_user(&tmp, info, sizeof(struct pid_info)))) {
		retval = -EFAULT;
		printk(KERN_INFO "copy to user fail with %ld\n", r);
		goto out;
	}

	printk(KERN_INFO "calling sys_get_pid_info");
	return retval;
out:
	return retval;
}
