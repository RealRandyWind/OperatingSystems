#include <sys/wait.h>
#include <string.h>
#include "misc.h"

#define INPUT_LIMIT_SIZE 4096
#define FAILD_FORK_DS "pid(%d) - faild to fork for %s\n"
#define INVALID_CMD_DSD "pid(%d) - invalid command %s of action %d\n"

typedef cmd_t { char *alias, **args, *_d; int act; } cmd_t;

int input(cmd_t *pcmd, int *pRUN)
{
	char buff[INPUT_LIMIT_SIZE];

	return failure;
}

int eval(const cmd_t cmd, int *pRUN)
{
	execvpe(cmd.alias, cmd.args, envp);
	return EXIT_FAILURE;
}

int main(int argc, char **argv, char **envp)
{
	int pid0, pid, RUN, PARENT;
	cmd_t cmd;

	pid0 = pid = getpid(); RUN = true;
	while(RUN)
	{
		if(pid != pid0) { return eval(cmd, &RUN); }
		else if(input(&cmd, &RUN) < 0) { continue; }
		else if((PARENT = fork()) < 0) { printf(FAILD_FORK_DS, pid, cmd.alias); }
		else { pid = getpid(); }
	}
	wait(null);

	return EXIT_SUCCESS;
}
