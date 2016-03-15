#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 10

struct buffer
{
	int array[SIZE];
	unsigned int front;
	unsigned int rear;
	pthread_mutex_t  m;
}queue;// treating this is a circular q

pthread_cond_t queue_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t queue_empty = PTHREAD_COND_INITIALIZER; 

/*	int i=0;*/
void *puta(void*arg)
{
	//calculate if the storage is full
	srand(1092);
	int value =0;
	while (1)
	{
		pthread_mutex_lock(&queue.m);
		while (((queue.rear+1)%SIZE )== (queue.front%SIZE))
		{

			printf("\n %s 0x%x producer thread  rear %d front  %d waititng for queue to be empty",__func__,pthread_self(),(queue.rear)%SIZE, queue.front%SIZE);
			pthread_cond_wait(&queue_empty,&queue.m);	
		}
		value =rand()%20;
		//printf("\n %s 0x%x producer thread  rear %d front  %d ",__func__,pthread_self(),(queue.rear)%SIZE, queue.front%SIZE);
		printf("\n %s 0x%x going to insert at  %d value  %d",__func__,pthread_self(),(queue.rear+1)%SIZE, value);
		queue.array[(++queue.rear)%SIZE]=value;
		sleep(5);
		pthread_cond_signal(&queue_full);
		pthread_mutex_unlock(&queue.m);
	}
	return NULL;
}

void *eata(void*arg)
{
	int value=0;
	sleep(50);
	while (1)
	{
		pthread_mutex_lock(&queue.m);
		while (((queue.rear)%SIZE ) == (queue.front%SIZE))
		{

			printf("\n %s 0x%x consumer thread  rear %d front  %d waiting for producer to fill the buffer",__func__,pthread_self(),(queue.rear)%SIZE, queue.front%SIZE);
			pthread_cond_wait(&queue_full,&queue.m);
		}
		value = queue.array[(++queue.front)%SIZE];
		//printf("\n %s 0x%x consumer thread  rear %d front  %d ",__func__,pthread_self(),(queue.rear)%SIZE, queue.front%SIZE);
		printf("\n %s 0x%x going to print at  %d value  %d",__func__,pthread_self(),(queue.front%SIZE), value);
		sleep(5);
		pthread_cond_signal(&queue_empty);
		pthread_mutex_unlock(&queue.m);
	}

	return NULL;
}

typedef void *( *func_type_t)(void*);
func_type_t func_arr[]={puta,eata,puta,eata};
int main()
{
	pthread_t tid[4];
	int ret=0;
	int i=0;
	pthread_mutex_init(&queue.m,NULL);
	queue.rear=SIZE-1;
	queue.front=SIZE-1;

	for(i=0;i<4;i++)
		if( ret=pthread_create(&tid[i],NULL,func_arr[i],NULL))
		{
			return -1;
		}

	for(i=0;i<2;i++)
		pthread_join(tid[i],NULL);

	return 0;
}
