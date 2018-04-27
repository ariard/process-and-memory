#include "pid_info.h"

asmlinkage long sys_get_pid_info(struct pid_info *ret, int pid)
{
	printk(KERN_INFO "hello world fron get_pid_info");
}
