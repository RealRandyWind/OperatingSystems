#include <sys/wait.h>
#include <string.h>
#include "misc.h"

#define INPUT_LIMIT_SIZE 4096
#define FAILD_FORK_DS "pid(%d) - faild to fork for %s\n"
#define FAILD_INPUT_D "pid(%d) - faild to take input\n"
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
	printf(INVALID_CMD_DSD, pid, cmd.alias, cmd.act);
	return failure;
}

int main(int argc, char **argv, char **envp)
{
	int pid0, pid, RUN;
	cmd_t cmd;

	pid0 = pid = getpid(); RUN = true;
	while(RUN)
	{
		if(pid != pid0)
		{
			eval(cmd, &RUN);
			printf(INVALID_CMD_DSD, pid, cmd.alias, cmd.act);
			return EXIT_FAILURE;
		}
		if(input(&cmd, &RUN) < 0) { printf(FAILD_INPUT_D, pid); continue; }
		if(fork() < 0) { printf(FAILD_FORK_DS, pid, cmd.alias); continue; }
		id = getpid();
	}
	wait(null);

	return EXIT_SUCCESS;
}
