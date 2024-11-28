#include <stdlib.h>
#include <assert.h>
#include "fence.h"

struct _fence_t {
    fence_t next;
    type_t type;
};

fence_t fence_P(){
    fence_t new_fence = malloc(sizeof(struct _fence_t));
    assert(new_fence != NULL);
    new_fence->type = P;
    new_fence->next = NULL;
    return new_fence;
}

fence_t fence_add_P(fence_t fence){
    fence_t new_node_P = malloc(sizeof(struct _fence_t));
    assert(new_node_P != NULL);
    new_node_P->type = P;
    new_node_P->next = NULL;
    if (fence == NULL)
    {
        fence = new_node_P;
    }else{
        fence_t aux = fence;
        while(aux->next != NULL){
            aux = aux->next;
        }
        aux->next = new_node_P;
    }
    return fence;
}

fence_t fence_add_A(fence_t fence){
    fence_t new_node_A = malloc(sizeof(struct _fence_t));
    assert(new_node_A != NULL);
    new_node_A->type = A;
    new_node_A->next = NULL;
    if (fence == NULL)
    {
        fence = new_node_A;
    }else{
        fence_t aux = fence;
        while(aux->next != NULL){
            aux = aux->next;
        }
        aux->next = new_node_A;
    }
    return fence;
}

fence_t fence_add_H(fence_t fence){
    fence_t new_node_H = malloc(sizeof(struct _fence_t));
    assert(new_node_H != NULL);
    new_node_H->type = H;
    new_node_H->next = NULL;
    if (fence == NULL)
    {
        fence = new_node_H;
    }else{
        fence_t aux = fence;
        while(aux->next != NULL){
            aux = aux->next;
        }
        aux->next = new_node_H;
    }
    return fence;
}

fence_t fence_add_T(fence_t fence){
    fence_t new_node_T = malloc(sizeof(struct _fence_t));
    assert(new_node_T != NULL);
    new_node_T->type = T;
    new_node_T->next = NULL;
    if (fence == NULL)
    {
        fence = new_node_T;
    }else{
        fence_t aux = fence;
        while(aux->next != NULL){
            aux = aux->next;
        }
        aux->next = new_node_T;
    }
    return fence;
}

bool fence_is_correct(fence_t fence){
    bool resp = false;
    struct _fence_t *aux = fence;
    struct _fence_t *prev = NULL;

    while(aux != NULL){
        if (aux->type == P)
        {
            resp = true;
            if (resp && prev->type != P)
            {
                resp = false;
            }
            if (resp && aux->next->type != P)
            {
                resp = false;
            }
            if (resp && prev == NULL)
            {
                resp = false;
            }
        }
        
    prev = aux;
    aux = aux->next;
    }
return resp;
}
         

unsigned int fence_perimeter(fence_t fence){
    unsigned int perimeter = 0u;
    fence_t aux = fence;
    while(aux != NULL){
        if (aux->type != P)
        {
            perimeter = perimeter + 3;
        }
        aux = aux->next;
    }
    return perimeter;
}

unsigned int fence_size(fence_t fence){
    unsigned int size = 0u;
    fence_t aux = fence;
    while(aux != NULL){
        size += 1;
        aux = aux->next;
    }
    return size;
}

type_t *fence_to_array(fence_t fence){
    type_t *array = malloc(sizeof(type_t)*fence_size(fence));
    assert(array != NULL);
    fence_t aux = fence;
    for (int i = fence_size(fence)-1; i >= 0; i--)
    {
        array[i] = aux->type;
        aux = aux->next;
    }
    return array;
}


fence_t fence_destroy(fence_t fence){
    fence_t aux = fence;
    while(aux != NULL){
        fence = fence->next;
        free(aux);
        aux = fence;
    }
    return fence;
}