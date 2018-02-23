#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "misc.h"

#define DELIM_SPACE " \t\n\r\v\f"
#define ARGS_INIT_SIZE 8
#define INVALID_COMMAND_SS "%s: command %s not found\n"

int execvpe(const char *path, char * const argv[], char *const envp[]);

typedef struct args_t { char **d, *_d; } args_t;

args_t makeargs(const char *src)
{
	args_t args;
	int N, _N;
	
	if(!src || !*src) { exit(EXIT_FAILURE); }
	
	N = 0; _N = ARGS_INIT_SIZE;
	args._d = cp0len(nullptr, src);
	args.d = pchar_s(_N);
	args.d[N] = strtok(args._d, DELIM_SPACE);

	while(args.d[N++])
	{
		if(N >= _N) { args.d = pchar2_s(args.d, _N >>= 1); }
		args.d[N] = strtok(nullptr, DELIM_SPACE);
	}
	return args;
}

int main(int argc, char **argv, char **envp)
{
	args_t args;
	int pid;

	if(argc != 2) { return EXIT_FAILURE; }
	
	args = makeargs(argv[1]);
	pid = fork();
	
	if(args.d[0] && pid)
	{
		execvpe(args.d[0], args.d, envp);
		printf(INVALID_COMMAND_SS, argv[0], args.d[0]);
		return EXIT_FAILURE;
	}
	
	wait(nullptr);
	free_s(args.d);
	free_s(args._d);

	return EXIT_SUCCESS;
}