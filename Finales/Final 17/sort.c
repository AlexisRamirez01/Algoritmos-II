#include <stdlib.h>
#include <stdio.h>
#include "sorted_list.h"
#include "sort.h"

void sort(int a[], unsigned int length)
{
    sorted_list_t list = sorted_list_empty();
    for (unsigned int i = 0; i < length; i++)
    {
        list = sorted_list_append(list, a[i]);
    }
    printf("El arreglo ordenado es: \n");
    sorted_list_print(list);
    sorted_list_destroy(list);
   
}
