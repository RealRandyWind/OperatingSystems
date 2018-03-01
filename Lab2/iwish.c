#include <sys/wait.h>
#include <string.h>
#include "misc.h"

int director();
int worker();

int main(int argc, char **argv, char **envp)
{
	int pid0, pid, PARENT, EXIT;

	pid0 = pid = getpid(); RUN = true;
	while(RUN)
	{
		pid = getpid();
		PARENT = (pid0 != pid ? worker() : director();
		if(PARENT < 0) { return EXIT_FAILURE; }
	}
	wait(null);
	return EXIT_SUCCESS;
}
