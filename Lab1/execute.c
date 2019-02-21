#include <sys/wait.h>
#include <string.h>
#include "misc.h"

#define DELIM_SPACE " \t\n\r\v\f"
#define ARGUMENT_INIT_SIZE 16
#define INVALID_COMMAND_SS "%s: command %s not found\n"

int execvpe(const char *file, char *const argv[], char *const envp[]);

/*
 * splits a string into parts based on the defined delimiters
 * subsitituts the delimiters by an nullchr. returns a ponter 
 * to the string containing the data as reference _saferc (needs
 * to be freed) and returns the a list of pointers to the parts
 * in the _saferc array.
 */
char** split(const char *src, char **_saferc)
{
	char **d, *_d;
	int N, _N;
	
	if(!src || !*src || !_saferc) { exit(EXIT_FAILURE); }
	
	N = 0; _N = ARGUMENT_INIT_SIZE;
	_d = *_saferc = (char *) cp0len(nullptr, src); d = pchar_safe(_N);
	d[N] = strtok(_d, DELIM_SPACE);

	while(d[N++])
	{
		if(N >= _N) { d = pchar2_safe(d, _N += ARGUMENT_INIT_SIZE); }
		d[N] = strtok(nullptr, DELIM_SPACE);
	}

	return d;
}

int main(int argc, char **argv, char **envp)
{
	char **d, *_d;

	if(argc != 2) { return EXIT_FAILURE; }
	
	d = split(argv[1], &_d);
	
	if(d[0] && fork_safe())
	{
		execvpe(d[0], d, envp);
		printf(INVALID_COMMAND_SS, argv[0], d[0]);
	}
	
	wait(null);
	free_safe(d); free_safe(_d);

	return EXIT_SUCCESS;
}