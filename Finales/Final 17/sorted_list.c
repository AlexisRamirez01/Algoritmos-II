#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "sorted_list.h"


struct _node
{
   sorted_list_t next;
   elem_t elem;
   unsigned int size;
};

sorted_list_t sorted_list_empty()
{
    sorted_list_t new_list = NULL;
    return new_list;
}

bool sorted_list_is_empty(sorted_list_t list)
{
	bool resp = (list->size == 0);
   return resp;
}

elem_t sorted_list_head(sorted_list_t list)
{  assert(!sorted_list_is_empty(list));
   
   elem_t elem = list->elem;
	return elem;
}

sorted_list_t sorted_list_append(sorted_list_t list, elem_t elem)
{
   sorted_list_t new_node = malloc(sizeof(struct _node));
   assert(new_node != NULL);
   new_node->elem = elem;
   new_node->next = NULL;
   new_node->size = 0u;
   if (list == NULL)
   {
      list = new_node;
   }else if(list->elem > new_node->elem){
      new_node->next = list;
      list = new_node;
   }else{
      sorted_list_t aux = list;
      sorted_list_t prev = NULL;
      while(aux != NULL && aux->elem < new_node->elem){
         prev = aux;
         aux = aux->next;
      }
      new_node->next = aux;
      prev->next = new_node;
   }

   list->size++;
   return list;
}

sorted_list_t sorted_list_tail(sorted_list_t list)
{
   sorted_list_t aux = list;
   list = list->next;
   free(aux);
   list->size--;
   return list; 
}

unsigned int sorted_list_length(sorted_list_t list)
{
   return list->size;
}

elem_t sorted_list_elem_at(sorted_list_t list, unsigned int index)
{
  assert(index < sorted_list_length(list) && !sorted_list_empty(list));
  sorted_list_t aux = list;
  for (unsigned int i = 0; i < index; i++)
  {
     aux = aux->next;
  }
  elem_t elem = aux->elem;
  return elem;
}

sorted_list_t sorted_list_take(sorted_list_t list, unsigned int num)
{
   sorted_list_t new_list = malloc(sizeof (struct _node));
   assert(new_list != NULL);
   if (!sorted_list_empty(list))
   {
      return new_list;
   }
   sorted_list_t aux = list;
   for (unsigned int i = 0; i < num; i++)
   {
      new_list = sorted_list_append(new_list, aux->elem);
      aux = aux->next;
   }
   return new_list; 
}

sorted_list_t sorted_list_drop(sorted_list_t list, unsigned int num)
{
   assert(num > 0);
   sorted_list_t new_list = malloc(sizeof(struct _node));
   assert(new_list != NULL);
   if (!sorted_list_empty(list))
   {
      return new_list;
   }
   sorted_list_t aux = list;
   for (unsigned int i = 0; i < num ; ++i)
   {
      aux = aux->next;
   }
   if (aux == NULL)
   {
      return new_list;
   }
   while(aux != NULL){
      new_list = sorted_list_append(new_list, aux->elem);
      aux = aux->next;
   }
   return new_list;
}

void sorted_list_print(sorted_list_t list)
{
   sorted_list_t aux = list;
   while(aux != NULL){
      printf("El elemento es: %d\n",aux->elem );
      aux = aux->next;
   }
}

sorted_list_t sorted_list_destroy(sorted_list_t list)
{  
   sorted_list_t aux = NULL;
   while(list->next != NULL){
      aux = list;
      list = list->next;
      free(aux);
   }
   free(list);
   list = NULL;
   return list;
}
