#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "cn_match.h"
#include "card.h"

struct _s_match {
  card elem;
  struct _s_match *next;
};

cn_match match_new(void) {
  cn_match new_match = NULL;
  return new_match;
}

cn_match match_add(cn_match match, card c) {
  cn_match node = malloc(sizeof(struct _s_match));
  assert(node != NULL);
  node->elem = c;
  node->next = NULL;
  if (match == NULL)
  {
    match = node;
  }else{
    cn_match aux = match;
    while(aux->next != NULL){
      aux = aux->next;
    }
    aux->next = node;
  }
  return match;
}

unsigned int match_size(cn_match match) {
  unsigned int size = 0u;
  cn_match aux = match;
  while(aux != NULL){
    size += 1;
    aux = aux->next;
  }
  return size;
}

unsigned int match_length(cn_match match) {
  unsigned int length = 0u;
  if (!is_match_correct(match))
  {
    return length;
  }
  length = match_size(match) / 2;
  return length;
}

bool is_match_correct(cn_match match) {
  bool valid = true;
  bool pair = (match_size(match) % 2 == 0);
  bool initial_player = card_player(match->elem) == 1;
  cn_match aux = match;
  while(aux->next != NULL){
    if (card_player(aux->elem) == 1)
    {
      valid = card_player(aux->next->elem) == 2 && valid;
    }
    if (card_player(aux->elem) == 2)
    {
      valid = card_player(aux->next->elem) == 1 && valid;
    }
    aux = aux->next;
  }
  valid = valid && pair && initial_player;
  return valid;
}

unsigned int winner_total_points(cn_match match) {
  
  unsigned int player1_points = 0u;
  unsigned int player2_points = 0u;
  unsigned int winner_points = 0u;
  cn_match aux = match;
  if (!is_match_correct(match))
  {
    return winner_points;
  }
  while(aux->next != NULL){
    player1_points += card_pair_points(aux->elem, aux->next->elem, card_player(aux->elem));
    player2_points += card_pair_points(aux->elem, aux->next->elem, card_player(aux->next->elem));
    aux = aux->next;
  }

  if (player1_points > player2_points)
  {
    winner_points = player1_points;
  }else{
    winner_points = player2_points;
  }
  return winner_points;
}

card * match_to_array(cn_match match) {
  card *array = malloc(sizeof(struct _s_match)*match_size(match));
  assert(array != NULL);
  unsigned int length = 0u;
  cn_match aux = match;
  for (unsigned int i = 0; i < match_size(match); i++)
  {
    array[i] = aux->elem;
    length += 1;
    aux = aux->next;
  }

  assert(length == match_size(match));
  return array;
}

void match_dump(cn_match match) {
  card *array = match_to_array(match);
  for (unsigned int i=0u; i < match_size(match); i++) {
    card_dump(array[i]);
  }
  free(array);
}

cn_match match_destroy(cn_match match) {
  cn_match aux = match;
  while(aux != NULL){
    match = match->next;
    aux->elem = card_destroy(aux->elem);
    free(aux);
    aux = match;
  }
  free(match);
  match = NULL;
  assert(match == NULL);
  return match;
}
