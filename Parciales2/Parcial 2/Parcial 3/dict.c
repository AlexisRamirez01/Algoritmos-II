#include <assert.h>
#include <stdlib.h>
#include "dict.h"
#include "string.h"

struct _dict_t {
	node_t words;
	node_t definitions;
	unsigned int dict_size;
};

struct _node_t {
   string word;
   struct _node_t *next;
};

static bool invrep(dict_t d) {
	bool b = (d != NULL);
	return b;
}

/*// returns the amount of nodes to skip when adding a new word
static int nodes_to_skip(dict_t dict, string word) {
  int count = 0;
  struct _node_t *aux = dict->words;
  while(aux != NULL){
  	if (string_eq(aux->word, word))
  	{
  		return count;
  	}
  	count++;
  	aux = aux->next;
  }

  return count;
}
*/
// returns the position of the word on the list of nodes (0 being the first node)
// returns -1 if the word is not on the dict
static int find_index_of_key(dict_t dict, string word) {
	int index = 0;
	node_t aux = dict->words;
	if (!dict_exists(dict, word))
	{
		return -1;
	}else{
		while(aux != NULL){
			if (string_eq(aux->word, word))
			{
				return index;
			}
			index++;
			aux = aux->next;
		}
	}
  return index;
}

dict_t dict_empty(void) {

    dict_t new_dict = malloc(sizeof(struct _dict_t));
    assert(new_dict != NULL);
    new_dict->words = NULL;
    new_dict->definitions = NULL;
    new_dict->dict_size = 0u;
    return new_dict;

}

static node_t create_node(string elem) {
	node_t new_node = malloc(sizeof(struct _node_t));
	assert(new_node != NULL);
	new_node->word = elem;
	new_node->next = NULL;
	return new_node;
}

static node_t destroy_node(node_t node) {
	assert(node != NULL);
    node->next = NULL;
    node->word = string_destroy(node->word);
    free(node);
    node = NULL;
    assert(node == NULL);
    return node;
}

dict_t dict_add(dict_t dict, string word, string def) {

    int index = find_index_of_key(dict, word);
    if (index != -1) // Caso donde la word ya existe y debo cambiar la def
    {
    	node_t aux = dict->words;
    	node_t aux2 = dict->definitions;
    	for (int i = 0; i < index; ++i)
    	{
    		aux = aux->next;
    		aux2 = aux2->next;
    	}
    	aux2->word = string_destroy(aux2->word);
    	aux2->word = def;
    }else{ //Caso donde la word no existe: el diccionario no tiene nada o la palabra está antes lexicograficamente de las demás
    	node_t new_word = create_node(word);
    	node_t new_def = create_node(def);
    	if ((dict->words == NULL && dict->definitions == NULL) || string_less(word, dict->words->word))
    	{
    		new_word->next = dict->words;
    		dict->words = new_word;
    		new_def->next = dict->definitions;
    		dict->definitions = new_def;
    	}else{//Caso donde ya hay más elementos en el dict 
    		node_t aux = dict->words;
    		node_t tmp = dict->definitions;
    		while(tmp->next != NULL && aux->next != NULL && string_less(word, aux->next->word)){
    			tmp = tmp->next;
    			aux = aux->next;
    		}
    		new_word->next = aux->next;
    		aux->next = new_word;
    		new_def->next = tmp->next;
    		tmp->next = new_def;
    	}
    	dict->dict_size++;

    }
return dict;
}

string dict_search(dict_t dict, string word) {

	string value = NULL;
    node_t aux = dict->words;
    node_t tmp = dict->definitions;
    while(aux != NULL && tmp != NULL){
    	if (string_eq(aux->word, word))
    	{
    		value = tmp->word;
    		break;
    	}
    	aux = aux->next;
    	tmp = tmp->next;
    }
    return value;
}

bool dict_exists(dict_t dict, string word) {

    bool exists = false;
    node_t aux = dict->words;
    while(aux != NULL){
    	if (string_eq(aux->word, word))
    	{
    		exists = true;
    		break;
    	}
    	aux = aux->next;
    }
    return exists;
}

unsigned int dict_length(dict_t dict) {

    return dict->dict_size;

}

// removes the "index" element of the "list" list of nodes
static node_t remove_on_index(node_t list, int index) {

	if (index == 0)
	{
		node_t aux = list;
		list = list->next;
		destroy_node(aux);
	}else{
		node_t aux = list;
		for (int i = 0; i < index - 1; i++)
		{
			aux = aux->next;
		}
		node_t prev = aux;
		aux = aux->next;
		destroy_node(prev);
	}
	return list;
}

dict_t dict_remove(dict_t dict, string word) {
  assert(invrep(dict));
  int index_of_word = find_index_of_key(dict, word);
  if (index_of_word != -1)
  {
  	dict->words = remove_on_index(dict->words, index_of_word);
  	dict->definitions = remove_on_index(dict->definitions, index_of_word);
  	dict->dict_size--;
  }
  assert(invrep(dict));
  return dict;
}

dict_t dict_remove_all(dict_t dict) {
	while(dict->words != NULL && dict->definitions != NULL){
		dict->words = remove_on_index(dict->words, 0);
		dict->definitions = remove_on_index(dict->definitions, 0);
		dict->dict_size--;
	}
	dict->dict_size = 0u;
	return dict;
}

void dict_dump(dict_t dict, FILE *file) {

    node_t aux = dict->words;
    node_t tmp = dict->definitions;
    while(aux != NULL && tmp != NULL){
    	fprintf(file, "La palabra es %s y su definición es:%s\n", string_ref(aux->word), string_ref(tmp->word));
    	aux = aux->next;
    	tmp = tmp->next;
    }

}

dict_t dict_destroy(dict_t dict) {
  dict = dict_remove_all(dict);
  free(dict);
  dict = NULL;
  return dict;
}
