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
}
typedef void *( *func_type_t)(void*);
sem_t num_elm;
sem_t num_slots;
void printS()
{
	node_t *it =S.top;
	char log[20]={0};
	if(S.top== NULL ) return;
	while(it !=NULL)
	{
		assert(it!= NULL);
		snprintf(log,20,"\t %d ",it->data);
		write_to_thread_log(log);
		it=it->next;
	}
	write_to_thread_log("\n");
	fflush(stdout);
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
	elm->next = S.top;
	S.top =elm;
}

node_t* delete()
{
	node_t *element = S.top;
	if(element == NULL ) return NULL;
	S.top=element->next;
	element->next=NULL;
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
		sem_wait(&num_elm);
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
		sem_post(&num_slots);
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
	 pthread_setspecific (thread_log_key, thread_log);

	srand(time(NULL));
	while(1)
	{
		
		node_t *element =getnode(rand()%MAX_RAND_NUM);
		sem_wait(&num_slots);
		write_to_thread_log("=====================================================================\n");
	fflush(stdout);
		printS();	
		snprintf(log,200,"\n%s%p  inserting %d\n",__func__ ,pthread_self(),element->data);	
		 write_to_thread_log(log);
		insert(element);
		printS();	
		write_to_thread_log("=====================================================================\n");
	fflush(stdout);
		sem_post(&num_elm);
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
	int  g_err1=sem_init(&num_elm, 0, 0) ;
	g_err1=sem_init(&num_slots, 0, 10) ;
	S.top=NULL;

	pthread_key_create (&thread_log_key, close_thread_log);

	for(i=0;i<3;i++)
		if( ret=pthread_create(&tid[i],NULL,func_arr[i],NULL))
		{
			return -1;
		}

	for(i=0;i<3;i++)
		pthread_join(tid[i],NULL);

	return 0;

}
