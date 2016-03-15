
#include <pthread.h>
#include <signal.h>
#include <stdio.h>

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
	return NULL;
}


typedef void *( *func_type_t)(void*);
 func_type_t func_arr[]={start_routine_a,start_routine_b,start_routine_c,start_routine_d ,start_routine_e};
int main()
{
	printf("\n In %s the child thread 0x%x ",__func__,pthread_self());
 pthread_t tid[5];
 int ret=0;
 int i=0;
 	
 for(i=0;i<5;i++)
 if( ret=pthread_create(&tid[i],NULL,func_arr[i],NULL))
 {
	return -1;
 }
#if 0 
 ret=0;
 for(i=0;i<2;i++)
 if( ret=pthread_kill(&tid[i],SIGUSR1))
 {
	printf("\n pthread_kill return failure %d",ret);
	return -1;
 }
#endif
 for(i=0;i<5;i++)
 pthread_join(tid[i],NULL);

 return 0;
}
