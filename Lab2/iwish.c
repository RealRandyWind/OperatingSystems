#include <string.h>
#include "misc.h"

#define QUIT_SZ 6
#define QUIT_CMD "_exit"
#define FAILD_WAIT_D "pid(%d) - faild to wait\n"
#define FAILD_FORK_DS "pid(%d) - faild to fork for %s\n"
#define INVALID_INPUT_D "pid(%d) - invalid input\n"
#define INVALID_CMD_DSS "pid(%d) - invalid command %s of action %d\n"

#include "iwish.incl"

static cur_t _cur;

typedef enum cmd_e
{
	CMD_NONE = none,
	CMD_PIPE,
	CMD_BACK,
	CMD_EXEC,
	CMD_IDIR,
	CMD_ODIR,
	_CMD_SZ
} cmd_e;

typedef struct cmd_t { char *alias, **args, *_d; cmd_e type; int fd[2]; struct cmd_t *_next; } cmd_t;

cmd_t rawcmd()
{
	cmd_t cmd;

	cmd._d = nullptr; cmd.args = nullptr;
	cmd.fd[0] = cmd.fd[1]; cmd._next = nullptr;
	return cmd;
}

/* STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO*/
int input(cmd_t *pcmd, int *pBLOCK, int *pRUN)
{
	cmd_t cmd;

	if(!pcmd || !pBLOCK || !pRUN) { exit(EXIT_FAILURE); }
	cmd = *pcmd; free(cmd.args); free(cmd._d);

	*pRUN = false;

	while(_cur.N = read_s(_cur.fd, _cur.d, _BUF_SZ))
	{
		if(_cur.N == QUIT_SZ && eq(_cur.d, QUIT_CMD, QUIT_SZ - 1)) { return failure; }
		_cur.n = 0;
		while(parse(&_cur) && !_cur.REJECT)
		{
			printf("   (%c, %d, %s, %s)\n", _cur.c, _cur.t, SBOOL(_cur.ACCEPT), SBOOL(_cur.REJECT));
		}	
		printf("%.*s",_cur.N, _cur.d);
	}
	return failure;
}

int eval(const cmd_t cmd, int *pRUN, char **envp)
{
	execvpe(cmd.alias, cmd.args, envp);
	return failure;
}

int main(int argc, char **argv, char **envp)
{
	int pid0, pid, RUN, BLOCK;
	cmd_t cmd, pcmd;

	/* initialize main variable */
	_cur = rawcur(STDIN_FILENO); cmd = rawcmd();
	pid0 = pid = getpid(); RUN = true; BLOCK = false;
	/* enter main loop */
	while(RUN)
	{
		/* check if root process or sub process */
		if(pid != pid0)
		{
			/* any sub proccess will evaluate command */
			eval(cmd, &RUN, envp);
			printf(INVALID_CMD_DSS, pid, cmd.alias, cmd.type);
			free(cmd.args); free(cmd._d); cmd = *cmd._next;
			return EXIT_FAILURE;
		}
		/* block can only be true on successfull fork */
		if (BLOCK && wait(null) > 0) { BLOCK = false; printf(FAILD_WAIT_D, pid); continue; }
		/* read command from input */	
		if (input(&cmd, &BLOCK, &RUN) < 0) { printf(INVALID_INPUT_D, pid); continue; }
		/* fork process for command execution */
		if (fork() < 0) { BLOCK = false; printf(FAILD_FORK_DS, pid, cmd.alias); continue; }
		pid = getpid(); 
	}
	wait_s(null);

	return EXIT_SUCCESS;
}
