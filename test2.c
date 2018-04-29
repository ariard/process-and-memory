#include <sys/syscall.h>
#include <unistd.h>

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


int	main(void)
{
	struct pid_info	ret;
	
	syscall(333, ret, 1);
	return (0);
}
