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
#include <linux/slab.h>
#include <linux/fs_struct.h>
#include <linux/list.h>

/* undervalue PATH_MAX ? */

# define PATH_SIZE	255

struct pid_info {
	char		name[PATH_SIZE + 1];
	int		pid;	
	int		parent;
	void 		*stack;
	long		state;
	u64		start_time;
	short int	children[100];
	char		root[PATH_SIZE + 1];
	char		pwd[PATH_SIZE + 1];
};

SYSCALL_DEFINE2(get_pid_info, struct pid_info __user *, info, int, pid)
{
	ssize_t			retval = 0;
	struct pid		*p;
	struct task_struct	*task;
	struct task_struct	*child;
	struct pid_info		tmp;
	char			buffpath[PATH_SIZE + 1];
	char			*path; 
	size_t			s_child = 0;

	p = find_get_pid(pid);
	/* test null ptr passing */
	if (!(task = pid_task(p, PIDTYPE_PID))) {
		retval = -ESRCH;
		goto out;
	}

	/* allocate memory with kmalloc(GFP_USER) ? */
	
	task_lock(task);
	memset(tmp.name, 0, PATH_SIZE + 1);
	strncpy(tmp.name, task->comm, PATH_SIZE + 1);
	tmp.pid = task->pid;
	tmp.parent = task->parent->pid;
	tmp.stack = task->stack;
	tmp.state = task->state;
	tmp.start_time = task->start_time;
	memset(tmp.children, 0, sizeof(short int) * 100);
	s_child = 0;
	list_for_each_entry(child, &task->children, sibling) {
		if (s_child > 100)
			break;
		tmp.children[s_child++] = child->pid;
	}
	memset(tmp.root, 0, PATH_SIZE + 1);
	memset(buffpath, 0, PATH_SIZE + 1);
	path = dentry_path_raw(task->fs->root.dentry, buffpath, PATH_SIZE + 1);
	strncpy(tmp.root, path, PATH_SIZE + 1);
	memset(tmp.pwd, 0, PATH_SIZE + 1);
	memset(buffpath, 0, PATH_SIZE + 1);
	path = dentry_path_raw(task->fs->pwd.dentry, buffpath, PATH_SIZE + 1);
	strncpy(tmp.pwd, path, PATH_SIZE + 1);
	task_unlock(task);

	if ((copy_to_user(info, &tmp, sizeof(struct pid_info)))) {
		retval = -EFAULT;
		printk(KERN_INFO "copy to user fail");
		goto out;
	}

	printk(KERN_INFO "calling sys_get_pid_info");

	return retval;
out:
	return retval;
}
