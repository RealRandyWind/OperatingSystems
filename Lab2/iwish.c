#include <string.h>
#include "misc.h"

#define INPUT_LIMIT_SIZE 4096
#define FAILD_FORK_DS "pid(%d) - faild to fork for %s\n"
#define INVALID_INPUT_D "pid(%d) - invalid input\n"
#define INVALID_CMD_DSS "pid(%d) - invalid command %s of action %s\n"

typedef enum cmd_e
{
	CMD_NONE = none,
	CMD_PIPE,
	CMD_BACK,
	CMD_EXEC,
	CMD_IDIR,
	CMD_ODIR,
	_CMD_SZE
} cmd_e;

const char _cmdstr_m[_CMD_SZE][5] = { "NONE", "PIPE", "BACK", "EXEC", "IDIR", "ODIR" };
const char* SCMD_E(cmd_e type) { return type < 0 || type >= _CMD_SZE ? "INVALID" : _cmdstr_m[(int)type]; }

typedef struct cmd_t { char *alias, **args, *_d; cmd_e type; } cmd_t;

int input(cmd_t *pcmd, int *pRUN)
{
	cmd_t cmd;
	char buff[INPUT_LIMIT_SIZE];

	if(!pcmd) { exit(EXIT_FAILURE); }
	cmd = *pcmd;
	


	*pcmd = cmd;
	return failure;
}

int eval(const cmd_t cmd, int *pRUN, char **envp)
{
	execvpe(cmd.alias, cmd.args, envp);
	
	*pRUN = false;
	return failure;
}

int main(int argc, char **argv, char **envp)
{
	int pid0, pid, RUN;
	cmd_t cmd;

	pid0 = pid = getpid(); RUN = true;
	while(RUN)
	{
		RUN = false;
		if(pid != pid0)
		{
			eval(cmd, &RUN, envp);
			printf(INVALID_CMD_DSS, pid, cmd.alias, SCMD_E(cmd.type));
			free_s(cmd.args); free_s(cmd._d);
			return EXIT_FAILURE;
		}
		if(input(&cmd, &RUN) < 0) { printf(INVALID_INPUT_D, pid); continue; }
		if(fork() < 0) { printf(FAILD_FORK_DS, pid, cmd.alias); continue; }
		pid = getpid();
	}
	wait(null);

	return EXIT_SUCCESS;
}
