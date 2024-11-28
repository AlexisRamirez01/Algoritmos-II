#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "tape.h"

struct _s_node {
	tape_elem elem;
	struct _s_node *next;
};

typedef struct _s_node * node_t;

struct _s_tape {
    node_t start;
    node_t cursor;
    unsigned int size;
};

static bool invrep(tape_t tape) {
	bool b = (tape != NULL);
	if (tape_length(tape) == 0)
	{
		b = tape->start == NULL && tape->cursor == NULL;
	}
    return b;
}

static node_t create_node(tape_elem e) {
	node_t new_node = malloc(sizeof(struct _s_node));
	assert(new_node != NULL);
	new_node->elem = e;
	new_node->next = NULL;
    return new_node;
}

static node_t destroy_node(node_t node) {
	assert(node != NULL);

	node->next = NULL;
	free(node);
	node = NULL;
	assert(node == NULL);
	return node;
}

tape_t tape_create(void) {
    
    tape_t new_tape = malloc(sizeof(struct _s_tape));
    assert(new_tape != NULL);
    new_tape->start = NULL;
    new_tape->cursor = NULL;
    new_tape->size = 0u;
	
	assert(invrep(new_tape));
	assert(tape_at_start(new_tape));
	assert(tape_is_empty(new_tape));
	return new_tape;
}

tape_t tape_rewind(tape_t tape) {
    assert(invrep(tape));
	
	tape->cursor = tape->start;
		
	assert(invrep(tape));
	assert(tape_at_start(tape));
	
	return tape;	
}


bool tape_at_start(tape_t tape) {
    bool resp = (tape->cursor == tape->start);
    return resp;
}

bool tape_at_stop(tape_t tape) {
	bool resp = (tape->cursor == NULL);
	return resp;
}

bool tape_is_empty(tape_t tape) {
	bool resp = (tape->size == 0u);
	return resp;
}

unsigned int tape_length(tape_t tape) {
    return tape->size;
}

tape_t tape_step(tape_t tape) {
	if (!tape_at_stop(tape))
	{
		tape->cursor = tape->cursor->next;
	}
    return tape;
}

tape_t tape_insertl(tape_t tape, tape_elem e) {
    assert(invrep(tape));
	
	node_t new_node = create_node(e);
	if (tape_is_empty(tape))
	{
		tape->start = new_node;
		tape->cursor = new_node;
	}else{
		if (tape_at_start(tape))
		{
			new_node->next = tape->cursor;
			tape->cursor = new_node;
			tape->start = new_node;
		}else{
			node_t prev = tape->start;
			while(prev->next != tape->cursor){
				prev = prev->next;
			}
			prev->next = new_node;
			new_node->next = tape->cursor;
			tape->cursor = new_node;
		}
	}

	tape->size++;
	assert(invrep(tape) && !tape_is_empty(tape) && !tape_at_stop(tape) && e == tape_read(tape));
	return tape;
}

tape_t tape_insertr(tape_t tape, tape_elem e) {
    assert(invrep(tape) && (!tape_at_stop(tape) || tape_is_empty(tape)));
    
    node_t new_node = create_node(e);
    if (tape_is_empty(tape))
    {
    	tape->cursor = new_node;
    	tape->start = new_node;
    }else{
    	new_node->next = tape->cursor->next;
    	tape->cursor->next = new_node;
    	tape->cursor = new_node;
    }

    tape->size++;
    
    assert(invrep(tape) && !tape_is_empty(tape) && !tape_at_stop(tape) && e == tape_read(tape));
    return tape;
}

tape_t tape_erase(tape_t tape) {
    assert(invrep(tape) && !tape_is_empty(tape) && !tape_at_stop(tape));
	
	node_t prev = NULL;
	if (tape_at_start(tape))
	{
		prev = tape->cursor;
		tape->cursor = tape->cursor->next;
		tape->start = tape->start->next;
		destroy_node(prev);
	}else{
		prev = tape->start;
		while(prev->next != tape->cursor){
			prev = prev->next;
		}
		prev->next = tape->cursor->next;
		destroy_node(tape->cursor);
		tape->cursor = prev->next;
	}
	
	tape->size--;
	assert(invrep(tape));
	return tape;
}

tape_elem tape_read(tape_t tape) {
	assert(invrep(tape) && !tape_is_empty(tape) && !tape_at_stop(tape));
	
	tape_elem elem = tape->cursor->elem;
	
	return elem;
}

void tape_dump(tape_t tape) {
    assert(invrep(tape));
    node_t node=tape->start;
    printf("#");
    while (node != NULL) {
        if (node != tape->cursor) {
            printf("-%c-", node->elem);
        } else {
            printf("-[%c]-", node->elem);
        }
        node = node->next;
    }
    if (tape->cursor==NULL) {
        printf("-[]-");
    }
    printf("#\n");
}

tape_t tape_copy(tape_t tape) {
    assert(invrep(tape));
	
	node_t previous_state = tape->cursor;
	tape = tape_rewind(tape);
	tape_t copy = tape_create();

	
	while(!tape_at_stop(tape)){
		tape_insertr(copy, tape_read(tape));
		tape = tape_step(tape);
	}

	copy = tape_rewind(copy);
	tape->cursor = previous_state;
	copy->size = tape_length(tape);
	 
	
	assert(invrep(copy) && copy != tape && tape_at_start(copy) && tape_length(copy) == tape_length(tape));
	return copy;
	
}

tape_t tape_destroy(tape_t tape) {
    assert(invrep(tape));
	
	node_t aux = NULL;
	tape = tape_rewind(tape);
	while(!tape_at_stop(tape)){
		aux = tape->cursor;
		tape = tape_step(tape);
		destroy_node(aux);
	}
	free(tape);
	tape = NULL;
	assert(tape == NULL);
	return tape;
}


