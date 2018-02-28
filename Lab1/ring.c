#include <sys/wait.h>
#include "misc.h"

#define RING_LIMIT_K 50
#define RING_MSG_DD "pid=%d: %d\n"

/* 
 * cpipe (circularpipe) spawns N - 1 processes, creats pipe,
 * given every fork and circularl links the buffers of pipes
 * with respect to the read buffer, each process writes to
 * its own and reads from its previous linked buffer.
 * 
 * N : integer - number of processes including the root
 * PARENT : *bool - is false(pid==0) or true(pid!=0) ret by ref
 * pidn : *int - ret by ref process id
 * fdn : int[2] - io read wirte of nth process 
 */
void cpipe(int N, int *PARENT, int *pidn, int fdn[2])
{	
	int n, rk, rn, w0, pid0;
	
	/* rn = rk = -, w0 = - , fdn = {b0,(r0,w0)} */
	rn = fdn[0]; w0 = fdn[1]; pid0 = *pidn;
	/* rn = rk = r0, w0 = w0, fdn = fdk = {b0,(r0,w0)} */
	for (n = 1; n < N; ++n)
	{	
		/* rn = rk, fdn = {bk,(rk,wk)} */
		pipe_s(fdn); fork_s();
		/* rn = rk, fdn = {bn,(rn,wn)} */
		rk = rn; rn = fdn[0]; fdn[0] = rk;
		/* rn = rn, fdn = {bn,(rk,wn)} */
		if(pid0 != (*pidn = getpid())) { return; }
	}
	/* rn = rn, w0 = w0, fdn = {bn,(rk,wn)} */
	fdn[0] = rn; fdn[1] = w0;
	/* rn = rn, w0 = w0, fdn = {b0,(rn,w0)} */
}

int main(int argc, char **argv)
{
	int PARENT, pidn, fdn[2], N, K, sz;

	if(argc != 2) { exit(EXIT_FAILURE); }

	/* check pre conditions and initialize variable */
	N = atoi(argv[1]); K = 0; sz = sizeof(int);
	if(N < 2 || N > 16) { exit(EXIT_FAILURE); }
	
	/* open a pipe and assign pid write first value to pipe buff */
	pipe_s(fdn); pidn = getpid();
	write(fdn[1], &K, sz);
	printf(RING_MSG_DD, pidn, K);
	
	/* create a circular pipe line */
	cpipe(N, &PARENT, &pidn, fdn);
	while(true)
	{
		/*
		 * the reading is blocking so order will be maintaind
		 * in order of incounter.
		 *
		 * observed some order problems with N=2 may meaning that
		 * they share the same pipe buff, the idea was to introduce
		 * a pipe for each proccess to write and block reads
		 * from its neighbour.
		 */
		read(fdn[0], &K, sz);
		++K;
		write(fdn[1], &K, sz);
		/* teminate the loop if K limit is reached */
		if(K > RING_LIMIT_K) { break; };
		printf(RING_MSG_DD, pidn, K);
	}
	/* cleanup and close discriptors and wait for all processes to finish */
	close(fdn[0]); close(fdn[1]);
	wait(null);

	return EXIT_SUCCESS;
}
