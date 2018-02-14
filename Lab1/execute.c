#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define nullptr 0
#define nullstr ""
#define nullchr '\0'
#define true 1
#define false 0


inline void* malloc_s(void* d, int sz) { if(!(d=malloc(sz))) { exit(EXIT_FAILURE); } return d; }

inline int isnull(void* d) { return !d; }
inline int iseol(char chr) { return chr == '\r' || '\n'; }
inline int iseof(char chr) { return chr == nullchr; }
inline int isspace(char chr) { return chr == ' ' || chr == '\n' || chr == '\r' || chr == '\t' || chr == '\v'; }

typedef struct args_t
{
	int N, _N;
	char **d;
} args_t;

args_t raw_args(const char *str)
{
	int n, k;
	char chr;
	args_t args;

	args._N = 1; args.N = 0;
	agrs.d = (char *) malloc_s(sizeof(char));

	if(isnull(str)) { return agrs; }
	
	for(k = 0; chr = str[k]; ++k)
	{
		if(isspace(chr)) { continue; }
		while(chr)
		{

		}
	}

	return args;
}

int main(int argc, const char *argv[], const char *envv[])
{
	if(argc <= 0) { return EXIT_SUCCESS; }
	arg
	if(execve(args.d[0], &agrs.d[1], envv)) { return EXIT_FAILURE; };

	free(agrs.d)

	return EXIT_SUCCESS;
}