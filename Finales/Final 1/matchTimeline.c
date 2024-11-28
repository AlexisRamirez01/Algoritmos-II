#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "matchTimeline.h"

/* ============================================================================
STRUCTS
============================================================================ */

struct s_node
{
    team team;
    event e;
    time t;
    playerNumber pn;
    struct s_node *next;
};

struct s_matchTimeline
{
    struct s_node *head;
    unsigned int register_size;
    unsigned int goals_of_home;
    unsigned int goals_of_away;
};

/* ============================================================================
INVERP & VALIDATION
============================================================================ */

static bool invrep(matchTimeline mt)
{
    bool resp = true;
    resp = (mt != NULL);
    unsigned int length = 0u;
    struct s_node *aux = mt->head;
    while (aux != NULL)
    {
        length++;
        aux = aux->next;
    }
    resp = resp && (length == mt->register_size);
    return resp; 
}

bool matchTimeline_is_time_and_score_valid(matchTimeline mt)
{
    bool resp = true;
    unsigned int register_of_home = 0u;
    unsigned int register_of_away = 0u;
    unsigned int time = 0u;
    struct s_node *aux = mt->head;
    while (aux != NULL)
    {
        if (aux->e == Goal)
        {
            if (aux->team == Home)
            {
                register_of_home += 1;
            }
            if(aux->team == Away){
                register_of_away += 1;
            }
        }
        if(aux->t < time){
            resp = false;
        }
        
        time = aux->t;
        aux = aux->next;
    }
    resp = resp && (mt->goals_of_away == register_of_away) && 
                    (mt->goals_of_home == register_of_home);
    
    return resp;
}

/* ============================================================================
CONSTRUCTOR
============================================================================ */

matchTimeline matchTimeline_empty(void)
{
    matchTimeline mt = NULL;
    mt = malloc(sizeof(struct s_matchTimeline));
    assert(mt != NULL);
    mt->head = NULL;
    mt->register_size = 0u;
    mt->goals_of_away = 0u;
    mt->goals_of_home = 0u;

    assert(invrep(mt));
    return mt;
}

/* ============================================================================
INSERTION
============================================================================ */

static struct s_node *create_node(team team, event e, time t, playerNumber pn)
{

    struct s_node *new_node = NULL;
    new_node = malloc(sizeof(struct s_node));
    assert(new_node != NULL);
    new_node->e = e;
    new_node->team = team;
    new_node->t = t;
    new_node->pn = pn;
    
    return new_node;
}

void insert_node(matchTimeline mt, team team, event e, time t, playerNumber pn){
    struct s_node *node = create_node(team, e, t, pn);
    if(mt->head == NULL){
        mt->head = node;
    } else {
        struct s_node *aux = mt->head;
        while (aux->next != NULL)
        {
            aux = aux->next;
        }
        aux->next = node;
        
    }
    mt->register_size++;
}

/* ============================================================================
GOAL
============================================================================ */

matchTimeline matchTimeline_score_goal(matchTimeline mt, team team, time t, playerNumber pn)
{
    assert(invrep(mt));

    if(t <= EVENT_MAX_TIME){
        struct s_node *aux = mt->head;
        while (aux != NULL)
        {
            //Chequeo caso donde el jugador que anota el gol esta expulsado.
            if (aux->e == RedCard && aux->pn == pn)
            {
                return mt;
            }
            aux = aux->next;
        }
        insert_node(mt, team, Goal, t, pn);
        //Chequeo quien hizo el gol.
        if(team == Home){
            mt->goals_of_home++;
        }
        if(team == Away){
            mt->goals_of_away++;
        }
        
    }
    
    return mt;
}

/* ============================================================================
THE CARDS
============================================================================ */

matchTimeline matchTimeline_receive_yellowCard(matchTimeline mt, team team, time t, playerNumber pn)
{
    assert(invrep(mt));

    if (t <= EVENT_MAX_TIME)
    {
        struct s_node *aux = mt->head;
        bool have_YellowCard = false;
        while(aux != NULL){
            if (aux->e == YellowCard && aux->pn == pn )
            {
                have_YellowCard = true;
            }
            if(aux->e == RedCard && aux->pn == pn){
                return mt;
            }
            aux = aux->next;
        }
        if (have_YellowCard)
        {
            insert_node(mt, team, RedCard, t, pn);
        }
        
        insert_node(mt,team, YellowCard, t, pn);
    }
    
    
    return mt;
}

matchTimeline matchTimeline_receive_redCard(matchTimeline mt, team team, time t, playerNumber pn)
{
    assert(invrep(mt));

    if (t <= EVENT_MAX_TIME)
    {
        struct s_node *aux = mt->head;
        while (aux != NULL)
        {
            if (aux->e == RedCard && aux->pn == pn)
            {
                return mt;
            }
            aux = aux->next;
            
        }
        insert_node(mt, team, RedCard, t, pn);
    }
    

    return mt;
}

/* ============================================================================
SCORE
============================================================================ */

unsigned int matchTimeline_get_score(matchTimeline mt, team team)
{
    assert(invrep(mt));

    unsigned int score = 0u;
    if(team == Home){
        score = mt->goals_of_home;
    }else{
        score = mt->goals_of_away;
    }
    
    return score;
}

/* ============================================================================
SIZE
============================================================================ */

unsigned int matchTimeline_size(matchTimeline mt)
{
    assert(invrep(mt));
    return mt->register_size;
}

/* ============================================================================
PRINT
============================================================================ */

void matchTimeline_print(matchTimeline mt)
{
    fprintf(
        stdout, "            Home %u -  %u Away \n", 
        matchTimeline_get_score(mt, Home), 
        matchTimeline_get_score(mt, Away)
    );

    struct s_node *aux = mt->head;
    while (aux != NULL)
    {
        unsigned int t = aux->t;
        char *e;
        switch (aux->e)
        {
        case Goal:
            e = "    Goal   ";
            break;
        case YellowCard:
            e = "Yellow Card";
            break;
        case RedCard:
            e = "  Red Card ";
            break;
        }
        unsigned int pn = aux->pn;

        if (aux->team == Home) {
            fprintf(stdout, "%u - %s - %u' \n", pn, e, t);
        }
        else {
            fprintf(stdout, "                   %u' - %s - %u \n", t, e, pn);
        }

        aux = aux->next;
    }
}

/* ============================================================================
ARRAY
============================================================================ */

event *matchTimeline_events_array(matchTimeline mt)
{
    event *array = NULL;
    
    array = malloc(sizeof(struct s_matchTimeline)*mt->register_size);
    assert(array != NULL);
    unsigned int capacity = mt->register_size;
    struct s_node *aux = mt->head;
    for (unsigned int i = 0u; i < capacity; i++)
    {
        array[i] = aux->e;
        aux = aux->next;
    }
    return array;
}

/* ============================================================================
DESTRUCTOR!
============================================================================ */

static struct s_node *destroy_node(struct s_node *node)
{
    if(node != NULL){
        node->next = NULL;
        free(node);
        node = NULL;
    }
    assert(node == NULL);
    return node;
}

matchTimeline matchTimeline_destroy(matchTimeline mt)
{
    assert(invrep(mt));

    while (mt->head != NULL)
    {
        struct s_node *aux = mt->head;
        mt->head = mt->head->next;
        aux = destroy_node(aux);
    }
    free(mt);
    assert(mt != NULL);
    mt = NULL;
    

    return mt;
}
