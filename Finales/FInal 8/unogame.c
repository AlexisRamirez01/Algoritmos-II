#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

#include "unogame.h"
#include "card.h"

#define NO_PLAYER -1

struct s_ugame {
    struct c_node *top;
    unsigned int played_cards;
};

struct c_node{
    card_t elem;
    player_t player;
    struct c_node *next;
};

//---------Funciones auxiliares para armar la pila------------
typedef struct c_node * node_t;
struct c_node *create_node(card_t card, player_t player){
    node_t new_node = malloc(sizeof(struct c_node));
    assert(new_node != NULL);
    new_node->elem = card;
    new_node->next = NULL;
    new_node->player = player;

    return new_node;
}

struct c_node *destroy_node(node_t node){
    assert(node != NULL);
    node->next = NULL;
    node->elem = card_destroy(node->elem);
    free(node);
    node = NULL;
    assert(node == NULL);

    return node;
}

//------------------------------------------------------------

unogame_t
uno_newgame(card_t card) {
    unogame_t new_game = malloc(sizeof(struct s_ugame));
    assert(new_game != NULL);
    new_game->top = create_node(card, NO_PLAYER);
    new_game->played_cards = 1u;

    return new_game;
}

card_t
uno_topcard(unogame_t uno) {
    card_t top_card = uno->top->elem;
    return top_card;
}

unogame_t
uno_addcard(unogame_t uno, card_t c) {
    
    player_t current_player = uno_nextplayer(uno);
    node_t new_node = create_node(c, current_player);
    new_node->next = uno->top;
    uno->top = new_node;
    uno->played_cards++;

    return uno;
}

unsigned int
uno_count(unogame_t uno) {
    return uno->played_cards;
}

bool
uno_validpile(unogame_t uno) {
    node_t aux = uno->top;
    bool valid = true;
    while(aux->next != NULL && valid){
        valid = card_compatible(aux->elem, aux->next->elem);
        aux = aux->next;
    }
    if (card_get_type(aux->elem) == change_color)
    {
        valid = false;
    }
    valid = valid && (uno->top != NULL);
    return valid;

}

color_t
uno_currentcolor(unogame_t uno) {
    return card_get_color(uno_topcard(uno));
}

player_t
uno_nextplayer(unogame_t uno) {
    

    player_t player = -1;
    if (card_get_type(uno->top->elem) != skip)
    {
        if (uno->top->player == -1 || uno->top->player == 2){
            player = 0;
        }else if (uno->top->player == 0){
            player = 1;
        }else if (uno->top->player == 1){
            player = 2;
        }
    }else{ 
        if ((uno->top->player == -1) || (uno->top->player == 2)){
                player = 1;
        }else if (uno->top->player == 0)
        {
            player = 2;
        }else if (uno->top->player == 1)
        {
            player = 0;
        }    
    }
    return player;
}

card_t *
uno_pile_to_array(unogame_t uno) {

    node_t aux = uno->top;
    card_t *array = malloc(sizeof(card_t*)*uno_count(uno));
    assert(array != NULL);
    unsigned int size = uno_count(uno) - 1;
    for (unsigned int i = 0; i < uno_count(uno); i++)
    {
        array[size - i] = aux->elem;
        aux = aux->next;
    }

    return array;
}

unogame_t
uno_destroy(unogame_t uno) {
    
    node_t aux = NULL;
    while(uno->top != NULL){
        aux = uno->top;
        uno->top = uno->top->next;
        destroy_node(aux);
    }
    free(uno);
    uno = NULL;
    assert(uno == NULL);
    return uno;
}

