#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"
#include "skyline.h"

#define ADD_FB    'f'
#define OVERLAY  'o'
#define OVERLAY_BUILDING   'b'
#define EMPTY    'v'
#define QUIT     'q'

void print_help(char *program_name) {
    /* Print the usage help of this program. */
    printf("Usage: %s\n\n", program_name);
}

char print_menu(void) {
    char result = '\0';
    char *line = NULL;

    printf("\nChoose what you want to do. Options are:\n"
       "\n"
       "\t************************************************\n"
       "\t* r: Add from right                            *\n"
       "\t* l: Add from left                             *\n"
       "\t* f: Add from building                         *\n"
       "\t* o: Overlay two skyline                       *\n"
       "\t* b: Overlay from building                     *\n" 
       "\t* v: Empty the skyline                         *\n" 
       "\t* q: Quit                                      *\n"
       "\t************************************************\n"
       "\nPlease enter your choice: ");

    line = readline_from_stdin();
    if (line != NULL) {
    result = line[0];
    }

    free(line);
    return(result);
}
bool is_valid_option(char option) {
    bool result = false;

    result = (option == ADD_FB ||
          option == OVERLAY ||
          option == OVERLAY_BUILDING ||
          option == EMPTY ||
          option == QUIT);
    return(result);
}

char *get_input(const char *message) {
    char *result = NULL;
    printf("\t%s: ", message);
    result = readline_from_stdin();
    assert(result != NULL);
    return(result);
}

int read_uint(const char * message, unsigned int * v) {
    int count = 0;
    char * word = NULL;
    word  = get_input(message);
    count = sscanf(word,"%u",v);
    free(word);
    return count;
}
int main(void){
    char option = '\0';
    skyline_t current = skyline_empty();
    do {
        printf("Your skyiline: \n");
        skyline_dump(current);
        option = print_menu();
        switch (option) {
        case ADD_FB:  current = skyline_from_building(9, 8, 10); break;
        case OVERLAY: current = skyline_overlay(current, current); break;
        case EMPTY: current = skyline_empty(); break;
        case OVERLAY_BUILDING: current = skyline_overlay_building(8, 9, 10, current); break;
        case QUIT: current = skyline_destroy(current); break;
        default:
            printf("\n\"%c\" is invalid. Please choose a valid "
               "option.\n\n", option);
    }
    } while (option != QUIT);
    return(EXIT_SUCCESS);
}
