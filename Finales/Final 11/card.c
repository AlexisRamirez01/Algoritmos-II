#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "card.h"

struct _s_card {
  unsigned int number;
  char suit;
  unsigned int player;
};

#define DIAMANTE  "DIAMANTE"
#define TREBOL    "TREBOL"
#define CORAZON   "CORAZON"
#define PICA      "PICA"

#define ROJO      "ROJO"
#define NEGRO     "NEGRO"

static const char *suit_str(char p) {
    const char *str;
    if (p == 'd') {
        str = DIAMANTE;
    } else if (p=='t') {
        str = TREBOL;
    } else if (p=='c') {
        str = CORAZON;
    } else {
        str = PICA;
    }
    return str;
}

/* Representation invariant */

static bool valid_suit(char p) {
  return p=='d' || p=='t' || p=='c' || p=='p';
}

static bool valid_player(int p) {
  return p==1 || p==2;
}

static bool invrep(card c) {
    bool valid_card=false;
    valid_card = c!=NULL &&
                  1 <= c->number && c->number <= 13 &&
                  valid_suit(c->suit) &&
                  valid_player(c->player);
    return valid_card;
}

/* Public functions */

card card_new(unsigned int num, char suit, unsigned int player) {
  assert(1 <= num && num <= 13 && valid_suit(suit) && valid_player(player));
  card new_card = malloc(sizeof(struct _s_card));
  assert(new_card != NULL);
  new_card->number = num;
  new_card->player = player;
  new_card->suit = suit;

  assert(invrep(new_card));
  return new_card;
}

unsigned int card_player(card c) {
  return c->player;
}

unsigned int card_number(card c) {
  return c->number;
}

char card_color(card c) {
  char color = 'n';
  if (card_suit(c) == 'c' || card_suit(c) == 'd') {
    color = 'r';
  }
  return color;
}

char card_suit(card c) {
  return c->suit;
}

bool card_equal_color(card fst, card snd) {
  bool resp = (card_color(fst) == card_color(snd));
  return resp;
}

unsigned int card_pair_points(card fst, card snd, unsigned int player) {
  unsigned int points = 0u;
  if (card_player(fst) == player && card_equal_color(fst, snd))
  {
    points = 1;
  }
  if (card_player(snd) == player && card_number(fst) == card_number(snd))
  {
    points = 10;
  }
  return points;
}

void card_dump(card c) {
  assert(invrep(c));
  printf("(%2d, %-7s, %2d)", c->number, suit_str(c->suit), c->player);
}

card card_destroy(card c) {
  assert(c != NULL);

  free(c);
  c = NULL;

  assert(c == NULL);
  return c;
}
