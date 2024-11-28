#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "set.h"

typedef int elem_t;

/*static bool elem_lt(elem_t a, elem_t b) {
    return (a < b);
}
*/
static bool elem_eq(elem_t a, elem_t b) {
    return (a == b);
}

static elem_t elem_clone(elem_t a) {
    return (a);
}

static elem_t elem_destroy(elem_t e) {
    return (e);
}

/*static void elem_dump(elem_t e, FILE *fd) {
    fprintf(fd, "%d", e);
}
*/
struct node_t_ {
    elem_t elem;
    struct node_t_ *next;
};

struct set_t_ {
    struct node_t_ *line;
    unsigned int size;
};

typedef struct node_t_ *node_t;

static node_t create_node(elem_t elem, node_t next) {
    node_t new_node = malloc(sizeof(struct node_t_));
    assert(new_node != NULL);
    new_node->elem = elem;
    new_node->next = next;
    return new_node;
}

static node_t destroy_node(node_t node) {
    assert(node != NULL);
    node->next = NULL;
    node->elem = elem_destroy(node->elem);
    free(node);
    node = NULL;
    assert(node == NULL);
    return node;
}


set_t set_empty(void) {
    set_t new_set = malloc(sizeof(struct set_t_));
    assert(new_set != NULL);
    new_set->line = NULL;
    new_set->size = 0u;
    return new_set;
}

set_t set_add(set_t set, int element) {
    
    if (set_belongs(set, element))
    {
        return set;
    }    
    node_t new_node = create_node(element, set->line);
    set->line = new_node;
    set->size++;
    return set;
}

bool set_is_empty(set_t set) {
    bool resp = (set->size == 0);
    return resp;
}

bool set_belongs(set_t set, int element) {
    node_t aux = set->line;
    bool belongs = false;
    if (set_is_empty(set))
    {
        return belongs;
    }
    while(aux != NULL){
        if (elem_eq(aux->elem, element))
        {
            belongs = true;
            break;
        }
        aux = aux->next;
    }

    return belongs;
}

unsigned int set_size(set_t set) {
    return set->size;
}
/*
 * Adds a clone of the given element to the set or frees the element if it already exists in the set.
 * 
 * @param set The set to add the element to.
 * @param elem The element to add to the set.
 * @return The updated set after adding the element.
 
static set_t add_clone_or_free(set_t set, elem_t elem) {

}
*/
static set_t set_clone(set_t set) {
    set_t copy = set_empty();
    
    if (set_is_empty(set))
    {
        return copy;
    }

    copy->line = create_node(set->line->elem, NULL);
    copy->line->elem = elem_clone(set->line->elem);

    node_t aux = set->line;
    node_t tmp = copy->line;

    while(aux->next != NULL){
        aux = aux->next;
        tmp->next = create_node(aux->elem, NULL);
        tmp->next->elem = elem_clone(aux->elem);
        tmp = tmp->next;
    }

    copy->size = set->size;
    return copy;
}


set_t set_union(set_t set1, set_t set2) {
    
    set_t set = set_clone(set1);
    if (set_is_empty(set2))
    {
        return set;
    }
    node_t aux = set2->line;
    while(aux != NULL){
        set_add(set, aux->elem);
        aux = aux->next;
    }

    return set;

}

set_t set_intersect(set_t set1, set_t set2) {
    set_t set = set_empty();
    node_t aux = set1->line;
    if (set_is_empty(set2) || set_is_empty(set1))
    {
        return set;
    }
    while(aux != NULL){
        if (set_belongs(set2, aux->elem))
        {
            set = set_add(set, aux->elem);
        }
        aux = aux->next;
    }
    return set;
}

set_t set_destroy(set_t set) {
    node_t aux = NULL;
    while(set->line != NULL){
        aux = set->line;
        set->line = set->line->next;
        destroy_node(aux);
    }
    free(set);
    set = NULL;
    assert(set == NULL);
    return set;
}

void set_dump(set_t set, FILE *fd) {
    assert(set != NULL);
    node_t current = set->line;
    fprintf(fd, "{");
    while (current != NULL) {
        fprintf(fd, "%d", current->elem);
        current = current->next;
        if (current != NULL) {
            fprintf(fd, ",");
        }
    }
    fprintf(fd, "}");
}
