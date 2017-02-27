/*****************************************************************************
* FILE: detached.cpp
* DESCRIPTION:
*   This example demonstrates how to explicitly create a thread in a 
*   detached state. This might be done to conserve some system resources
*   if the thread never needs to join later. Compare with the join.cpp program
*   where the threads are created joinable
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#pragma comment(lib, "pthreadVC2.lib")

#define NUM_THREADS	4

void *BusyWork(void *t)
{
	int tid = (long)t;
	printf("Thread %ld starting...\n",tid);
	Sleep(tid*1000);
	printf("Thread %ld done!\n",tid);
	pthread_exit(t);
	return nullptr;
}

int main (int argc, char *argv[])
{
	pthread_t thread[NUM_THREADS];
	pthread_attr_t attr;

	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	for (int t = 0; t < NUM_THREADS; t++)
	{
		printf("Main: creating thread %ld\n", t);
		int rc = pthread_create(&thread[t], &attr, BusyWork, (void *)t); 
		if (rc != 0) 
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	/* Free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);

	printf("Main: program completed. Exiting.\n");
	pthread_exit(nullptr);

	return 0;
}
