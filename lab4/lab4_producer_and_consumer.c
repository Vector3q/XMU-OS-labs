#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<stdlib.h>

#define sizeofbuffer 20

int ring[sizeofbuffer]; //储存队列

sem_t s; // 填充
sem_t n; // 空槽
sem_t e;

int in = 0;
int out = 0;

void producer()
{
	while (1)
	{
		// produce
		sem_wait(&e);
		sem_wait(&s);
		//append
		int data = rand() % 512 + 1;
		ring[in] = data;
		printf("生产者在第%d位产出产品：%d\n", in, data);
		in++;
		in %= sizeofbuffer;
		sem_post(&s);
		sem_post(&n);
		sleep(rand() % 2);
	}
}

void consumer()
{
	while (1)
	{
		sem_wait(&n);
		sem_wait(&s);
		//take
		int data = ring[out];
		printf("消费者在第%d位已消费产品：%d\n", out, data);
		out++;
		out %= sizeofbuffer;
		sem_post(&s);
		sem_post(&e);
		sleep(rand() % 2);
		//consume;
	}
}

void main()
{
	pthread_t p;
	pthread_t c;

	sem_init(&s, 0, 1);
	sem_init(&n, 0, 0);
	sem_init(&e, 0, sizeofbuffer);

	while ((pthread_create(&p, NULL, (void*)producer, NULL)) != 0);
	while ((pthread_create(&c, NULL, (void*)consumer, NULL)) != 0);

	pthread_join(p, NULL);
	pthread_join(c, NULL);

	//sem_destroy(&s);
	//sem_destroy(&n);
	//sem_destroy(&e);
}