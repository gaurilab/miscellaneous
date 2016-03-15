typedef struct
{
 int data;
} request_t;

request_t *get_request()
{
	request_t *request;
	request = (request_t *) malloc(sizeof(request_t));
	request->data = read_from_net();
	return(request)
}
void process_request(request_t *request)
{ 
process(request->data);
	free(request);
}
void* producer(void *arg)
{request_t *request;
	while(1)
	{request = get_request();
		add(request);
		sem_post(&requests_length);
	}
}
void* consumer(void *arg)
{request_t *request;
	while(1)
	{SEM_WAIT(&requests_length);
		request = remove();
		process_request(request);
	}
}
