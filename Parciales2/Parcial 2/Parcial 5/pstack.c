#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "pstack.h"

struct s_pstack {
	struct s_node *front;
	size_t pstack_size;   
};

struct s_node {
	pstack_elem elemento;
	priority_t prioridad;
	struct s_node *next;
};

static struct s_node * create_node(pstack_elem e, priority_t priority) {
    struct s_node *new_node = NULL;
    new_node = malloc(sizeof(struct s_node));
    assert(new_node!=NULL);
	new_node->elemento = e;
	new_node->prioridad = priority;
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
	bool b = (s!=NULL);
    /*if(b && s->front != NULL && s->pstack_size != 0u){
		struct s_node *current = s->front;
		unsigned int pstack_size = 1u;
		while(current->next != NULL){
			b = b && current->prioridad <= current->next->prioridad;
			current = current->next;
			pstack_size = pstack_size + 1u;
		}
		b = b && (s->pstack_size == pstack_size);
	}*/
	return b;
}

pstack pstack_empty(void) {
    pstack s=NULL;
	s = malloc(sizeof(struct s_pstack));
	if(s == NULL){
		return NULL;	
	}
	
	s->front = NULL;
	s->pstack_size = 0;
	assert(invrep(s) && pstack_is_empty(s));
    return s;
}

pstack pstack_push(pstack s, pstack_elem e, priority_t priority) {
    assert(invrep(s));
	
	struct s_node *aux = s->front;
	struct s_node *prev = NULL;
	struct s_node *node = create_node(e, priority);
	if (s->front == NULL)
	{
		s->front = node;
	}else{
		while(aux != NULL && aux->prioridad > node->prioridad){
			prev = aux;
			aux = aux->next;
		}
		node->next = aux;
		if (prev != NULL)
		{
			prev->next = node;
		}else{
			s->front = node;
		}
	}
	s->pstack_size++;
	assert(invrep(s) && !pstack_is_empty(s));
	return s;
}

bool pstack_is_empty(pstack s) {
	return (s->front == NULL);
}

pstack_elem pstack_top(pstack s) {
    assert(invrep(s) && !pstack_is_empty(s));
	
	struct s_node *top_node = s->front;
	pstack_elem e = top_node->elemento;
	
	assert(invrep(s));
    return e;
}

priority_t pstack_top_priority(pstack s) {
	assert(invrep(s) && !pstack_is_empty(s));
	
	struct s_node *top_node = s->front;
	priority_t priority = top_node->prioridad;
	
	assert(invrep(s));
    return priority;
}

unsigned int pstack_size(pstack s) {
    assert(invrep(s));
    unsigned int size=0;
	size = s->pstack_size;
    return size;
}

pstack pstack_pop(pstack s) {
    assert(invrep(s) && !pstack_is_empty(s));
	
	struct s_node *aux = s->front;
	s->front = s->front->next;
	destroy_node(aux);
	s->pstack_size--;
	
	assert(invrep(s));
	return s;
}

pstack pstack_destroy(pstack s) {
    assert(invrep(s));
	
	while(!pstack_is_empty(s)){
		s = pstack_pop(s);
	}
	free(s);
	s = NULL;
	return s;
}

