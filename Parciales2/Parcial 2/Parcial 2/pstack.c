#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include "pstack.h"

struct s_pstack {
 	struct s_node **array;
	unsigned int max_priority;
	size_t size;  
};

struct s_node {
	pstack_elem elem;
	struct s_node *next;
};



static struct s_node * create_node(pstack_elem e) {
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


static bool invrep(pstack s) {
	bool b = (s != NULL);
	return b;
}

pstack pstack_empty(priority_t max_priority) {
	pstack s = malloc(sizeof(struct s_pstack));
	assert(s != NULL);
	s->array = calloc(max_priority+1, sizeof(struct s_node));
	assert(s->array != NULL);
	s->size = 0u;
	s->max_priority = max_priority;
	
	assert(invrep(s) && pstack_is_empty(s));
    return s;
}

pstack pstack_push(pstack s, pstack_elem e, priority_t priority) {
    assert(invrep(s));
	
	struct s_node *new_node = create_node(e);
	new_node->next = s->array[priority];
	s->array[priority] = new_node;
	s->size++;
	
	assert(invrep(s) && !pstack_is_empty(s));
    return s;
}

bool pstack_is_empty(pstack s) {
	bool b = (s->size == 0);
    return b;
}

pstack_elem pstack_top(pstack s) {
    assert(invrep(s) && !pstack_is_empty(s));

    pstack_elem e = 0u;
	struct s_node *aux = NULL;
	priority_t priority_top = 0u;
	for (unsigned int i = 0; i <= s->max_priority; i++)
	{
		aux = s->array[i];
		if (aux != NULL)
		{
			priority_top = i;
		}
	}

	e = s->array[priority_top]->elem;


	assert(invrep(s));
	return e;
}

priority_t pstack_top_priority(pstack s) {
    assert(invrep(s) && !pstack_is_empty(s));
    
    priority_t priority_top = 0u;
    struct s_node *aux = NULL;
    for (unsigned int i = 0; i <= s->max_priority; i++)
    {
    	aux = s->array[i];
    	if (aux != NULL)
    	{
    		priority_top = i;
    	}
    }

	assert(invrep(s));
	return priority_top;
}	

size_t pstack_size(pstack s) {
    assert(invrep(s));

    size_t length = s->size;
    return length;
}

pstack pstack_pop(pstack s) {
    assert(invrep(s) && !pstack_is_empty(s));
	
	priority_t priority_top = pstack_top_priority(s);
	struct s_node *remove = s->array[priority_top];
	s->array[priority_top] = s->array[priority_top]->next;
	destroy_node(remove);
	s->size--; 
	
	return s;
}

pstack pstack_destroy(pstack s) {
    assert(invrep(s));

    while(!pstack_is_empty(s)){
    	s = pstack_pop(s);
    }

    free(s->array);
    free(s);
    s = NULL;
    assert(s == NULL);
	return s;
}
