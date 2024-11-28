#include <stdlib.h>
#include <assert.h>

#include "stack.h"

#define CHUNK_CAPACITY 5

struct s_node {
    stack_elem *chunk;
    unsigned int used;
    struct s_node *next;
};

struct _s_stack {
   struct s_node *top;
   unsigned int size;
};

static bool
valid_chunks(stack s) {
    bool valid = true;
    struct s_node *aux = s->top;
    if (aux != NULL)
    {
        valid = valid && aux->chunk != NULL;
        valid = valid && aux->used <=CHUNK_CAPACITY;
    }
    return valid;
}

static bool
invrep(stack s) {
    bool resp = true;
    resp = (s != NULL);
    unsigned int length = 0u;
    struct s_node *aux = s->top;
    while(aux != NULL){
        length += aux->used;
        aux = aux->next;
    }
    resp = resp && (length == s->size);
    return (resp && valid_chunks(s));
    
}

static struct s_node *
create_node(void) {
    struct s_node *newNode = malloc(sizeof(struct s_node));
    assert(newNode != NULL);
    newNode->next = NULL;
    newNode->chunk = calloc(CHUNK_CAPACITY, sizeof(stack_elem));
    assert(newNode->chunk != NULL);
    newNode->used = 0u;

    return newNode;
}


static struct s_node *
destroy_node(struct s_node *node) {
    assert(node != NULL);
    node->next = NULL;
    free(node->chunk);
    free(node);
    node = NULL;
    assert(node == NULL);

    return node;
}

stack stack_empty(void) {
    stack new_s = malloc(sizeof(struct _s_stack));
    new_s->top = create_node();
    new_s->size = 0u;

    assert(invrep(new_s) && stack_is_empty(new_s));
    return new_s;
}


stack stack_push(stack s, stack_elem e) {
    assert(invrep(s));
    struct s_node *aux = s->top;
    if (aux->used == CHUNK_CAPACITY)
    {
        s->top = create_node();
        s->top->next = aux;
    }
    aux->chunk[aux->used] = e;
    aux->used++;
    s->size++;

    assert(invrep(s) && !stack_is_empty(s));
    return s;
}


stack stack_pop(stack s) {
    assert(invrep(s) && !stack_is_empty(s));

    struct s_node *aux = s->top;
    while(aux->next != NULL){
        aux = aux->next;
    }
    aux->used--;
    s->size--;
    if (s->size > 0 && aux->used == 0)
    {
        destroy_node(aux);
    }
   
    assert(invrep(s));
    return s;
}


unsigned int stack_size(stack s) {
    return s->size;
}

void stack_top(stack s, stack_elem *top) {
    assert(invrep(s) && !stack_is_empty(s));
    *top = s->top->chunk[s->top->used];
}

bool stack_is_empty(stack s) {
    return (s->size == 0);
}


stack_elem *stack_to_array(stack s) {
    
    stack_elem *array = calloc(s->size, sizeof(stack_elem));
    assert(array != NULL);

    struct s_node *aux = s->top;
    int pos = s->size-1;

    if (stack_size(s) == 0)
    {
        return array;
    }

    while(aux != NULL){
        for (int i = aux->used-1; i >= 0 ; --i)
        {
            array[pos] = aux->chunk[i];
            --pos;
        }
        aux = aux->next;
    }

    return array;
}
  



stack stack_destroy(stack s) {

    struct s_node *aux = NULL;
    while(s->top != NULL){
        aux = s->top;
        s->top = s->top->next;
        aux = destroy_node(aux);
    }
    free(s);
    s = NULL;
    assert(s == NULL);
    return s;
}