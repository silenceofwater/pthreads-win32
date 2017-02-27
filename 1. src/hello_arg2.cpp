/************************** 使用全局参数 **************************/
/*
 * FILE: hello_arg2.cpp
 * DESCRIPTION: 
 *	 A "hello world" Pthreads program which demonstrates another safe way
 *   to pass arguments to threads during thread creation.  In this case,
 *   a structure is used to pass multiple arguments.
 */

#include <pthread.h>
#include <cstdio>
#include <cstdlib>   // for exit(-1)
#include <Windows.h> // for Sleep()

#pragma comment(lib, "pthreadVC2.lib")

#define NUM_THREADS	8

struct thread_data
{
	int	thread_id;
	int sum;
	char *message;
};

// 注意 thread_data_array的位置
thread_data thread_data_array[NUM_THREADS];

void *PrintHello(void *threadarg)
{
	Sleep(1000);
	thread_data my_data = *(thread_data *) threadarg;
	int taskid = my_data.thread_id;
	int sum = my_data.sum;
	char *hello_msg = my_data.message;
	printf("Thread %d: %s  Sum=%d\n", taskid, hello_msg, sum);
	pthread_exit(nullptr);
	return nullptr;
}

int main(int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	char *messages[NUM_THREADS];
	messages[0] = "English: Hello World!";
	messages[1] = "French: Bonjour, le monde!";
	messages[2] = "Spanish: Hola al mundo";
	messages[3] = "Klingon: Nuq neH!";
	messages[4] = "German: Guten Tag, Welt!"; 
	messages[5] = "Russian: Zdravstvuyte, mir!";
	messages[6] = "Japan: Sekai e konnichiwa!";
	messages[7] = "Latin: Orbis, te saluto!";

	int sum = 0;

	for (int t=0; t < NUM_THREADS; t++)
	{
		sum += t;
		thread_data_array[t].thread_id = t;
		thread_data_array[t].sum = sum;
		thread_data_array[t].message = messages[t];

		printf("Creating thread %d\n", t);
		int rc = pthread_create(&threads[t], NULL, PrintHello, (void *) &thread_data_array[t]);
		if (rc != 0) 
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	pthread_exit(nullptr);
	return 0;
}

/************************** 使用动态创建 **************************/
#include <pthread.h>
#include <cstdio>
#include <cstdlib>   // for exit(-1)
#include <Windows.h> // for Sleep()

#pragma comment(lib, "pthreadVC2.lib")

#define NUM_THREADS	8

struct thread_data
{
	int	thread_id;
	int sum;
	char *message;
};

void *PrintHello(void *threadarg)
{
	Sleep(1000);
	thread_data* my_data = (thread_data *) threadarg;
	int taskid = my_data->thread_id;
	int sum = my_data->sum;
	char *hello_msg = my_data->message;
	printf("Thread %d: %s  Sum=%d\n", taskid, hello_msg, sum);
	
	delete my_data;

	pthread_exit(nullptr);
	return nullptr;
}

int main(int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	char *messages[NUM_THREADS];
	messages[0] = "English: Hello World!";
	messages[1] = "French: Bonjour, le monde!";
	messages[2] = "Spanish: Hola al mundo";
	messages[3] = "Klingon: Nuq neH!";
	messages[4] = "German: Guten Tag, Welt!"; 
	messages[5] = "Russian: Zdravstvuyte, mir!";
	messages[6] = "Japan: Sekai e konnichiwa!";
	messages[7] = "Latin: Orbis, te saluto!";

	int sum = 0;
	thread_data* thread_data_array[NUM_THREADS];

	for (int t=0; t < NUM_THREADS; t++)
	{
		sum += t;
		thread_data_array[t] = new thread_data();
		thread_data_array[t]->thread_id = t;
		thread_data_array[t]->sum = sum;
		thread_data_array[t]->message = messages[t];

		printf("Creating thread %d\n", t);
		int rc = pthread_create(&threads[t], NULL, PrintHello, (void *) thread_data_array[t]);
		if (rc != 0) 
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	pthread_exit(nullptr);
	return 0;
}