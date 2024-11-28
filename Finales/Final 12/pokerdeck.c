#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <assert.h>

#include "pokerdeck.h"

//Estructura auxiliar para implementar count en O(1)
struct s_count{
    unsigned int count_spades;
    unsigned int count_hearts;
    unsigned int count_diamonds;
    unsigned int count_clubs;
};

struct s_pokerdck {
    struct s_node *first;
    unsigned int length;
    struct s_count count;
};

struct s_node {
    cardnum_t num;
    cardsuit_t suit;
    struct s_node *nextcard;
};

typedef struct s_node * node_t;

// Auxiliar dump functions
static void dump_suit(cardsuit_t suit) {
    printf("%s", suit==spades   ? "♠️  ":
                 suit==hearts   ? "♥️  ":
                 suit==diamonds ? "♦️  ":
                 suit==clubs    ? "♣️  ": "???");
}

static void dump_num(cardnum_t num) {
    if (1 < num && num < 11) {
        printf("%-2d", num);
    } else {
        printf("%-2s", num==1 ? "A":
                       num==11 ? "J":
                       num==12 ? "Q":
                       num==13 ? "K": "???");

    }
}

// Representation invariant
static bool invrep(pokerdeck deck) {
    bool valid=false;
    valid = (deck != NULL);
    return valid;
}

// Creates a single node in dynamic memory with a given card
static node_t create_node(cardnum_t num, cardsuit_t suit) {
    node_t newnode=NULL;
    newnode = malloc(sizeof(struct s_node));    
    assert(newnode!=NULL);
    newnode->num = num;
    newnode->suit = suit;
    newnode->nextcard = NULL;
    return newnode;
}

// Destroy a single node
static node_t destroy_node(node_t node) {
    assert(node != NULL);
    node->nextcard = NULL;
    free(node);
    node = NULL;
    assert(node==NULL);
    return node;
}

// Public functions of the Abstract Data Type
pokerdeck pokerdeck_empty(void) {
    pokerdeck new_deck = malloc(sizeof(struct s_pokerdck));
    assert(new_deck != NULL);
    new_deck->first = NULL;
    new_deck->count.count_spades = 0u;
    new_deck->count.count_hearts = 0u;
    new_deck->count.count_diamonds = 0u;
    new_deck->count.count_clubs = 0u;
    new_deck->length = 0u;
    assert(invrep(new_deck));
    return new_deck;
}

bool pokerdeck_is_empty(pokerdeck deck) {
    bool resp = (deck->length == 0);
    return resp;
}

pokerdeck pokerdeck_add(pokerdeck deck, cardnum_t num, cardsuit_t suit) {
    
    node_t aux = NULL;
    node_t newnode = create_node(num, suit);
    if (deck->first == NULL)
    {
        deck->first = newnode;
    }else{
        aux = deck->first;
        while(aux->nextcard != NULL){
            aux = aux->nextcard;
        }
        aux->nextcard = newnode;
    }

    if (newnode->suit == spades)
    {
        deck->count.count_spades++;
    }else if (newnode->suit == hearts)
    {
        deck->count.count_hearts++;
    }else if (newnode->suit == clubs)
    {
        deck->count.count_clubs++;
    }else
    {
        deck->count.count_diamonds++;
    }
    deck->length++;
    return deck;
}

pokerdeck pokerdeck_push(pokerdeck deck,  cardnum_t num, cardsuit_t suit) {
    
    node_t newnode = create_node(num, suit);
    newnode->nextcard = deck->first;
    deck->first = newnode;
    if (newnode->suit == spades)
    {
        deck->count.count_spades++;
    }else if (newnode->suit == hearts)
    {
        deck->count.count_hearts++;
    }else if (newnode->suit == clubs)
    {
        deck->count.count_clubs++;
    }else
    {
        deck->count.count_diamonds++;
    }
    deck->length++;
    return deck;
}

pokerdeck pokerdeck_pop(pokerdeck deck, cardnum_t *popped_num, cardsuit_t *popped_suit) {
    if (deck->length != 0u && deck->first != NULL)
    {
        node_t uax = NULL;
        if(popped_num != NULL){ 
            *popped_num = deck->first->num;
        }
        if (popped_suit != NULL){ 
            *popped_suit = deck->first->suit;
        }
        uax = deck->first;
        deck->first = deck->first->nextcard;
        uax = destroy_node(uax);
        deck->length--;
    }
    return deck;
}

unsigned int pokerdeck_length(pokerdeck deck) {
    return deck->length;
}

pokerdeck pokerdeck_remove(pokerdeck deck, cardnum_t num, cardsuit_t suit) {
    node_t aux = deck->first;
    node_t remove_node = NULL;
    while (aux != NULL){
        if (aux->num == num && aux->suit == suit)
        {
            remove_node = aux;
            break;
        }
    }
    destroy_node(remove_node);
    return deck;
}

unsigned int pokerdeck_count(pokerdeck deck, cardsuit_t suit) {
    if (suit == spades)
    {
        return deck->count.count_spades;
    }else if (suit == hearts)
    {
        return deck->count.count_hearts;
    }else if (suit == clubs)
    {
        return deck->count.count_clubs;
    }else{
        return deck->count.count_diamonds;
    }
}

struct card * pokerdeck_to_array(pokerdeck deck) {
    struct card *array = malloc(sizeof(struct card)*deck->length);
    assert(array != NULL);
    node_t aux = deck->first;
    for (unsigned int i = 0; i < deck->length; i++)
    {
        array[i].num = aux->num;
        array[i].suit = aux->suit;
        aux = aux->nextcard;
    }
    return array;
}

void card_dump(cardnum_t num, cardsuit_t suit) {
    printf("|");
    dump_num(num);
    printf(" ");
    dump_suit(suit);
    printf("|");
}

void pokerdeck_dump(pokerdeck deck) {
    assert(invrep(deck));
    node_t node = deck->first;
    while (node != NULL) {
        card_dump(node->num, node->suit);
        printf(" ");
        node = node->nextcard;
    }
    printf("\n");
}

pokerdeck pokerdeck_destroy(pokerdeck deck) {
    node_t aux = NULL;
    while(deck->first != NULL){
        aux = deck->first;
        deck->first = deck->first->nextcard;
        aux = destroy_node(aux);
    }
    free(deck);
    deck = NULL;
    assert(deck == NULL);
    return deck;
}

