#include <stdio.h>
#include <stdlib.h>
#include "utlist.h"

struct my_list {
  int data;
  struct my_list *next;
};

struct my_list *head = NULL;

int main()
{
  int i;

  for(i=0; i<100; i++)
    {
      struct my_list *tmp = malloc(sizeof(struct my_list));
      tmp->data = i;
      LL_APPEND(head, tmp);
    }

  struct my_list *tmp;
  LL_FOREACH(head, tmp) {
    printf("%d ", tmp->data);
  }
  printf("\n");

  return 0;
}


