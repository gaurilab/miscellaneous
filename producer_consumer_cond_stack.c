#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 10

struct buffer
{
	int array[SIZE];
	unsigned int top;
	pthread_mutex_t  m;
}stack;// treating this is a circular q

pthread_cond_t stack_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t stack_empty = PTHREAD_COND_INITIALIZER; 

/*	int i=0;*/
void *puta(void*arg)
{
	//calculate if the storage is full
	srand(1092);
	int value =0;
	while (1)
	{
		pthread_mutex_lock(&stack.m);
		printf("\n %s 0x%x Checking condition stack.top == SIZE-1 ",__func__,pthread_self());
		while (stack.top == SIZE-1 )
		{

			printf("\n %s 0x%x producer thread  top %d waititng for stack to be empty",__func__,pthread_self(),(stack.top));
			pthread_cond_wait(&stack_empty,&stack.m);	
		}
		value =rand()%20;
		printf("\n %s 0x%x going to insert at  %d value  %d",__func__,pthread_self(),stack.top+1, value);
		stack.array[(++stack.top)]=value;
		printf("\n %s 0x%x sending signal stack_full ",__func__,pthread_self());
		pthread_cond_signal(&stack_full);
		pthread_mutex_unlock(&stack.m);
	}
	return NULL;
}

void *eata(void*arg)
{
	int value=0;
	while (1)
	{
		pthread_mutex_lock(&stack.m);
		printf("\n %s 0x%x Checking condition stack.top == -1 value of top =%d",__func__,pthread_self(),stack.top);
		while (stack.top == -1 )
		{

			printf("\n %s 0x%x consumer thread  top %d top  %d waiting for producer to fill the buffer",__func__,pthread_self(),(stack.top)%SIZE, stack.top%SIZE);
			pthread_cond_wait(&stack_full,&stack.m);
		}
		value = stack.array[stack.top--];
		printf("\n %s 0x%x going to print at  %d value  %d",__func__,pthread_self(),stack.top+1, value);
		printf("\n %s 0x%x sending signal stack_empty",__func__,pthread_self());
		pthread_cond_signal(&stack_empty);
		pthread_mutex_unlock(&stack.m);
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
	pthread_mutex_init(&stack.m,NULL);
	stack.top=-1;

	for(i=0;i<4;i++)
		if( ret=pthread_create(&tid[i],NULL,func_arr[i],NULL))
		{
			return -1;
		}

	for(i=0;i<2;i++)
		pthread_join(tid[i],NULL);

	return 0;
}
