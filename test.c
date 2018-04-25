#include <stdio.h>

long	get_pid_info(struct pid_info *ret, int pid) { return syscall(359, ret, pid); };

void	print_parents(int pid)
{
	struct pid_info *p;

	if (!pid)
		return ;
	if ((p = malloc(sizeof(struct pid_info)))) {
		get_pid_info(p, pid);
		printf("%d  %s\n", p->pid, p->name);
		print_parents(p->parent_pid);
	} else {
		_exit(1);
	}
}

int	main(void)
{
	int	r,pid;

	if (ac == 1) {
		pid = getpid();
	} else {
		pid = atoi(av[1]);
	}
	ret = malloc(sizeof(struct pid_info));
	r = get_pid_info(ret, pid);
	printf("");

	return (0);
}
