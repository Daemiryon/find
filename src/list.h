#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct list {
    char *value;
    struct list *next;
} list_param;

list_param *create_list();
void destroy_list(list_param *list);
int is_empty(list_param *list);
void append(list_param *list, char *param);
char *get(list_param *list, int index);
void print_list(list_param *list);

#endif
