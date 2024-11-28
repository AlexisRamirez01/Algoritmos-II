#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "card.h"

struct _s_card {
    unsigned int number;
    char suit;
    unsigned int player;
};

#define DIAMANTE "DIAMANTE"
#define TREBOL "TREBOL"
#define CORAZON "CORAZON"
#define PICA "PICA"

static const char* suit_str(char p)
{
    const char* str;
    if (p == 'd') {
        str = DIAMANTE;
    } else if (p == 't') {
        str = TREBOL;
    } else if (p == 'c') {
        str = CORAZON;
    } else {
        str = PICA;
    }
    return str;
}

/* Representation invariant */

static bool valid_suit(char p)
{
    return p == 'd' || p == 't' || p == 'c' || p == 'p' || p == 'x';
}

static bool valid_player(int p)
{
    return p == 1 || p == 2;
}

static bool invrep(card c)
{
    bool valid_card = false;
    valid_card = c != NULL && 1 <= c->number && c->number <= 13 && valid_suit(c->suit) && valid_player(c->player);
    return valid_card;
}

/* Public functions */

card card_new(unsigned int num, char suit, unsigned int player)
{
    card new_card = malloc(sizeof(struct _s_card));
    assert(new_card != NULL);
    new_card->number = num;
    new_card->suit = suit;
    new_card->player = player;
    
    assert(invrep(new_card));
    return new_card;
}

unsigned int card_player(card c)
{
    return c->player;
}

unsigned int card_number(card c)
{
    return c->number;
}

char card_color(card c)
{
    if (c->suit == 'd' || c->suit == 'c') {
        return 'R';
    }
    return 'N';
}

char card_suit(card c)
{
    return c->suit;
}

bool card_equal_color(card fst, card snd)
{
    return card_color(fst) == card_color(snd);
}

bool card_equal_number(card fst, card snd)
{
    return card_number(fst) == card_number(snd);
}

int card_drop_points(card fst, card snd, bool isFlush, bool upwardsFlush) {
    int points = 0;

    // Si las cartas tienen el mismo número
    if (card_equal_number(fst, snd)) {
        points = 7;
    }
    if (isFlush)
    {
        points = 3;
    }if (isFlush && (upwardsFlush || card_number(fst) == card_number(snd) - 1))
    {
        points = 5;
    }if (!isFlush && ((card_number(fst) != card_number(snd) + 1) || (card_number(fst) != card_number(snd) - 1)))
    {
        if (card_number(fst) > card_number(snd))
        {
            points = 0;
        }else{
            points = -3;
        }
    }
    //Contigua == carta que estoy mirando == carta que sigue + 1 o carta que sigue - 1

    return points;
}


void card_dump(card c)
{
    assert(invrep(c));
    printf("(%2d, %-7s, %2d)", c->number, suit_str(c->suit), c->player);
}

card card_destroy(card c)
{
    free(c);
    c = NULL;
    assert(c == NULL);
    return c;
}
