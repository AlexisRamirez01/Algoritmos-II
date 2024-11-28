#include "domino_line.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "domino.h"

struct _s_line
{
    struct _line_node *head;
    unsigned int size;
};

struct _line_node
{
    domino tile;
    struct _line_node *next;
};

typedef struct _line_node * node_t;

node_t create_node(domino t){
    node_t new_node = malloc(sizeof(struct _line_node));
    assert(new_node != NULL);
    new_node->tile = t;
    new_node->next = NULL;
    return new_node;
}

node_t destroy_node(node_t node){
    assert(node != NULL);
    node->next = NULL;
    node->tile = domino_destroy(node->tile);
    free(node);
    node = NULL;
    assert(node == NULL);
    return node;
}

domino_line line_new(domino first)
{
   domino_line new_line = malloc(sizeof(struct _s_line));
   assert(new_line != NULL);
   new_line->head = create_node(first);
   new_line->size = 1;
   return new_line;
}

domino_line line_add(domino_line line, domino t)
{
    node_t new_node = create_node(t);
    node_t aux = line->head;
    while(aux->next != NULL){
        aux = aux->next;
    }
    aux->next = new_node;
    line->size++;
    return line;
}

unsigned int line_length(domino_line line)
{
    return line->size;
}

bool line_n_correct(domino_line line, unsigned int n){
    assert(n < line_length(line));

    node_t aux = line->head;
    node_t prev = NULL;
    unsigned int count = 0u;
    bool resp = false;

    if (line_length(line) == 1) //Caso base #1: la línea solo tiene una ficha
    {
        resp = true;
    }else if (n == 0 && aux->next != NULL) //Caso base #2: se quiere chequear si la primera encaja con la segunda ficha
    {
        resp = domino_matches(aux->tile, aux->next->tile);
    }else if (n == line_length(line) - 1) //Caso base #3: se quiere chequear la última con la penultima ficha
    {
        while(aux->next != NULL){
            prev = aux;
            aux = aux->next;
        }
        resp = domino_matches(prev->tile, aux->tile);
    }else{ //Caso complejo: estoy buscando si la n-ésima encaja
        while(aux->next != NULL && count < n){
            count++;
            prev = aux;
            aux = aux->next;
        }
        resp = domino_matches(prev->tile, aux->tile) && domino_matches(aux->tile, aux->next->tile);
    }
    
    return resp;
}
    


int line_total_points(domino_line line)
{
    int points = 0;
    node_t aux = line->head;
    while(aux != NULL){
        points += domino_up(aux->tile) + domino_down(aux->tile);
        aux = aux->next;
    }
    return points;
}

domino* line_to_array(domino_line line)
{
    domino *array = malloc(sizeof(domino)*line_length(line));
    assert(array != NULL);
    unsigned int length = 0u;
    node_t aux = line->head;
    for (unsigned int i = 0; i < line_length(line); i++)
    {
        array[i] = aux->tile;
        aux = aux->next;
        length++;
    }
    assert(length == line_length(line));
    return array;
}

void line_dump(domino_line line)
{
    domino* array = line_to_array(line);
    for (unsigned int i = 0u; i < line_length(line); i++)
    {
        domino_dump(array[i]);
    }
    free(array);
}

domino_line line_destroy(domino_line line)
{
    node_t aux = NULL;
    while(line->head != NULL){
        aux = line->head;
        line->head = line->head->next;
        destroy_node(aux);
    }
    free(line);
    line = NULL;
    assert(line == NULL);
    return line;
}

line_iterator line_iterator_begin(domino_line line)
{
    line_iterator lit = line->head;
    assert(line_length(line) == 0 || lit != NULL);
    return lit;
}

domino line_iterator_get(line_iterator lit)
{
    assert(!line_iterator_end(lit));
    domino elem = lit->tile;
    assert(elem != NULL);
    return elem;
}

line_iterator line_iterator_next(line_iterator lit)
{
    if (lit == NULL || lit->next == NULL)
    {
        return NULL;
    }else{
        lit = lit->next;
    }
    return lit;
}

bool line_iterator_end(line_iterator lit)
{
    bool b = (lit->next == NULL);
    return b;
}
