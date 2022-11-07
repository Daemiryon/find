#include "checke_param.h"

int check_no_param(option *opt)
{
    if (strlen(opt->parameter_value))
    {
        printf("parametre %s invalide : l'option %s ne prend pas de parametre\n", opt->parameter_value, opt->name);
        return 0;
    }
    return 1;
}

int check_name_param(option *opt)
{
    return 1;
}

// int check_size_param(option *opt)
// {
//     return 1;
// }
// int check_date_param(option *opt);
// int check_mime_param(option *opt);
// int check_ctc_param(option *opt);
// int check_dir_param(option *opt);
// int check_color_param(option *opt);
// int check_perm_param(option *opt);
// int check_link_param(option *opt);
// int check_threads_param(option *opt);
// int check_ou_param(option *opt);
// int check_test_param(option *opt);
