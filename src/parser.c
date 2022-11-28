#include "parser.h"

option *init_option(char *opt_name, int (*check_opt_parameter)(char *), int (*opt_filter)(char*, struct dirent*, option*))
{
    option *opt = calloc(1, sizeof(option));
    opt->name = opt_name;
    opt->activated = 0;
    opt->check_opt_parameter = *check_opt_parameter;
    opt->opt_filter = *opt_filter;
    return opt;
}

void destroy_option(option *opt)
{
    free(opt);
}

int update_option(option_table opt_list, const char *opt)
{
    for (unsigned int j = 0; j < 13; j++)
    {
        if (strcmp(opt_list[j]->name, opt) == 0)
        {
            opt_list[j]->activated = 1;
            return j;
        }
    }
    return -1;
}

int parser(option_table table, int argc, const char *argv[])
{
    int flag = 12;
    for (int i = 0; i < argc; i++)
    {
        // Cas où on a un paramètre d'option a mettre (si cette option n'a pas déjà de paramètre)
        if (( table[flag]->check_opt_parameter(argv[i]) ) & ( !strlen(table[flag]->parameter_value) ))
        {
            strcpy(table[flag]->parameter_value, argv[i]);
        }

        // Cas où on rencontre une option
        else if (argv[i][0] == '-')
        {
            flag = update_option(table, argv[i] + 1);
            if (flag == -1)
            {
                printf("Le flag -%s n'est pas correct\n", argv[i] + 1);
                return 0;
            }
        }

        // Cas où test est activé ==> Ce qui suit une option (si argv[i][0] != '-') devient le paramètre de cette option
        else if (( table[11]->activated ) & ( !strlen(table[flag]->parameter_value) ))
        {
            strcpy(table[flag]->parameter_value, argv[i]);
        }
    }
    return 1;
}

void destroy_optable(option_table optable, int size)
{
    for (int i = 0; i < size; i++)
    {
        destroy_option(optable[i]);
    }
}
