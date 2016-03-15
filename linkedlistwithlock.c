#include <stdio.h>
#include <mutex.h>


typedef struct node1
{
	int value ;
	struct node1 *ptr;
	mutex_t lock;
} node_t;

typedef node_t *nodeptr_t;

int main()
{
	
}
