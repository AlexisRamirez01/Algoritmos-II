#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <assert.h>

#include "stringbuffer.h"

struct s_stringbuffer {
    struct s_node *first;
    struct s_node *last;
    unsigned int size;
};

struct s_node {
    char c;
    struct s_node *nextchar;
};

typedef struct s_node * node_t;

// Representation invariant
static bool invrep(stringbuffer sb) {
    bool resp = (sb != NULL);
    return resp;
}


// Creates a single node in dynamic memory with a given char
static node_t create_node(char c) {
    node_t newnode = malloc(sizeof(struct s_node));
    assert(newnode != NULL);
    newnode->c = c;
    newnode->nextchar = NULL;

    return newnode;
}


// Destroy a single node
static node_t destroy_node(node_t node) {
    assert(node != NULL);
    node->nextchar = NULL;
    free(node);
    node = NULL;
    assert(node == NULL);
    return node;
}


// Public functions of the Abstract Data Type
stringbuffer stringbuffer_empty(void) {
    stringbuffer new_sb = malloc(sizeof(struct s_stringbuffer));
    assert(new_sb != NULL);
    new_sb->first = NULL;
    new_sb->last = NULL;
    new_sb->size = 0u;
    
    assert(invrep(new_sb) && stringbuffer_is_empty(new_sb));
    return new_sb;
}


stringbuffer stringbuffer_create(const char *word) {
    assert(word != NULL);

    node_t newnode = NULL;
    node_t first = NULL;
    node_t last = NULL;


    stringbuffer sb = stringbuffer_empty();
    sb->size = strlen(word);

    newnode = create_node(word[0]);
    first = newnode;
    sb->first = first;
    sb->last = first;
    for (unsigned int i = 1; i < strlen(word); i++)
    {
        last = create_node(word[i]);
        sb->last->nextchar = last;
        sb->last = last;
    }

    assert(invrep(sb));
    return sb;
}


bool stringbuffer_is_empty(stringbuffer sb){
    assert(invrep(sb));
    return (sb->first == NULL && sb->last == NULL);
}


stringbuffer stringbuffer_append(stringbuffer sb, const char c) {
    assert(invrep(sb));

    node_t newnode = create_node(c);
    if (sb->first == NULL)
    {
        sb->first = newnode;
        sb->last = newnode;
    }else{
        sb->last->nextchar = newnode;
        sb->last = newnode;
    }
    sb->size++;
    
    assert(invrep(sb) && !stringbuffer_is_empty(sb));
    return sb;
}


char stringbuffer_char_at(stringbuffer sb, unsigned int index) {
    assert(invrep(sb) && !stringbuffer_is_empty(sb) && index < stringbuffer_length(sb));

    node_t aux = sb->first;
    for (unsigned int i = 0; i < index; i++)
    {
        aux = aux->nextchar;
    }
    assert(invrep(sb));
    return aux->c;
    
}


stringbuffer stringbuffer_remove(stringbuffer sb, unsigned int index) {
    assert(invrep(sb));

    node_t aux = sb->first;
    node_t prev = NULL;
    if (index == 0)
    {
        sb->first = sb->first->nextchar;
        destroy_node(aux);
    }else
    {
        prev = sb->first;
        for (unsigned int i = 0; i < index; i++)
        {
            prev = aux;
            aux = aux->nextchar;
        }
        prev->nextchar = aux->nextchar;
        destroy_node(aux);
    }
    sb->size--;
    return sb;
}


stringbuffer stringbuffer_replace(stringbuffer sb, const char c, unsigned int index) {
    assert(invrep(sb) && index < stringbuffer_length(sb));

    node_t aux = sb->first;
    for (unsigned int i = 0; i < index; i++)
    {
        aux = aux->nextchar;
    }
    aux->c = c;
    assert(invrep(sb));
    return sb;
}


unsigned int stringbuffer_length(stringbuffer sb) {
    assert(invrep(sb));
    return sb->size;
}


char* stringbuffer_to_string(stringbuffer sb) {
    assert(invrep(sb));

    char *array = calloc(stringbuffer_length(sb)+1, sizeof(struct s_node));
    assert(array != NULL);
    node_t aux = sb->first;
    for (unsigned int i = 0; i < stringbuffer_length(sb); i++)
    {
        array[i] = aux->c;
        aux = aux->nextchar;
    }
    
    assert((stringbuffer_length(sb) == 0) == (array == NULL));
    return array;
}


void stringbuffer_dump(stringbuffer sb) {
    assert(invrep(sb));
    node_t node = sb->first;
    while (node != NULL) {
        printf("%c", node->c);
        node = node->nextchar;
    }
    printf("\n");
}


stringbuffer stringbuffer_destroy(stringbuffer sb) {
    assert(invrep(sb));
    node_t aux = NULL;
    while(sb->first->nextchar != NULL && sb->first != sb->last){
        aux = sb->first;
        sb->first = sb->first->nextchar;
        destroy_node(aux);
    }
    free(sb->first);
    free(sb);
    sb = NULL;
    assert(sb == NULL);
    return sb;
}
