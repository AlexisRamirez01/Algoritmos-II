#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "character.h"
#include "pqueue.h"

/* ============================================================================
STRUCTS!
============================================================================ */

struct s_pqueue {
  struct s_node *front;
  unsigned int size;
};

struct s_node {
  Character character;
  float priority;
  struct s_node *next;
};

/* ============================================================================
INVREP
============================================================================ */

static bool invrep(pqueue q) {
  bool resp = (q != NULL);
  
  return resp;
}

/* ============================================================================
NEW
============================================================================ */

pqueue pqueue_empty(void) {
  pqueue q = malloc(sizeof(struct s_pqueue));
  assert(q != NULL);
  q->front = NULL;
  q->size = 0u;
  assert(invrep(q) && pqueue_is_empty(q));
  return q;
}

/* ============================================================================
ENQUEUE
============================================================================ */

static float calculate_priority(Character character) {
  float Alive = 0.0;
  float modificator = 0.0;
  float baseIniciative = 0.0;
  float iniciative = 0.0;

  if (character_ctype(character) == agile)
  {
    modificator = 1.5;
  }else if (character_ctype(character) == tank)
  {
    modificator = 0.8;
  }else{
    modificator = 1.0;
  }

  if (character_is_alive(character))
  {
    Alive = 1.0;
  }else{
    Alive = 0.0;
  }

  baseIniciative = character_agility(character);

  iniciative = baseIniciative * modificator * Alive;

  return iniciative;
}

static struct s_node *create_node(Character character) {
  struct s_node *new_node = malloc(sizeof(struct s_node));
  assert(new_node != NULL);
  new_node->priority = calculate_priority(character);
  new_node->character = character;
  new_node->next = NULL;
  return new_node;
}

pqueue pqueue_enqueue(pqueue q, Character character) {
  assert(invrep(q));
  
  struct s_node *node = create_node(character);
  if (q->front == NULL)
  {
    q->front = node;
  }else if (node->priority > q->front->priority)
  {
    node->next = q->front;
    q->front = node;
  }else{
    struct s_node *aux = q->front;
    node->next = q->front;
    while(node->next != NULL && node->next->priority >= node->priority){
      node->next = (node->next)->next;
      if (aux->next != node->next)
      {
        aux = aux->next;
      }
    }
    aux->next = node;
  }

  q->size++;
  assert(invrep(q) && !pqueue_is_empty(q));
  return q;
}

/* ============================================================================
IS EMPTY?
============================================================================ */

bool pqueue_is_empty(pqueue q) {
  assert(invrep(q));
  bool resp = (q->size == 0);
  return resp;
  
}

/* ============================================================================
PEEKS
============================================================================ */

Character pqueue_peek(pqueue q) {
  assert(invrep(q) && !pqueue_is_empty(q));

  Character character = q->front->character;
  
  assert(invrep(q));
  return character;
}

float pqueue_peek_priority(pqueue q) {
  assert(invrep(q) && !pqueue_is_empty(q));

  float priority = q->front->priority;
  
  assert(invrep(q));
  return priority;
}

/* ============================================================================
SIZE
============================================================================ */

unsigned int pqueue_size(pqueue q) {
  assert(invrep(q));
  
  unsigned int size = q->size;
  
  assert(invrep(q));
  return size;
}

/* ============================================================================
COPY
============================================================================ */

pqueue pqueue_copy(pqueue q) {
  assert(invrep(q));

  pqueue copy = pqueue_empty();
  copy->size = q->size;
  copy->front = create_node(q->front->character);
  copy->front->character = character_copy(q->front->character);

  struct s_node *aux = q->front;
  struct s_node *tmp = copy->front;
  while(aux->next != NULL){
    aux = aux->next;
    tmp->next = create_node(aux->character);
    tmp->next->character = character_copy(aux->character);
    tmp = tmp->next;
  }


  assert(invrep(q) && invrep(copy));
  return copy;
}

/* ============================================================================
DESTROY!
============================================================================ */
static struct s_node *destroy_node(struct s_node *node) {
  assert(node != NULL);

  node->next = NULL;
  node->character = character_destroy(node->character);
  free(node);
  node = NULL;
  assert(node == NULL);
  return node;
}

pqueue pqueue_dequeue(pqueue q) {
  assert(invrep(q) && !pqueue_is_empty(q));
  
  struct s_node *aux = q->front;
  q->front = q->front->next;
  destroy_node(aux);
  q->size--;

  assert(invrep(q));
  return q;
}

pqueue pqueue_destroy(pqueue q) {
  assert(invrep(q));

  while(!pqueue_is_empty(q)){
    q = pqueue_dequeue(q);
  }

  free(q);
  q = NULL;
  assert(q == NULL);

  return q;
}
