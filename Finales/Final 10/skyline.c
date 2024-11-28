#include <stdio.h>
#include <stdlib.h>
#include "skyline.h"
#include <assert.h>

struct _node_t {
    height_t height;
    struct _node_t *next;
};

typedef struct _node_t *node_t;

skyline_t skyline_empty(void) {
    skyline_t result = NULL;
    return(result);
}

skyline_t skyline_destroy(skyline_t skyline) {
    skyline_t aux = NULL;
    while (skyline != NULL) {
        aux = skyline->next;
        free(skyline);
        skyline = aux;
    }
    return (skyline);
}

static node_t new_node(height_t height, node_t next) {
   node_t node = malloc(sizeof(struct _node_t));
   assert(node != NULL);
   node->next = next;
   node->height = height;
   return node;
}

static skyline_t skyline_add_left(skyline_t skyline,  height_t height) {
    node_t skyline_act = new_node(height, skyline);
    return skyline_act;
}

static node_t last_node(skyline_t skyline) {
    assert(skyline != NULL);
    node_t last_n = NULL;
    node_t last = skyline;
    while(last->next != NULL){
        last = last->next;
    }
    last_n = last;
    return last_n;

}

static skyline_t skyline_add_right(skyline_t skyline,  height_t height) {
    node_t node = new_node(height, NULL);
    if (skyline == NULL)
    {
        skyline = node;
    }else{
        node_t last = last_node(skyline);
        last->next = node;
    }
    return skyline;
}

skyline_t skyline_add_building_left(skyline_t skyline, 
width_t width, height_t height){
    for (unsigned int i = 0; i < width; i++)
    {
        skyline = skyline_add_left(skyline, height);
    }
    return skyline;
}

skyline_t skyline_add_building_right(skyline_t skyline,
 width_t width, height_t height){
    for (unsigned int i = 0; i < width; i++)
    {
        skyline = skyline_add_right(skyline, height);
    }
    return skyline;
}

static height_t max_height (height_t h, height_t j) {
    height_t max = h;
    if (h < j)
    {
        max = j;
    }
    return max;
}

height_t skyline_max_height(skyline_t skyline) {

    height_t max_heights = 0;
    node_t aux = skyline;
    while(aux != NULL){
        max_heights = max_height(max_heights, aux->height);
        aux = aux->next;
    }
    return max_heights;
    
}


skyline_t skyline_overlay(skyline_t skyline_one , skyline_t skyline_two) {
    skyline_t s1 = skyline_one;
    skyline_t s2 = skyline_two;
    skyline_t new_skyline = NULL;
    while(s1 != NULL || s2 != NULL){
        if (s1 != NULL && s2 != NULL)
        {
            height_t max = max_height(s1->height, s2->height);
            new_skyline = skyline_add_right(new_skyline, max);
            s1 = s1->next;
            s2 = s2->next;
        }else if (s1 != NULL)
        {
            new_skyline = skyline_add_right(new_skyline, s1->height);
            s1 = s1->next;
        }else if (s2 != NULL)
        {
            new_skyline = skyline_add_right(new_skyline, s2->height);
            s2 = s2->next;
        }
    }
    return new_skyline;
}

skyline_t skyline_from_building(unsigned int start, width_t width, height_t height) {
    skyline_t sky = skyline_empty();
    for (unsigned int i = 0; i < start; i++)
    {
        sky = skyline_add_building_left(sky, 1, 0);
    }
    sky = skyline_add_building_right(sky, width, height);
    return sky;
}

skyline_t skyline_overlay_building(unsigned int start, width_t width, height_t height, skyline_t skyline) {
    skyline_t single_sky = skyline_from_building(start, width, height);
    skyline_t result_overlay = skyline_overlay(skyline, single_sky);
    return result_overlay;
}

void skyline_dump(skyline_t skyline) {
    node_t current = NULL;
    current = skyline;
    printf("\n[");
    while (current != NULL) {
        printf("%d ", current->height); 
        current = current->next;
    }
    printf("]\n"); 
}

