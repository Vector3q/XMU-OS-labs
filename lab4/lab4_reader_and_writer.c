#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<stdlib.h>

#define N 5
#define M 2
#define MAX 1000

int readcount;
sem_t x;
sem_t wsem;

int buffer[MAX];

int in = 0;
int out;

void reader(void* arg)
{
	int n = *(int*)arg;
	while (1)
	{
		if (in == 0)
		{
			continue;
		}

		sem_wait(&x);

		readcount++;
		if (readcount == 1)
			sem_wait(&wsem);

		sem_post(&x);


		//read
		out = rand() % in;
		printf("读者%d读取缓冲区第%d位，数据为：%d\n", n, out + 1, buffer[out]);

		sem_wait(&x);

		readcount--;
		if (readcount == 0)
			sem_post(&wsem);

		sem_post(&x);

		sleep(rand() % 3);
	}
}

void writer(void* arg)
{
	int n = *(int*)arg;
	while (1)
	{
		if (in == 0)
			sem_wait(&x);

		sem_wait(&wsem);

		//write
		int data = rand() % 512 + 1;
		buffer[in] = data;
		in++;

		if (in == 1)
			sem_post(&x);

		printf("写者%d已在第%d位写下内容：%d\n", n, in, data);

		sem_post(&wsem);

		sleep(rand() % 2);
	}
}

void main()
{
	readcount = 0;
	pthread_t r[N];
	pthread_t w[M];

	sem_init(&x, 0, 1);
	sem_init(&wsem, 0, 1);

	int i = 1;

	for (i = 1; i < N; ++i)
	{
		int* d = (int*)malloc(sizeof(int));//i是局部变量，如果可能当你将i值传进去时
	 //时间片切出去将会把i更改，这有就会影响i的值
		*d = i;
		pthread_create(&r[i], NULL, (void*)reader, (void*)d);
	}
	for (i = 1; i < N; ++i)
	{
		int* d = (int*)malloc(sizeof(int));
		*d = i;
		pthread_create(&w[i], NULL, (void*)writer, (void*)d);
	}

	for (i = 1; i < N; ++i)
	{
		pthread_join(r[i], NULL);
	}
	for (i = 1; i < N; ++i)
	{
		pthread_join(w[i], NULL);
	}

	//sem_destroy(&s);
	//sem_destroy(&n);
	//sem_destroy(&e);
}