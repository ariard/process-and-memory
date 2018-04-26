#ifndef PID_INFO_H
#define PID_INFO_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>

asmlinkage long sys_get_pid_info(struct pid_info *ret, int pid);

#endif
