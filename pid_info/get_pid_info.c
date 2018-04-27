#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE0(get_pid_info)
{
	printk(KERN_INFO "hello world fron get_pid_info");
}
