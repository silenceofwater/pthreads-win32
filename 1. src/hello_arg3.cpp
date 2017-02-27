/*****************************************************************************
* FILE: hello_arg3.cpp
* DESCRIPTION:
*   This "hello world" Pthreads program demonstrates an unsafe (incorrect)
*   way to pass thread arguments at thread creation.  In this case,
*   it passes the address of variable t, which is shared memory space and visible to all threads.
*   As the loop iterates, the value of this memory location changes, 
*   possibly before the created threads can access it.
******************************************************************************/
#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#pragma comment(lib, "pthreadVC2.lib")
#define NUM_THREADS     8

void *PrintHello(void *threadid)
{
	int taskid = *(int *)threadid;
	printf("Hello from thread %ld\n", taskid);
	pthread_exit(nullptr);

	return nullptr;
}

int main(int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];

	for (int t=0; t < NUM_THREADS; t++) 
	{
		printf("Creating thread %ld\n", t);
		// 这里传递t有问题
		// 有可能在线程还未获取到其原本传递的值时，已经被循环更新了
		int rc = pthread_create(&threads[t], nullptr, PrintHello, (void *) &t);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	pthread_exit(nullptr);
	return 0;
}