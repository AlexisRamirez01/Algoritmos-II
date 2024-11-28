#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "pstack.h"

struct s_pstack {
    struct s_node *top;
    unsigned int size;
};

struct s_node {
    pstack_elem elem;
    priority_t priority;
    struct s_node *next;
};

static struct s_node * create_node(pstack_elem e, priority_t priority) {
    struct s_node *new_node = NULL;
    new_node = malloc(sizeof(struct s_node));
    assert(new_node!=NULL);
    new_node->elem = e;
    new_node->priority = priority;
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
    bool resp = (s != NULL);
    return resp;
}

pstack pstack_empty(void) {
    pstack s=NULL;
    s = malloc(sizeof(struct s_pstack));
    assert(s != NULL);
    s->top = NULL;
    s->size = 0u;
    assert(invrep(s) && pstack_is_empty(s));
    return s;
}

pstack pstack_push(pstack s, pstack_elem e, priority_t priority) {
    assert(invrep(s));
    struct s_node *new_node = create_node(e, priority);
    if (s->top == NULL)
    {
        s->top = new_node;
    }else if (new_node->priority > s->top->priority)
    {
        new_node->next = s->top;
        s->top = new_node;
    }else{
        struct s_node *aux = s->top;
        struct s_node *prev = NULL;
        while(aux != NULL && aux->priority > new_node->priority){
            prev = aux;
            aux = aux->next;
        }
        new_node->next = aux;
        if (prev != NULL)
        {
            prev->next = new_node;
        }else{
            s->top = new_node;
        }
    }

    s->size++;
    return s;
}

bool pstack_is_empty(pstack s) {
    bool resp = (s->size == 0);
    return resp;
}

pstack_elem pstack_top(pstack s) {
    assert(invrep(s) && !pstack_is_empty(s));
    pstack_elem e = s->top->elem;
    assert(invrep(s));
    return e;
}

priority_t pstack_top_priority(pstack s) {
    assert(invrep(s) && !pstack_is_empty(s));
    priority_t worst = s->top->priority;
    assert(invrep(s));
    return worst;
}

unsigned int pstack_size(pstack s) {
    assert(invrep(s));
    unsigned int size=0;
    size = s->size;
    return size;
}

pstack pstack_pop(pstack s) {
    struct s_node *aux = s->top;
    s->top = s->top->next;
    destroy_node(aux);
    s->size--;
    return s;
}

pstack pstack_destroy(pstack s) {
    assert(invrep(s));
    while(!pstack_is_empty(s)){
        s = pstack_pop(s);
    }
    free(s);
    s = NULL;
    assert(s == NULL);
    return s;
}

