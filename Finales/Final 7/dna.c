#include "dna.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

struct _node {
    nucleobase_t elem;
    struct _node *next;
};

struct _dna_t
{
    struct _node* chain;
    unsigned int length;
};

static struct _node *destroy_node(struct _node *nodo){
    assert(nodo!=NULL);
    nodo->next = NULL;
    free(nodo);
    nodo = NULL;
    assert(nodo == NULL);
    return nodo;
}

static struct _node* single_node(nucleobase_t e)
{
    struct _node *node = malloc(sizeof(struct _node));
    assert(node != NULL);
    node->elem = e;
    node->next = NULL;
    return (node);
}

/*******FUNCION AUXILIAR**********/
static dna_t dna_empty(nucleobase_t e){
    struct _node *new_node = single_node(e);
    dna_t new_dna = malloc(sizeof(struct _dna_t));
    assert(new_dna != NULL);
    new_dna->chain = new_node;
    new_dna->length = 1u;
    return new_dna;
}

dna_t dna_T()
{
    dna_t dna = dna_empty(T);
    return dna;
}

dna_t dna_C()
{
    dna_t dna = dna_empty(C);
    return dna;
}

dna_t dna_A()
{
    dna_t dna = dna_empty(A);
    return dna;
}

dna_t dna_G()
{
    dna_t dna = dna_empty(G);
    return dna;
}


dna_t dna_join(dna_t first, dna_t second)
{
    assert(first != NULL && second != NULL);
    if (first->chain == NULL)
    {
        first->chain = second->chain;
    }else{
        struct _node *aux = first->chain;
        while(aux->next != NULL){
            aux = aux->next;
        }
        aux->next = second->chain;
    }
    first->length = first->length + second->length;
    return first;
}
    


unsigned int dna_length(dna_t dna)
{
    unsigned int res = 0u;
    res = dna->length;
    return res;
}

void dna_print(dna_t dna)
{
    struct _node *aux = dna->chain;
    while(aux != NULL){
        printf(" %c",aux->elem);
        aux = aux->next;
    }
}

bool dna_is_prefix(dna_t first, dna_t second)
{
    bool is_prefix = false;
    struct _node *aux1 = first->chain;
    struct _node *aux2 = second->chain;
    unsigned int equal_elem = 0u;
    if (first->length <= second->length)
    {
        while(aux1 != NULL && aux2 != NULL){
            if (aux1->elem == aux2->elem)
            {
                equal_elem += 1;
            }
            aux1 = aux1->next;
            aux2 = aux2->next;
        }
        is_prefix = (equal_elem == first->length);
    }
    return is_prefix;
}

bool dna_is_equal(dna_t first, dna_t second)
{
    bool is_equal = true;
    struct _node *aux = first->chain;
    struct _node *tmp = second->chain;
    while(aux->next != NULL && tmp->next != NULL){
        if (aux->elem != tmp->elem)
        {
            is_equal = false;
            break;
        }
        aux = aux->next;
        tmp = tmp->next;
    }

    is_equal = is_equal && first->length == second->length;
    return is_equal;
}

dna_t *dna_cut(dna_t dna, unsigned int count) {
    assert(dna != NULL);
    assert(count > 0 && count < dna->length);

    // Crear un arreglo para almacenar los dos segmentos resultantes
    dna_t *res = calloc(2, sizeof(struct _dna_t));
    assert(res != NULL);

    for (unsigned int i = 0; i < 2; i++)
    {
        res[i] = malloc(sizeof(struct _dna_t));
        assert(res[i] != NULL);
        res[i]->chain = NULL;
        res[i]->length = 0u;
    }

    // Variables auxiliares para recorrer la lista original
    struct _node *aux = dna->chain;
    unsigned int c = 0;
    struct _node *tmp = NULL;

    // Primera parte: copiar los primeros `count` nodos a res[0]
    while (aux != NULL && c < count) {
        if (res[0]->chain == NULL) {
            tmp = single_node(aux->elem);
            res[0]->chain = tmp;
            res[0]->length++;
        } else {
            struct _node *new_node = single_node(aux->elem); 
            res[0]->chain->next = new_node;
            res[0]->chain = new_node;
            res[0]->length++;
        }
        aux = aux->next;
        c++;
    }

    // Segunda parte: copiar los nodos restantes a res[1]
    while (aux != NULL) {
        if (res[1]->chain == NULL) {
            res[1]->chain = single_node(aux->elem);
            res[1]->length++;
        } else {
            struct _node *new_node = single_node(aux->elem);
            res[1]->chain->next = new_node;
            res[1]->chain = new_node;
            res[1]->length++;
        }
        aux = aux->next;
    }

    return res;
}


dna_t dna_destroy(dna_t dna) {
    struct _node *aux = NULL;
    while(dna->chain->next != NULL){
        aux = dna->chain;
        dna->chain = dna->chain->next;
        aux = destroy_node(aux);
    }
    free(dna);
    dna = NULL;
    assert(dna == NULL);
    return dna;
}


/*Observaciones generales:
    *En primer lugar, noto que falta liberar mucha memoria y creo que puede ser por un mal 
     funcionamiento de dna_destroy().
    *En segundo lugar, creo que aunque dna_cut() está en un buen camino, deja bastantes
     memory leaks lo que es un problema grandisimo.
    *En tercer lugar, el TestJoin1 falla por consola pero por valgrind está correcto lo que
     me deja la duda de si la lógica usada por dna_join() esta correcta.
    *Por último, añadí dna_empty() para modularizar el código aunque tengo la sospecha que
     también es algo que jode bastante durante la liberación de memori.
*/