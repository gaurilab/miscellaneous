
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#define NUM_THREADS 50
void *start_routine_a(void*arg)
{
	printf("\n In %s the child thread 0x%x ",__func__,pthread_self());
	sched_yield();
	sleep (3);
	printf("\n In %s the child thread 0x%x after yielding ",__func__,pthread_self());
	return NULL;
}
void *start_routine_b(void*arg)
{
	printf("\n In %s the child thread 0x%x ",__func__,pthread_self());
	sched_yield();
	sleep (3);
	printf("\n In %s the child thread 0x%x after yielding ",__func__,pthread_self());
	return NULL;
}
void *start_routine_c(void*arg)
{
	printf("\n In %s the child thread 0x%x ",__func__,pthread_self());
	sched_yield();
	sleep (3);
	printf("\n In %s the child thread 0x%x after yielding ",__func__,pthread_self());
	return NULL;
}

void *start_routine_d(void*arg)
{
	printf("\n In %s the child thread 0x%x ",__func__,pthread_self());
	sched_yield();
	sleep (3);
	printf("\n In %s the child thread 0x%x after yielding ",__func__,pthread_self());
	return NULL;
}

void *start_routine_e(void*arg)
{
	printf("\n In %s the child thread 0x%x ",__func__,pthread_self());
	sleep (3);
	sched_yield();
	printf("\n In %s the child thread 0x%x after yielding ",__func__,pthread_self());
	while(1);
	return NULL;
}


typedef void *( *func_type_t)(void*);
 func_type_t func_arr[]={start_routine_a,start_routine_b,start_routine_c,start_routine_d ,start_routine_e};
int main()
{
 pthread_t tid[NUM_THREADS];
 int ret=0;
 int i=0;
 	
	sleep (3);
 printf("\n In %s the thread %u ",__func__,pthread_self());
 for(i=0;i<NUM_THREADS;i++)
{
 if( (ret=pthread_create(&tid[i],NULL,func_arr[i%5],NULL)))
 {
	return -1;
 }
}
#if 0 
 ret=0;
 for(i=0;i<NUM_THREADS;i++)
{
 if( ret=pthread_kill(tid[NUM_THREADS],SIGUSR1))
 {
	printf("\n pthread_kill return failure %d",ret);
	/*return -1;*/
 }
}
#endif
#if 0
pthread_exit(1); // To create defunct process
#endif
 for(i=0;i<NUM_THREADS;i++)
{	
 pthread_join(tid[i],NULL);
 sleep(5);
}
 return 0;
}
