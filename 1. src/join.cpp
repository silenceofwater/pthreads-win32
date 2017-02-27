/*****************************************************************************
* FILE: join.cpp
* DESCRIPTION:
*   This example demonstrates how to "wait" for thread completions by using
*   the Pthread join routine.
*   Threads are explicitly created in a joinable
*   state for portability reasons. 
*	Use of the pthread_exit status argument is also shown. 
*   Compare to detached.cpp
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
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

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

	void *status;
	for (int t = 0; t < NUM_THREADS; t++)
	{
		int rc = pthread_join(thread[t], &status);
		if (rc != 0) 
		{
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
		printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
	}

	printf("Main: program completed. Exiting.\n");
	pthread_exit(nullptr);

	return 0;
}
