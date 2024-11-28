#include <stdlib.h>     /* malloc(), free()... */
#include <stdbool.h>    /* bool type           */
#include <assert.h>     /* assert()            */
#include "key_value.h"  /* key_t               */
#include "dict.h"       /* intarface           */

struct _s_dict {
    struct _node_t * first;
    unsigned int size;
};

// Internal structure
struct _node_t {
    key_t key;
    value_t value;
    struct _node_t *next;
};

typedef struct _node_t * node_t;


node_t create_node(key_t k, value_t v) {
    node_t new_node = malloc(sizeof(struct _node_t));
    assert(new_node != NULL);
    new_node->key = k;
    new_node->value = v;
    new_node->next = NULL;
    return new_node;
}

node_t destroy_node(node_t node) {
    assert(node != NULL);
    node->next = NULL;
    node->key = key_destroy(node->key);
    node->value = value_destroy(node->value);
    free(node);
    node = NULL;
    assert(node == NULL);
    return node;
}

static bool invrep(dict_t dict) {
    bool resp = (dict != NULL);
    return resp;
}
// --- Interface functions ---

dict_t dict_empty(void) {
    dict_t dict = malloc(sizeof(struct _s_dict));
    assert(dict != NULL);
    dict->first = NULL;
    dict->size = 0u;
    assert(invrep(dict) && (dict_length(dict)==0));
    return dict;
}

dict_t dict_add(dict_t dict, key_t word, value_t def) {
    assert(invrep(dict));

    if (dict_exists(dict, word))
    {
        node_t aux = dict->first;
        while(aux != NULL){
            if (key_eq(aux->key, word))
            {
                aux->value = value_destroy(aux->value);
                aux->value = def;
            }
            aux = aux->next;
        }
    }else{
        node_t node = create_node(word, def);
        if (dict->first == NULL)
        {
            dict->first = node;
        }else{
            node->next = dict->first;
            dict->first = node;
        }
        dict->size++;
    }

    assert(invrep(dict) && dict_exists(dict, word));
    return dict;
    
}

value_t dict_search(dict_t dict, key_t word) {
    assert(invrep(dict));

    value_t value = NULL;
    if (!dict_exists(dict, word))
    {
        return value;
    }
    node_t aux = dict->first;
    while(aux != NULL){
        if (key_eq(aux->key, word))
        {
            value = aux->value;
            break;
        }
    }
    
    assert(dict_exists(dict, word));
    return value;
}

bool dict_exists(dict_t dict, key_t word) {
    assert(invrep(dict));

    bool exists = false;
    node_t aux = dict->first;
    while(aux != NULL){
        if (key_eq(aux->key, word))
        {
            exists = true;
        }
        aux = aux->next;
    }
    
    assert(invrep(dict));
    return exists;
}

unsigned int dict_length(dict_t dict) {
    assert(invrep(dict));
    return dict->size;
}

dict_t dict_remove(dict_t dict, key_t word) {
    assert(invrep(dict));

    node_t aux = dict->first;
    node_t prev = dict->first;
    node_t remove = NULL;
    while(aux->next != NULL){
        if (key_eq(aux->key, word))
        {
            remove = aux;
            prev->next = remove->next;
            aux = aux->next;
            remove = destroy_node(remove);
            dict->size--;
        }else{
            prev = aux;
            aux = aux->next;
        }
    }
    
    assert(invrep(dict) && !dict_exists(dict, word));
    return dict;
}

void dict_dump(dict_t dict, FILE *file) {
    assert(invrep(dict));
    for (struct _node_t *check=dict->first; check!=NULL; check=check->next) {
        fprintf(file, "key: ");
        key_dump(check->key, file);
        fprintf(file, "\n");
        fprintf(file, "value: ");
        value_dump(check->value, file);
        fprintf(file, "\n\n");
    }
}

dict_t dict_remove_all(dict_t dict) {
    assert(invrep(dict));

    node_t aux = NULL;
    while(dict->first != NULL){
        aux = dict->first;
        dict->first = dict->first->next;
        destroy_node(aux);
        dict->size--;
    }
    

    assert(invrep(dict) && dict_length(dict)==0);
    return dict;
    
}

dict_t dict_destroy(dict_t dict) { 
    assert(invrep(dict));
    
    dict = dict_remove_all(dict);
    free(dict);
    dict = NULL;
    assert(dict == NULL);
    return dict;
}

