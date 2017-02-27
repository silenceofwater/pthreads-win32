/***************************************************************
* FILE: condvar.cpp
* DESCRIPTION:
*   Example code for using Pthreads condition variables.  The main thread
*   creates three threads.  Two of those threads increment a "count" variable,
*   while the third thread watches the value of "count".  When "count" 
*   reaches a predefined limit, the waiting thread is signaled by one of the
*   incrementing threads. The waiting thread "awakens" and then modifies
*   count. The program continues until the incrementing threads reach
*   TCOUNT. The main program prints the final value of count.
***************************************************************/
#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <Windows.h>

#pragma comment(lib, "pthreadVC2.lib")

#define NUM_THREADS  3
#define TCOUNT 10
#define COUNT_LIMIT 12

int     count = 0;
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *inc_count(void *t) 
{
	for (int i = 0; i < TCOUNT; i++) 
	{
		pthread_mutex_lock(&count_mutex);
		count++;

		/* 
		Check the value of count and signal waiting thread when condition is
		reached.  Note that this occurs while mutex is locked. 
		*/
		if (count == COUNT_LIMIT)
		{
			printf("inc_count(): thread %p, count = %d  Threshold reached. ", pthread_self().p, count);
			pthread_cond_signal(&count_threshold_cv);
			printf("Just sent signal.\n");
		}
		printf("inc_count(): thread %p, count = %d, unlocking mutex\n", pthread_self().p, count);
		pthread_mutex_unlock(&count_mutex);

		/* Do some "work" so threads can alternate on mutex lock */
		Sleep(5000);
    }

	pthread_exit(nullptr);
	return nullptr;
}

void *watch_count(void *t) 
{
	printf("Starting watch_count(): thread %p\n", pthread_self().p);

	/*
	 Lock mutex and wait for signal.  Note that the pthread_cond_wait routine
	 will automatically and atomically unlock mutex while it waits. 
	 Also, note that if COUNT_LIMIT is reached before this routine is run by
	 the waiting thread, the loop will be skipped to prevent pthread_cond_wait
	 from never returning.
	 */
	pthread_mutex_lock(&count_mutex);
	while (count < COUNT_LIMIT) 
	{
		printf("watch_count(): thread %p Count= %d. Going into wait...\n",  pthread_self().p,count);
		pthread_cond_wait(&count_threshold_cv, &count_mutex);
		printf("watch_count(): thread %p Condition signal received. Count= %d\n",  pthread_self().p,count);
		printf("watch_count(): thread %p Updating the value of count...\n",  pthread_self().p,count);
		count += 125;
		printf("watch_count(): thread %p count now = %d.\n",  pthread_self().p, count);
	}
	printf("watch_count(): thread %p Unlocking mutex.\n",  pthread_self().p);
	pthread_mutex_unlock(&count_mutex);
	pthread_exit(nullptr);
	return nullptr;  
}

int main(int argc, char *argv[])
{
	pthread_t threads[3];
	pthread_attr_t attr;

	/* Initialize mutex and condition variable objects */
	pthread_mutex_init(&count_mutex, NULL);
	pthread_cond_init (&count_threshold_cv, NULL);

	/* For portability, explicitly create threads in a joinable state */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_create(&threads[0], &attr, watch_count, nullptr);
	pthread_create(&threads[1], &attr, inc_count, nullptr);
	pthread_create(&threads[2], &attr, inc_count, nullptr);

	/* Wait for all threads to complete */
	for (int i = 0; i < NUM_THREADS; i++) 
	{
		pthread_join(threads[i], NULL);
	}
	printf ("Main(): Waited and joined with %d threads. Final value of count = %d. Done.\n", 
		NUM_THREADS, count);

	/* Clean up and exit */
	pthread_attr_destroy(&attr);
	pthread_mutex_destroy(&count_mutex);
	pthread_cond_destroy(&count_threshold_cv);
	pthread_exit (nullptr);

	return 0;
}