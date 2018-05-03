#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

# define PATH_SIZE	512
# define CHILD_SIZE	100
# define TASK_COMM_LEN	16

/* Need to write comment about layer violation, three methods and how-to */

struct pid_info {
	char		name[TASK_COMM_LEN];
	int		pid;
	int		parent;
	void 		*stack;
	long		state;
	long unsigned	start_time;
	size_t		s_child;
	int		*children;
	size_t		s_root;
	char		*root;
	size_t		s_pwd;
	char		*pwd;
};

long	get_pid_info(struct pid_info *ret, int pid) { return syscall(333, ret, pid); };

void	show_pid_info(int pid)
{
	struct pid_info		ret;
	int			i = 0;

	if (!pid)
		return ;
	ret.s_child = CHILD_SIZE;
	ret.children = malloc(CHILD_SIZE * sizeof(short int));
	ret.s_root = PATH_SIZE;
	ret.root = malloc(PATH_SIZE);
	ret.s_pwd = PATH_SIZE;
	ret.pwd = malloc(PATH_SIZE);
	if (ret.children && ret.root && ret.pwd) {
		memset(ret.children, 0, CHILD_SIZE * sizeof(int));
		memset(ret.root, 0, PATH_SIZE);
		memset(ret.pwd, 0, PATH_SIZE);
		if (get_pid_info(&ret, pid) == 0) {
			printf("name %s\n", ret.name);
			printf("pid %d\n", ret.pid);
			printf("pid parent %d\n", ret.parent);
			printf("stack %p\n", ret.stack);
			printf("state %d\n", ret.state);
			while (ret.children[i])
				printf("%d\n", ret.children[i++]);
			printf("root %s\n", ret.root);
			printf("pwd %s\n", ret.pwd);
			i = 0;
			printf("\n------\n");
			while (ret.children[i])
				show_pid_info(ret.children[i++]);
		} else {
			printf("Buffers too small..., sizes needed:\n");
			printf("children %ld\n", ret.s_child);
			printf("pwd %ld\n", ret.s_pwd);
			printf("root %ld\n", ret.s_root);
		}
	}
	else {
		exit(1);
	}
}
			


int	main(int argc, char **argv)
{
	struct pid_info		ret;
	int			pid;	
	int			i;
	
	if (argc == 1) {
		pid = getpid();
	} else {
		pid = atoi(argv[1]);
	}
	show_pid_info(pid);
	return (0);
}
