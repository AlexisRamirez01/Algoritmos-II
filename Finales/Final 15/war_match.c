#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "war_match.h"
#include "card.h"

struct _s_match {
  card elem;
  struct _s_match *next;
};

war_match match_new(void) {
  war_match new_match = NULL;
  return new_match;
}

war_match match_add(war_match match, card c) {
  war_match node = malloc(sizeof(struct _s_match));
  assert(node != NULL);
  node->elem = c;
  node->next = NULL;
  if (match == NULL)
  {
    match = node;
  }else{
    war_match aux = match;
    while(aux->next != NULL){
      aux = aux->next;
    }
    aux->next = node;
  }
  return match;
}

unsigned int match_size(war_match match) {
  unsigned int size = 0u;
  war_match aux = match;
  while(aux != NULL){
    size += 1;
    aux = aux->next;
  }
  return size;
}

unsigned int match_length(war_match match) {
  unsigned int rounds_played = 0u;
  if (!is_match_correct(match))
  {
    return rounds_played;
  }
  rounds_played = match_size(match)/2 ;
  return rounds_played;
}

bool is_match_correct(war_match match) {
  bool valid = true;
  bool initial_player = card_player(match->elem) == 1;
  bool pair = (match_size(match) % 2 == 0);
  war_match aux = match;
  while(aux->next != NULL && valid){
    if (card_player(aux->elem) == 1)
    {
      valid = (card_player(aux->next->elem) == 2);
    }else if (card_player(aux->elem) == 2)
    {
      valid = (card_player(aux->next->elem) == 1);
    }
    aux = aux->next;
  }
  valid = valid && initial_player && pair;
  return valid;
}

static unsigned int player_score(war_match match, unsigned int player) {
  unsigned int player_points = 0u;
  war_match aux = match;
  war_match prev = NULL;
  if (player == 1)
  {
    while(aux->next != NULL){
      if (card_player(aux->elem) == 1)
      {
        player_points += card_match_points(aux->elem, aux->next->elem);
      }
      aux = aux->next;
    }
  }else{
    while(aux->next != NULL){
      prev = aux;
      aux = aux->next;
      if (card_player(aux->elem) == 2)
      {
        player_points += card_match_points(aux->elem, prev->elem);
      }
    }
  }
  return player_points;
}

unsigned int winner_total_points(war_match match) {
  unsigned int winner_points = 0u;
  unsigned int player1_points = 0u;
  unsigned int player2_points = 0u;
  if (!is_match_correct(match))
  {
    return winner_points;
  }
  player1_points = player_score(match, 1);
  player2_points = player_score(match, 2);
  if (player1_points > player2_points)
  {
    winner_points = player1_points;
  }else if(player2_points > player1_points){
    winner_points = player2_points;
  }

  return winner_points;
}

card * match_to_array(war_match match) {
  card *array = malloc(sizeof(card)*match_size(match));
  assert(array != NULL);
  war_match aux = match;
  for (unsigned int i = 0; i < match_size(match); i++)
  {
    array[i] = aux->elem;
    aux = aux->next;
  }
  return array;
}

void match_dump(war_match match) {
  card *array = match_to_array(match);
  for (unsigned int i=0u; i < match_size(match); i++) {
    card_dump(array[i]);
  }
  free(array);
}

war_match match_destroy(war_match match) {
  war_match aux = match;
  while(aux != NULL){
    match = match->next;
    aux->elem = card_destroy(aux->elem);
    free(aux);
    aux = match;
  }
  return match;
}
