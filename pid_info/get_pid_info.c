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

struct pid_info {
	size_t		s_name;
	char		name[PATH_SIZE + 1];
	int		pid;	
	int		parent;
	void 		*stack;
	long		state;
	u64		start_time;
	size_t		nbr_child;
	short int	children[100];
	size_t		s_root;
	char		root[PATH_SIZE + 1];
	size_t		s_pwd;
	char		pwd[PATH_SIZE + 1];
};

/* could you read _user buffer safely ? */
/* nope, need to use copy_*_user family */

SYSCALL_DEFINE2(get_pid_info, struct pid_info __user *, info, int, pid)
{
	ssize_t			retval = 0;
	struct pid		*p;
	struct task_struct	*task;
	struct task_struct	*child;
	struct pid_info		wrap;
	char			buffpath[PATH_SIZE + 1];
	char			*path; 
	size_t			s_tmp;
	size_t			s_child = 0;

	p = find_get_pid(pid);
	/* test null ptr passing */
	if (!(task = pid_task(p, PIDTYPE_PID))) {
		retval = -ESRCH;
		goto out;
	}


	if (info->s_name == 0)
	{
		task_lock(task);
		s_tmp = strlen(task_comm) + 1;
		if (copy_to_user(info->s_name, &s_tmp, s_tmp))
			goto eval_err;
		memset(buffpath, 0, PATH_SIZE + 1);
		path = dentry_path_raw(task->fs->root.dentry, buffpath, PATH_SIZE + 1);
		s_tmp = strlen(path) + 1;
		if (copy_to_user(info->s_root, &s_tmp, s_tmp))
			goto eval_err;
		memset(buffpath, 0, PATH_SIZE + 1);
		path = dentry_path_raw(task->fs->pwd.dentry, buffpath, PATH_SIZE + 1);
		s_tmp = strlen(path) + 1;
		if (copy_to_user(info->s_pwd, &s_tmp, s_tmp))
			goto eval_err;
		list_for_each(child, &task->children, sibling) {
			s_tmp++;
		}
		s_tmp = sizeof(short int) * s_tmp;
		if (copy_to_user(info->nbr_child, &s_tmp, s_tmp))
			goto eval_err;
		task_unlock(task);
	}
	else {	 
		task_lock(task);
		memset(wrap.name, 0, info->s_name);
		strncpy(wrap.name, task->comm, info->s_name);
		wrap.pid = task->pid;
		wrap.parent = task->parent->pid;
		wrap.stack = task->stack;
		wrap.state = task->state;
		wrap.start_time = task->start_time;
		memset(wrap.children, 0, info->nbr_child);
		s_tmp = 0;
		list_for_each_entry(child, &task->children, sibling) {
			/* inaccurate if number of childs change during the two syscalls */
			if (s_tmp > info->nbr_child)
				break;
			wrap.children[s_tmp++] = child->pid;
		}
		memset(wrap.root, 0, info->root);
		memset(buffpath, 0, info->root);
		path = dentry_path_raw(task->fs->root.dentry, buffpath, info->root);
		strncpy(wrap.root, path, info->root);
		memset(wrap.pwd, 0, info->pwd);
		memset(buffpath, 0, info->pwd);
		path = dentry_path_raw(task->fs->pwd.dentry, buffpath, info->pwd);
		strncpy(wrap.pwd, path, info->pwd);
		task_unlock(task);

		if ((copy_to_user(info, &wrap, sizeof(struct pid_info)))) {
			retval = -EFAULT;
			printk(KERN_INFO "copy to user fail");
			goto out;
		}
	}

	return retval;

out:
	return retval;

eval_err:
	task_unlock(task);
	retval = -EINVAL;
	return retval;
}
