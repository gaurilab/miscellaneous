#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

pthread_mutex_t M1= PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t M2= PTHREAD_MUTEX_INITIALIZER;

void* producer_routine(void* arg)
{
 while(1)
	{
	if( 0 == pthread_mutex_lock(&M1)) 	
	{
	
		//	sched_yield();
	if( 0 ==pthread_mutex_lock(&M2)) 
		{	
			
		printf("\n %s Got lock M1 and M2\n",__func__);
		}
	
	pthread_mutex_unlock(&M2); 
	}
	printf("\n %s\n",__func__);
	pthread_mutex_unlock(&M1); 
	}
}

void* consumer_routine(void* arg)
{
	while(1)
	{
		if(0 == pthread_mutex_lock(&M2)) 	
		{
		//	sched_yield();
			if(0 == pthread_mutex_lock(&M1)) 
			{	

		printf("\n %s Got lock M2 and M1\n",__func__);
			}

			pthread_mutex_unlock(&M1); 
		}
		printf("\n %s\n",__func__);
		pthread_mutex_unlock(&M2); 
	}
}



typedef void* (* func_t)(void*);
func_t func_arg[]={producer_routine ,consumer_routine};

int main (int argc ,char *argv[])
{
	pthread_t tid[2];
	int i=0;
	for(i=0;i<2;++i)
	if(0 != pthread_create(&tid[i],NULL,func_arg[i],NULL))
	{
		exit(-1);
	}
	
	for(i=0;i<2;++i)
	pthread_join(&tid[i],NULL);
}
