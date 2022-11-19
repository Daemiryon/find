#include "list.h"

list_param *create_list(){
    list_param *list = (list_param*) calloc(1,sizeof(list_param));
    return list;
}

void destroy_list(list_param *list){
    if (!is_empty(list)){
        if (list->next)
        {
            destroy_list(list->next);
        }
    }
    free(list);
}

int is_empty(list_param *list){
    return list->value == NULL;
}

void append(list_param *list,char *param){
    if (is_empty(list))
    {
        list->value = param;
    }
    else
    {
        if (!list->next)
        {
            list_param *new_elem = create_list();
            new_elem->value = param;
            list->next = new_elem;
        } 
        else
        {
            append(list->next,param);
        }
    }
}

char *get(list_param *list, int index){
    if (!is_empty(list))
    {
        if (index == 0)
        {
            return list->value;
        }
        else if (!list->next)
        {
            return "-1";
        }
        else
        {
            return get(list->next,index-1);
        }
    } 
    else
    {
        return "-1";
    }   
}


void print_list_aux(list_param *list){
    printf(",%s",list->value);
    if (list->next)
    {
        print_list_aux(list->next);
    }
}
void print_list(list_param *list){
    printf("[");
    if (!is_empty(list))
    {
        printf("%s",list->value);
        if (list->next)
        {
            print_list_aux(list->next);
        }
    }
    printf("]");
}
