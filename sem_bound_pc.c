#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define MAX_RAND_NUM 13
typedef struct node
{
 int data;
 struct node *next;
 struct node *prev;
}node_t;
typedef struct queue
{
 node_t *front;
 node_t *rear;
}queue_t;

queue_t Q;

typedef void *( *func_type_t)(void*);
sem_t num_elm;
sem_t num_slots;
void printQ()
{
	node_t *it =Q.rear;
	if(Q.rear == NULL ) return;
	while(it !=NULL)
	{
		assert(it!= NULL);
		printf("\t %d ",it->data);
		it=it->next;
	}
	printf("\n");
	fflush(stdout);
}
node_t* getnode(int data)
{
	node_t *element =(node_t*) malloc (sizeof(node_t));
	assert(element!= NULL);
	element->next=NULL;	
	element->prev=NULL;	
	element->data=data;
	return element;
}
void insert(node_t* elm)
{
	if(Q.front == NULL && Q.rear == NULL)
	{
		//first element
		Q.front =elm;
		Q.rear =elm;
	}
	else
	{
		Q.rear->prev =elm;
		elm->next=Q.rear;
		Q.rear=elm;
	}

}

node_t* delete()
{
	node_t *element=NULL;
	node_t *tmp=NULL;
	if((Q.front == Q.rear)&&(Q.rear != NULL))
	{
		//last element left
		element= Q.front;
		Q.front =NULL;
		Q.rear =NULL;
	}	
	else if((Q.front == Q.rear)&&(Q.rear == NULL))
	{
		element=NULL;	
	}
	else
	{
		element = Q.front;
		Q.front = Q.front->prev;
		Q.front->next=element->next;
	}
	return element;	
}

void* consumer_routine(void *arg) 
{
	srand(time(NULL));
	while(1)
	{
		sem_wait(&num_elm);
		printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	fflush(stdout);
		printQ();	
		node_t* elm =  delete();
		if(elm != NULL)
		printf("\n%s%p  deleting %d\n",__func__ ,pthread_self(),elm->data);	
		free(elm);
		printQ();	
		printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	fflush(stdout);
		sem_post(&num_slots);
		//sleep(1);
	}
}
void* producer_routine(void *arg) 
{
	while(1)
	{
		
		node_t *element =getnode(rand()%MAX_RAND_NUM);
		sem_wait(&num_slots);
		printf("=====================================================================\n");
	fflush(stdout);
		printQ();	
		printf("\n%s%p  inserting %d\n",__func__ ,pthread_self(),element->data);	
		insert(element);
		printQ();	
		sem_post(&num_elm);
		printf("=====================================================================\n");
	fflush(stdout);
		//sleep(10);
	}
}

func_type_t func_arr[]={producer_routine , consumer_routine,consumer_routine};
int main()
{
	pthread_t tid[3];
	int ret=0;
	int i=0;
	int  g_err1=sem_init(&num_elm, 0, 0) ;
	g_err1=sem_init(&num_elm, 0, 10) ;
	//int y=printf("\n %d",g_err1);
	Q.rear=NULL;
	Q.front=NULL;
	
	for(i=0;i<3;i++)
		if( ret=pthread_create(&tid[i],NULL,func_arr[i],NULL))
		{
			return -1;
		}

	for(i=0;i<3;i++)
		pthread_join(tid[i],NULL);

	return 0;

}
