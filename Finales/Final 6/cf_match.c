#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "card.h"
#include "cf_match.h"

struct _s_match {
    struct s_node *head;
    unsigned int size;
};

struct s_node
{
    card elem;
    struct s_node *next;    
};
typedef struct s_node * node_t;

node_t create_node(card e) {
    node_t new_node = malloc(sizeof(struct s_node));
    assert(new_node != NULL);
    new_node->elem = e;
    new_node->next = NULL;
    return new_node;
}

node_t destroy_node(node_t node){
    assert(node != NULL);
    node->next = NULL;
    node->elem = card_destroy(node->elem);
    free(node);
    node = NULL;
    assert(node == NULL);
    return node;
}

cf_match match_new(void){
    cf_match new_match = malloc(sizeof(struct _s_match));
    assert(new_match != NULL);
    new_match->head = NULL;
    new_match->size = 0u;
    return new_match;
}

cf_match match_add(cf_match match, card c){
    node_t new_node = create_node(c);
    if (match->head == NULL)
    {
        match->head = new_node;
    }else{
        node_t aux = match->head;
        while(aux->next != NULL){
            aux = aux->next;
        }
        aux->next = new_node;
    }
    match->size++;

    return match;
}

unsigned int match_size(cf_match match)
{
    return match->size;
}

bool is_match_correct(cf_match match)
{
    bool valid = true;
    bool pair = (match->size % 2 == 0);
    bool initialplayer = (card_player(match->head->elem) == 1);
    node_t aux = match->head;
    while(aux->next != NULL){
        if (card_equal_color(aux->elem, aux->next->elem))
        {
            valid = false;
            break;
        }
        if (card_player(aux->elem) == card_player(aux->next->elem))
        {
            valid = false;
            break;
        }
        aux = aux->next;
    }
    valid = valid && pair && initialplayer;
    return valid;
}

static int player_score(cf_match match, unsigned int player)
{
    int score = 0;
    bool isFlush = false;
    bool upwardsFlush = false;
    node_t aux = match->head;
    node_t prev = NULL;
    if (player == 1)
    {
        while(aux->next != NULL){
            if (card_number(aux->elem) == card_number(aux->next->elem) + 1)
            {
                isFlush = true;
                upwardsFlush = true;
            } else if (card_number(aux->elem) == card_number(aux->next->elem) - 1)
            {
                isFlush = true;
            }
            if (card_player(aux->next->elem) == 1)
            {
                score += card_drop_points(aux->elem, aux->next->elem, isFlush, upwardsFlush);
            }
            isFlush = false;
            upwardsFlush = false;
            aux = aux->next;
        }
    }else{
        while(aux->next != NULL){
            prev = aux;
            aux = aux->next;
            if (card_number(prev->elem)+1 == card_number(aux->elem))
            {
                isFlush = true;
                upwardsFlush = true;
            } else if (card_number(prev->elem) == card_number(aux->elem) - 1)
            {
                isFlush = true;
            }
            if (card_player(aux->elem) == 2)
            {
                score += card_drop_points(prev->elem, aux->elem, isFlush, upwardsFlush);
            }
            isFlush = false;
            upwardsFlush = false;
        }
    }

    return score;
}

unsigned int match_winner(cf_match match)
{
    int player1_points = player_score(match, 1);
    int player2_points = player_score(match, 2);
    if (!is_match_correct(match))
    {
        return -1;
    }else if (player1_points > player2_points)
    {
        return 1;
    }else if (player2_points > player1_points)
    {
        return 2;
    }else{
        return 0;
    }
    
}

unsigned int winner_total_points(cf_match match)
{
    unsigned int winner_points = 0u;
    int player1_points = player_score(match, 1);
    int player2_points = player_score(match, 2);
    if (!is_match_correct(match))
       {
           return -1;
       }else if (player1_points > player2_points)
       {
           winner_points = player1_points;
       }else if (player2_points > player1_points )
       {
           winner_points = player2_points;
       }
    return winner_points;   
}

card* match_to_array(cf_match match)
{
    card *array = malloc(sizeof(card)*match_size(match));
    assert(array != NULL);
    node_t aux = match->head;
    for (unsigned int i = 0; i < match_size(match); i++)
    {
        array[i] = aux->elem;
        aux = aux->next;
    }
    return array;
    
}

void match_dump(cf_match match)
{
    card* array = match_to_array(match);
    for (unsigned int i = 0u; i < match_size(match); i++) {
        card_dump(array[i]);
    }
    free(array);
}

cf_match match_destroy(cf_match match)
{
    node_t aux = NULL;
    while(match->head != NULL){
        aux = match->head;
        match->head = match->head->next;
        aux->elem = card_destroy(aux->elem);
        aux = destroy_node(aux); 
    }
    free(match);
    match = NULL;
    assert(match == NULL);
    return match;
}
