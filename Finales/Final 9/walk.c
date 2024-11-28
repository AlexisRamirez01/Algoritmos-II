#include <stdio.h>
#include <stdlib.h>
#include "walk.h"
#include <assert.h>


#define STEP_NORTH 0
#define STEP_EAST 1
#define STEP_NORTHEAST 2
#define STEP_UNDEF 3

typedef unsigned int step_t;
typedef unsigned int uint_t;

struct _node_t {
    step_t step;
    struct _node_t *next;
};

typedef struct _node_t *node_t;

struct _walk_t {
    node_t init;
    node_t last;
    uint_t length;
    uint_t width;
    uint_t height;
};

walk_t walk_empty(void) {
    walk_t new_walk = malloc(sizeof(struct _walk_t));
    assert(new_walk != NULL);
    new_walk->init = NULL;
    new_walk->last = NULL;
    new_walk->length = 0u;
    new_walk->width = 0u;
    new_walk->height = 0u;
    assert(walk_length(new_walk) == 0);
    return new_walk;
}

static node_t destroy_node(node_t node){
    assert(node != NULL);
    node->next = NULL;
    free(node);
    node = NULL;
    assert(node == NULL);
    return node;
}

static node_t create_node(void){
    node_t new_node = malloc(sizeof(struct _node_t));
    assert(new_node != NULL);
    new_node->next = NULL;
    new_node->step = STEP_UNDEF;
    return new_node;
}

static walk_t walk_add(walk_t walk , node_t new_node) {
    if (walk->init == NULL)
    {
        walk->init = new_node;
        walk->last = new_node;
    }else{
        node_t aux = walk->init;
        while(aux->next != NULL){
            aux = aux->next;
        }
        aux->next = new_node;
    }
    walk->length++;
    return walk;
}

walk_t walk_north(walk_t walk) {
    node_t new_node = create_node();
    new_node->step = STEP_NORTH;
    walk = walk_add(walk, new_node);
    walk->height++;
    return walk;
}

walk_t walk_east(walk_t walk) {
    node_t new_node = create_node();
    new_node->step = STEP_EAST;
    walk = walk_add(walk, new_node);
    walk->width = walk->width + new_node->step;
    return walk;
}

walk_t walk_northeast(walk_t walk) {
    node_t new_node = create_node();
    new_node->step = STEP_NORTHEAST;
    walk = walk_add(walk, new_node);
    walk->width = walk->width + new_node->step;
    walk->height = walk->height + new_node->step;
    return walk;
}

unsigned int walk_length(walk_t walk) {
    return walk->length;
}

unsigned int walk_height(walk_t walk) {
    return walk->height;
}

unsigned int walk_width(walk_t walk) {
    return walk->width;
}

walk_t walk_extend(walk_t walk, unsigned int east, unsigned int north) {
    for (unsigned int i = 0; i < east; i++)
    {
        walk = walk_east(walk);
    }
    for (unsigned int i = 0; i < north; i++)
    {
        walk = walk_north(walk);
    }
    return walk;
}

static char step_to_char (step_t step) {
    char c = '\0';
    switch (step) {
        case STEP_NORTH : c = 'N'; break;
        case STEP_EAST  : c = 'E' ; break;
        case STEP_NORTHEAST : c = 'X' ; break;
        default : c = 'U' ; break;
    }
    return c;
}

void walk_dump(walk_t walk) {
    assert(walk != NULL);
    node_t current = NULL;
    current = walk -> init;
    printf("[");
    while (current != NULL) {
        printf("%c",step_to_char(current->step));
        if(current->next != NULL) {
            printf(" -> ");
        }
        current = current -> next;
    }
    printf("] (L=%u,W=%u,H=%u)\n",walk->length,walk->width,walk->height);
}

walk_t walk_destroy(walk_t walk) {
    node_t aux = walk->init;
    node_t prev = NULL;
    if (aux == NULL)
        {
            free(walk);
            walk = NULL;
            assert(walk == NULL);
            return walk;
        }

    while(aux->next != NULL){
        prev = aux;
        aux = aux->next;
        prev = destroy_node(prev);
    }
    aux = destroy_node(aux);
    free(walk);
    walk = NULL;
    assert(walk == NULL);
    return(walk);
}

