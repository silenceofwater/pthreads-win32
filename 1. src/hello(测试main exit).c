/******************************************************************************
* FILE: hello.c
* DESCRIPTION:
*   A "hello world" Pthreads program.  Demonstrates thread creation and
*   termination.
* AUTHOR: Blaise Barney
* LAST REVISED: 08/09/11
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#define NUM_THREADS	5

#pragma comment(lib, "pthreadVC2.lib")

void *PrintHello(void *threadid)
{
	long tid;
	tid = (long)threadid;
	printf("Hello World! It's me, thread #%ld!\n", tid);
	
	//加入下面两行，测试main()中是否加入pthread_exit(NULL)的影响
	Sleep(tid*1000);
	printf("Hello World! It's me, thread #%ld!---finished\n", tid);
	
	pthread_exit(NULL);
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	int rc;
	long t;

	for (t = 0; t < NUM_THREADS; t++)
	{
		printf("In main: creating thread %ld\n", t);
		rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
		if (rc)
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	/* Last thing that main() should do */
	pthread_exit(NULL);

	return 0;
}