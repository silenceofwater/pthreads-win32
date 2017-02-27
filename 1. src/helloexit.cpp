/*
 * FILE: helloexit.cpp
 * DESCRIPTION: 
 *	 Demonstrates the influence of pthread_exit
 */

#include <pthread.h>
#include <cstdio>
#include <cstdlib>   // for exit(-1)
#include <Windows.h> // for Sleep()

#pragma comment(lib, "pthreadVC2.lib")

#define NUM_THREADS	5

void *PrintHello(void *threadid)
{
	int tid = (int)threadid;
	
	//加入下面行，测试main()中是否加入pthread_exit(nullptr)的影响
	Sleep(tid*1000);

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

	//注释掉时观测不加入该行时的影响
	pthread_exit(nullptr);

	return 0;
}