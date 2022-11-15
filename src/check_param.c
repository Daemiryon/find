#include "check_param.h"
#include <regex.h>

int check_no_param(option *opt)
{
    if (strlen(opt->parameter_value))
    {
        printf("Invalid argument %s : %s option does not take any argument.\n", opt->parameter_value, opt->name);
        return 0;
    }
    return 1;
}

int check_name_param(option *opt)
{
    if (opt->activated)
    {
        if (!strcmp(opt->parameter_value,""))
        {
            printf("Missing argmuent : -name option takes \"file_name.file_extension\" argument.\n");
            return 0;
        }
        int i = strlen(opt->parameter_value)-1;
        while (i>0)
        {
            if (opt->parameter_value[i] == '.')
            {
                return 1;
            }
            i--;
        }
        printf("Invalid argmuent %s : -name option takes \"file_name.file_extension\" argument.\n",opt->parameter_value);
        return 0;
    }
    return 1;
    
    
}

// int check_size_param(option *opt)
// {
//     if (opt->activated)
//     {
//         regex_t size_regex;
//         int test;
        
//         test = regcomp(&size_regex,"(\+|-)?:digit:+[ckMG]",0);
//         if (test){
//             printf("Regex error");
//             return 0;
//         }
//         test = regexec(&size_regex,opt->parameter_value,0,NULL,0);
//         if (test == REG_NOMATCH)
//         {
//             printf("Invalid argument %s for -size option.",opt->parameter_value);
//             return 0;
//         }
//         printf("test");
//     }
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
