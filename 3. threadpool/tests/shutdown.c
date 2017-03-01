#include <cstdio>
#include <cassert>
#include <Windows.h>
#include <pthread.h>
#include "threadpool.h"

#pragma comment(lib, "pthreadVC2.lib")

#define THREAD 4
#define SIZE   256

threadpool_t *pool;
int left;
pthread_mutex_t lock;

int error;

void dummy_task(void *arg) 
{
	pthread_mutex_lock(&lock);
	Sleep(20);
	left--;
	pthread_mutex_unlock(&lock);
}

int main(int argc, char **argv)
{
	int i;

	pthread_mutex_init(&lock, NULL);

	/* Testing immediate shutdown */
	/* 立即结束，不再执行任务队列中的任务 */
	/*left = SIZE;
	pool = threadpool_create(THREAD, SIZE, 0);
	for(i = 0; i < SIZE; i++) {
		assert(threadpool_add(pool, &dummy_task, NULL, 0) == 0);
	}
	assert(threadpool_destroy(pool, 0) == 0);
	assert(left > 0);*/

	/* Testing graceful shutdown */
	/* 优雅结束，执行完任务队列中的任务 */
	left = SIZE;
	pool = threadpool_create(THREAD, SIZE, 0);
	for(i = 0; i < SIZE; i++) {
		assert(threadpool_add(pool, &dummy_task, NULL, 0) == 0);
	}
	assert(threadpool_destroy(pool, threadpool_graceful) == 0);
	assert(left == 0);

	pthread_mutex_destroy(&lock);

	return 0;
}