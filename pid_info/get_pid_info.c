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
	char		*name;
	int		pid;	
	int		parent;
	void 		*stack;
	long		state;
	u64		start_time;
	size_t		s_child;
	short int	*children;
	size_t		s_root;
	char		*root;
	size_t		s_pwd;
	char		*pwd;
};

SYSCALL_DEFINE2(get_pid_info, struct pid_info __user *, info, int, pid)
{
	ssize_t			retval = 0;
	struct pid		*p;
	struct task_struct	*task;
	struct task_struct	*child;
	struct pid_info		wrap;
	char			buffpath[PATH_MAX + 1];
	char			*path; 
	size_t			usize = 0;
	size_t			ksize = 0;
	size_t			i = 0;

	p = find_get_pid(pid);
	/* test null ptr passing */
	if (!(task = pid_task(p, PIDTYPE_PID))) {
		retval = -ESRCH;
		goto out;
	}

	printk("flag A\n");

	wrap.name = NULL;
	wrap.pwd = NULL;
	wrap.root = NULL;
	wrap.children = NULL;
	wrap.pid = task->pid;
	wrap.parent = task->parent->pid;
	wrap.stack = task->stack;
	wrap.state = task->state;
	wrap.start_time = task->start_time;

	if ((copy_to_user(info, &wrap, sizeof(struct pid_info)))) {
		retval = -EFAULT;
		goto err;
	}

	printk("flag B\n");
	/* does it better to lock/unlock at every step */
	task_lock(task);
	if (copy_from_user(&usize, &info->name, sizeof(size_t)))
		goto err;
	ksize = strlen(task->comm);
	if (ksize > usize) {
		if (copy_to_user(&info->s_name, &ksize, sizeof(size_t)))
			goto err;
		goto too_small;
	}
	else if (copy_to_user(info->name, task->comm, strlen(task->comm)))
		goto err;

	printk("flag C\n");
	if (copy_from_user(&usize, &info->s_child, sizeof(size_t)))
		goto err;
	list_for_each_entry(child, &task->children, sibling) {
		/* doesn t matter that the size isn't accurate, number of children is dynamic */
		if (i > usize) {
			if (copy_to_user(&info->s_child, &i, sizeof(size_t)))
				goto too_small;
		}
		else if (copy_to_user(info->children, &child->pid, sizeof(short int)))
			goto err;
		i++;	
	}

	printk("flag D\n");
	if (copy_from_user(&usize, &info->s_root, sizeof(size_t)))
		goto err;
	memset(buffpath, 0, PATH_MAX + 1);
	path = dentry_path_raw(task->fs->root.dentry, buffpath, PATH_MAX + 1);
	ksize = strlen(path);
	if (ksize > usize) {
		if (copy_to_user(&info->s_root, &ksize, sizeof(size_t)))
			goto err;
		goto too_small;
	}
	else if (copy_to_user(info->root, path, usize))
		goto err;

	printk("flag E\n");
	if (copy_from_user(&usize, &info->s_pwd, sizeof(size_t)))
		goto err;
	memset(buffpath, 0, PATH_MAX + 1);
	path = dentry_path_raw(task->fs->pwd.dentry, buffpath, PATH_MAX + 1);
	ksize = strlen(path);
	if (ksize > usize) {
		if (copy_to_user(&info->s_pwd, &ksize, sizeof(size_t)))
			goto err;
		goto too_small;
	}
	else if (copy_to_user(info->pwd, path, usize))
		goto err;

	task_unlock(task);

	return retval;
out:
	return retval;

err:
	task_unlock(task);
	retval = -EFAULT;
	return retval;

too_small:
	task_unlock(task);
	retval = -EINVAL;
	return retval;
}
