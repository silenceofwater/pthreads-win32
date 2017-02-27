/*
 * FILE: hello_arg1.cpp
 * DESCRIPTION: 
 *	 A "hello world" Pthreads program which demonstrates one safe way
 *   to pass arguments to threads during thread creation.
 */

#include <pthread.h>
#include <cstdio>
#include <cstdlib>   // for exit(-1)
#include <Windows.h> // for Sleep()

#pragma comment(lib, "pthreadVC2.lib")

#define NUM_THREADS	8

char *messages[NUM_THREADS];

void *PrintHello(void *threadid)
{
	Sleep(1000);
	int taskid = (int) threadid;
	printf("Thread %d: %s\n", taskid, messages[taskid]);
	pthread_exit(nullptr);
	return nullptr;
}

int main(int argc, char *argv[])
{
	messages[0] = "English: Hello World!";
	messages[1] = "French: Bonjour, le monde!";
	messages[2] = "Spanish: Hola al mundo";
	messages[3] = "Klingon: Nuq neH!";
	messages[4] = "German: Guten Tag, Welt!"; 
	messages[5] = "Russian: Zdravstvuyte, mir!";
	messages[6] = "Japan: Sekai e konnichiwa!";
	messages[7] = "Latin: Orbis, te saluto!";

	pthread_t threads[NUM_THREADS];
	int taskids[NUM_THREADS];

	for (int t=0; t < NUM_THREADS; t++)
	{
		taskids[t] = t;
		printf("Creating thread %d\n", t);
		int rc = pthread_create(&threads[t], NULL, PrintHello, (void *) taskids[t]);
		if (rc != 0) 
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	pthread_exit(nullptr);
	return 0;
}