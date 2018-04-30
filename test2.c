#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

# define PATH_SIZE	255

struct pid_info {
	char		name[PATH_SIZE + 1];
	int		pid;
	int		parent;
	void 		*stack;
	long		state;
	long unsigned	start_time;
	short int	children[100];
	char		root[PATH_SIZE + 1];
	char		pwd[PATH_SIZE + 1];
};

long	get_pid_info(struct pid_info *ret, int pid) { return syscall(333, ret, pid); };

int	main(int argc, char **argv)
{
	struct pid_info		*ret;
	int			pid;	
	int			i;
	
	if (argc == 1) {
		pid = getpid();
	} else {
		pid = atoi(argv[1]);
	}
	ret = malloc(sizeof(struct pid_info));
	get_pid_info(ret, pid);
	printf("name %s\n", ret->name);
	printf("pid %d\n", ret->pid);
	printf("parent %d\n", ret->parent);
	printf("stack %p\n", ret->stack);
	printf("state %d\n", ret->state);
	i = 0;
	while (ret->children[i])
		printf("%d\n", ret->children[i++]);
	printf("root %s\n", ret->root);
	printf("pwd %s\n", ret->pwd);
	return (0);
}
