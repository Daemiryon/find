#include "parser.h"

option *init_option(char *opt_name, int (*check_opt_parameter)(option *))
{
    option *opt = calloc(1, sizeof(option));
    opt->name = opt_name;
    opt->activated = 0;
    opt->check_opt_parameter = *check_opt_parameter;
    return opt;
}

void destroy_option(option *opt)
{
    free(opt);
}

int update_option(option_table opt_list, const char *opt)
{
    for (unsigned int j = 0; j < 12; j++)
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

    int flag = -1;
    for (int i = 0; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            flag = update_option(table, argv[i] + 1);
            if (flag == -1)
            {
                printf("l'option %s n'existe pas \n", argv[i] + 1);
                return 0;
            }
        }

        else if (flag != -1)
        {
            if (strlen(table[flag]->parameter_value))
            {
                strcat(table[flag]->parameter_value, " ");
            }

            strcat(table[flag]->parameter_value, argv[i]);
        }
    }
    for (int i = 0; i < 12; i++)
    {
        option *opt = table[i];
        if (!opt->check_opt_parameter(opt))
        {
            return 0;
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
