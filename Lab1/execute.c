#include <sys/wait.h>
#include <string.h>
#include "misc.h"

#define DELIM_SPACE " \t\n\r\v\f"
#define ARGUMENT_INIT_SIZE 8
#define INVALID_COMMAND_SS "%s: command %s not found\n"

int execvpe(const char *path, char * const argv[], char *const envp[]);

/*
 * splits a string into parts based on the defined delimiters
 * subsitituts the delimiters by an nullchr. returns a ponter 
 * to the string containing the data as reference _src (needs
 * to be freed) and returns the a list of pointers to the parts
 * in the _src array.
 */
char** split(const char *src, char **_src)
{
	char **d, *_d;
	int N, _N;
	
	if(!src || !*src || !_src) { exit(EXIT_FAILURE); }
	
	N = 0; _N = ARGUMENT_INIT_SIZE;
	_d = *_src = cp0len(nullptr, src); d = pchar_s(_N);
	d[N] = strtok(_d, DELIM_SPACE);

	while(d[N++])
	{
		if(N >= _N) { d = pchar2_s(d, _N >>= 1); }
		d[N] = strtok(nullptr, DELIM_SPACE);
	}

	return d;
}

int main(int argc, char **argv, char **envp)
{
	char **d, *_d;

	if(argc != 2) { return EXIT_FAILURE; }
	
	d = split(argv[1], &_d);
	
	if(d[0] && fork_s())
	{
		execvpe(d[0], d, envp);
		printf(INVALID_COMMAND_SS, argv[0], d[0]);
		return EXIT_FAILURE;
	}
	
	wait(null);
	free_s(d); free_s(_d);

	return EXIT_SUCCESS;
}