#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "misc.h"

#define DELIM_SPACE " \t\n\r\v\f"

int execvpe(const char *path, char * const argv[], char *const envp[]);

typedef struct args_t { char **d, *_d; } args_t;

args_t makeargs(const char *src)
{
	args_t args;
	char* token;
	args.N = args._N = 0; args.d = nullptr; args._d = nullptr;
	if(!src || !*src) { return args; }
	args._d = cp0len2(nullptr, src, &args._N);
	args.d = pchar(1);
	args.d[0] 
	while()
	return args;
}

int main(int argc, char **argv, char **envp)
{
	args_t args;

	if(argc != 2) { return EXIT_FAILURE; }
	args = makeargs(argv[1]);
	/* for(int n = 0; n < args.N; ++n) { printf("[%d]  \"%s\"\n", n, args.d[n]); } */
	execvpe(args.d[0], args.d, envp);

	free(args._d);
	free(args.d);

	return EXIT_FAILURE;
}