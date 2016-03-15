#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 10000

struct buffer
{
	int array[SIZE];
	unsigned int front;
	unsigned int rear;
}queue;/* treating this is a circular q*/


sem_t empty_flag;
sem_t full_flag;

/*	int i=0;*/
void *puta(void*arg)
{
	//calculate if the storage is full
	srand(1092);
	int value =0;
	while (1)
	{
		if(((queue.rear+1)%SIZE) == queue.front) // that is the storage is full
		{
			sem_wait(&empty_flag);
		}
		else
		{
			value =rand()%20;
			printf("\n %s 0x%x going to insert at  %d value  %d",__func__,pthread_self(),(queue.rear+1)%SIZE, value);	
			queue.array[(++queue.rear)%SIZE]=value;
			sem_post(&full_flag);
		}
	}
	return NULL;
}

void *eata(void*arg)
{
	int value=0;
	while (1)
	{
		if(queue.rear == queue.front) // that is the storage is empty
		{
			sem_wait(&full_flag);
		}
		else
		{

			value = queue.array[(++queue.front)%SIZE];
			printf("\n %s going to eat at  %d value  %d",__func__,queue.front%SIZE , value);	
			sem_post(&empty_flag);
		}
	}

	return NULL;
}

typedef void *( *func_type_t)(void*);
func_type_t func_arr[]={puta,eata,puta};
int main()
{
	pthread_t tid[3];
	int ret=0;
	int i=0;
	int  g_err=sem_init(&empty_flag, 0, 0) ;
	int x=printf("\n %d",g_err);
	int  g_err1=sem_init(&full_flag, 0, 0) ;
	int y=printf("\n %d",g_err1);
	queue.rear=SIZE-1;
	queue.front=SIZE-1;

	for(i=0;i<3;i++)
		if( ret=pthread_create(&tid[i],NULL,func_arr[i],NULL))
		{
			return -1;
		}

	for(i=0;i<3;i++)
		pthread_join(tid[i],NULL);

	return 0;
}
