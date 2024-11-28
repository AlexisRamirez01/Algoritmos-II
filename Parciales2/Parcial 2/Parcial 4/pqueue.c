#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "pqueue.h"

struct s_pqueue {
    struct s_node **array;
    unsigned int min_priority;
    unsigned int psize;
};

struct s_node {
    pqueue_elem elem;
    struct s_node *next;
};

static struct s_node * create_node(pqueue_elem e) {
	struct s_node *new_node = malloc(sizeof(struct s_node));
	assert(new_node != NULL);
	new_node->elem = e;
	new_node->next = NULL;

    return new_node;
}

static struct s_node * destroy_node(struct s_node *node) {
	assert(node != NULL);
	node->next = NULL;
	free(node);
	node = NULL;
	assert(node == NULL);
   	return node;
}


static bool invrep(pqueue q) {
    bool resp = (q != NULL);
    return resp;
}

pqueue pqueue_empty(priority_t min_priority) {
    pqueue new_pqueue = malloc(sizeof(struct s_pqueue));
    assert(new_pqueue != NULL);
    new_pqueue->array = calloc(min_priority+1, sizeof(struct s_node));
    assert(new_pqueue->array != NULL);
    new_pqueue->min_priority = min_priority;
    new_pqueue->psize = 0u;

    assert(invrep(new_pqueue));
    return new_pqueue;
}

pqueue pqueue_enqueue(pqueue q, pqueue_elem e, priority_t priority) {
    assert(invrep(q));
	
	struct s_node *new_node = create_node(e);
	if (q->array[priority] == NULL)
	{
		q->array[priority] = new_node;
	}else{
		struct s_node *aux = q->array[priority];
		while(aux->next != NULL){
			aux = aux->next;
		}
		aux->next = new_node;
	}
	q->psize++;
	
    assert(invrep(q) && !pqueue_is_empty(q));
    return q;
}

bool pqueue_is_empty(pqueue q) {
	bool resp = (q->psize == 0);
	return resp;    
}

pqueue_elem pqueue_peek(pqueue q) {
    assert(invrep(q) && !pqueue_is_empty(q));
	
	unsigned int j = 0u;
	pqueue_elem e = 0u;
	struct s_node *aux = NULL;
	for (int i = q->min_priority; i >= 0; i--)
	{
		aux = q->array[i];
		if (aux != NULL)
		{
			j = i;
		}
	}

	e = q->array[j]->elem;
	
	assert(invrep(q));
	return e;

}

priority_t pqueue_peek_priority(pqueue q) {
    assert(invrep(q) && !pqueue_is_empty(q));
	
	priority_t j = 0u;
	struct s_node *aux = NULL;
	for (int i = q->min_priority; i >= 0; i--)
	{
		aux = q->array[i];
		if (aux != NULL)
		{
			j = i;
		}
	}
	
	assert(invrep(q));
	return j;
}

size_t pqueue_size(pqueue q) {
    assert(invrep(q));
    return q->psize;
    
}

pqueue pqueue_dequeue(pqueue q) {
    assert(invrep(q) && !pqueue_is_empty(q));
    
    priority_t min_priority = pqueue_peek_priority(q);
    struct s_node *aux = q->array[min_priority];
    q->array[min_priority] = q->array[min_priority]->next;
    destroy_node(aux);
    q->psize--;
	
    assert(invrep(q));
    return q;
}

pqueue pqueue_destroy(pqueue q) {
    assert(invrep(q));
	
	while(!pqueue_is_empty(q)){
		q = pqueue_dequeue(q);
	}
	free(q->array);
	free(q);
	q = NULL;
	assert(q == NULL);
	return q;
}

