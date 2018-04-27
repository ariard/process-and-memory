#include <linux/syscalls.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

SYSCALL_DEFINE0(get_pid_info)
{
	printk(KERN_INFO "hello world form get_pid_info");
	return 0; 
}
