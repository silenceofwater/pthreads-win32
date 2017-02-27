/*
 * FILE: hello.cpp
 * DESCRIPTION:
 *   A "hello world" Pthreads program.  
 *	 Demonstrates thread creation and termination.
 */

#include <pthread.h>
#include <cstdio>
#include <cstdlib> // for exit(-1)

#pragma comment(lib, "pthreadVC2.lib")

#define NUM_THREADS	5

void *PrintHello(void *threadid)
{
	int tid = (int)threadid;
	printf("Hello World! It's me, thread #%ld!\n", tid);
	pthread_exit(nullptr);
	return nullptr;
}
 
int main(int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];

	for (int t = 0; t < NUM_THREADS; t++)
	{
		printf("In main: creating thread %ld\n", t);
		int rc = pthread_create(&threads[t], nullptr, PrintHello, (void *)t);
		if (rc != 0)
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	// Last thing that main() should do
	pthread_exit(nullptr);

	return 0;
}