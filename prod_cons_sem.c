#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

typedef void *( *func_type_t)(void*);
func_type_t func_arr[]={producer_routine , consumer_routine};

void* consumer_routine(void*arg) 
{
	while(1)
	{	
	}
}
void* producer_routine(void*arg) 
{
	while(1)
	{	
	}
}

int main()
{
	pthread_t tid[2];
	int ret=0;
	int i=0;
	sem_t full_flag;
	int  g_err1=sem_init(&full_flag, 0, 0) ;
	int y=printf("\n %d",g_err1);
	queue.rear=SIZE-1;
	queue.front=SIZE-1;

	for(i=0;i<2;i++)
		if( ret=pthread_create(&tid[i],NULL,func_arr[i],NULL))
		{
			return -1;
		}

	for(i=0;i<2;i++)
		pthread_join(tid[i],NULL);

	return 0;

}
