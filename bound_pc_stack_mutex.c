#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define MAX_RAND_NUM 13

typedef struct node
{
 int data;
 struct node *next;
}node_t;
static pthread_key_t thread_log_key;
typedef struct  stack
{
 node_t *top;
}stack_t;

stack_t S;
void write_to_thread_log (const char* message)
{
  FILE* thread_log = (FILE*) pthread_getspecific (thread_log_key);
  fprintf (thread_log, "%s\t",message);
  fflush(thread_log);	
}
typedef void *( *func_type_t)(void*);
pthread_mutex_t access = PTHREAD_MUTEX_INITIALIZER;
void printS()
{
	pthread_mutex_lock(&access);
	node_t *it =S.top;
	char log[20]={0};
	if(S.top== NULL ) return;
	
	while(it !=NULL)
	{
		assert(it!= NULL);
		printf("\t %d ",it->data);
		it=it->next;
	}
	printf("\n");
	pthread_mutex_unlock(&access);
}
node_t* getnode(int data)
{
	node_t *element =(node_t*) malloc (sizeof(node_t));
	assert(element!= NULL);
	element->next=NULL;	
	element->data=data;
	return element;
}
void insert(node_t* elm)
{
	pthread_mutex_lock(&access);
	elm->next = S.top;
	S.top =elm;
	pthread_mutex_unlock(&access);
}

node_t* delete()
{
	pthread_mutex_lock(&access);
	node_t *element = S.top;
	if(element == NULL ) return NULL;
	S.top=element->next;
	element->next=NULL;
	pthread_mutex_unlock(&access);
	return element;	
}

void* consumer_routine(void *arg) 
{
	char thread_log_filename[20];
	char log[200];
	FILE* thread_log;

	/* Generate the filename for this thread's log file.  */
	sprintf(thread_log_filename, "thread%d.log", (int) pthread_self ());
	/* Open the log file.  */
	thread_log = fopen (thread_log_filename, "w");
	pthread_setspecific (thread_log_key, thread_log);
	 
	while(1)
	{
		write_to_thread_log("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
		fflush(stdout);
		printS();	
		node_t* elm =  delete();
		if(elm != NULL)
		{
			snprintf(log,200,"%s%p  deleting %d\n",__func__ ,pthread_self(),elm->data);	
		write_to_thread_log(log);
		}
		free(elm);
		printS();	
		write_to_thread_log("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
		fflush(stdout);
		//sleep(1);
	}
}
void* producer_routine(void *arg) 
{
	char thread_log_filename[20];
	char log[200];
	FILE* thread_log;

	/* Generate the filename for this thread's log file.  */
	sprintf(thread_log_filename, "thread%d.log", (int) pthread_self ());
	/* Open the log file.  */
	thread_log = fopen (thread_log_filename, "w");
	 //pthread_setspecific (thread_log_key, thread_log);

	srand(time(NULL));
	while(1)
	{
	pthread_mutex_lock(&access);
	printf("\n lock iteration 1");
	pthread_mutex_unlock(&access);
	pthread_mutex_lock(&access);
	printf("\n lock iteration 2");
	pthread_mutex_unlock(&access);
	pthread_mutex_lock(&access);
	printf("\n lock iteration 3");
	pthread_mutex_unlock(&access);



		node_t *element =getnode(rand()%MAX_RAND_NUM);
		printf("=====================================================================\n");
		fflush(stdout);
		printS();	
		snprintf(log,200,"\n%s%p  inserting %d\n",__func__ ,pthread_self(),element->data);	
	//	write_to_thread_log(log);
		insert(element);
		printS();	
	printf("=====================================================================\n");
		fflush(stdout);
		//sleep(10);
	}
}
void close_thread_log (void* thread_log)
{
  fclose ((FILE*) thread_log);
}

func_type_t func_arr[]={producer_routine , consumer_routine,consumer_routine};
int main()
{
	pthread_t tid[3];
	int ret=0;
	int i=0;
	S.top=NULL;
	ret =pthread_mutex_init(&access, NULL);
	if(ret!=0) return -1;

	pthread_mutex_lock(&access);
	printf("\n lock iteration 1");
	pthread_mutex_unlock(&access);
	pthread_mutex_lock(&access);
	printf("\n lock iteration 2");
	pthread_mutex_unlock(&access);
	pthread_mutex_lock(&access);
	printf("\n lock iteration 3");
	pthread_mutex_unlock(&access);

	//pthread_key_create (&thread_log_key, close_thread_log);
	producer_routine(NULL);
#if 0
	for(i=0;i<1;i++)
		if( 0 != (ret=pthread_create(&tid[i],NULL,func_arr[i],NULL)))
		{
			return -1;
		}

	for(i=0;i<1;i++)
		pthread_join(tid[i],NULL);
#endif
	return 0;

}
