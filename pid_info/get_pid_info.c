#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/syscalls.h>

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
	printk(KERN_INFO "hello world form get_pid_info");
	return 0; 
}
