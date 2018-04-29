#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct pid_info {
	int		pid;
	int		parent;
	void		*stack;
	long		state;
	unsigned long	start_time;
	int		*children;
	char		*root;
	char		*pwd;
};

long	get_pid_info(struct pid_info *ret, int pid) { return syscall(333, ret, pid); };

int	main(int argc, char **argv)
{
	struct pid_info		*ret;
	int			pid;	
	
	if (argc == 1) {
		pid = getpid();
	} else {
		pid = atoi(argv[1]);
	}
	
	ret = malloc(sizeof(struct pid_info));
	get_pid_info(ret, pid);
	printf("pid %d\n", ret->pid);
	printf("parent %d\n", ret->parent);
	printf("stack %p\n", ret->stack);
	printf("state %d\n", ret->state);
	return (0);
}
