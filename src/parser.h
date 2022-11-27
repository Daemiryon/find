#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

typedef struct option
{
    char *name;
    int activated;
    char parameter_value[256];
    int (*check_opt_parameter)(char *param);
    int (*opt_filter)(char* path,struct dirent *file, struct option opt);
} option;

typedef option *option_table[];

option *init_option(char *opt_name, int (*check_opt_parameter)(char *param), int *(opt_filter)(char*, struct dirent, option));

void destroy_option(option *opt);

void destroy_optable(option_table optable, int size);

int update_option(option_table opt_list, const char *opt);

int parser(option_table table, int argc, const char *argv[]);

#endif
