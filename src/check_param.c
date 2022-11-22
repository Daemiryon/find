#include "check_param.h"

int check_no_param(char *param)
{
    if (strlen(param))
    {
        printf("Invalid argument %s : option does not take any argument.\n", param);
        return 0;
    }
    return 1;
    //return !strlen(param);
}

int check_name_param(char *param)
{
    return (strlen(param)>0);
}

int check_size_param(char *param)
{
    regex_t size_regex;
    int test = regcomp(&size_regex,"[+-]\\?[0-9]\\+[ckMG]",0);
    if (test){
        printf("Regex error\n");
        regfree(&size_regex);
        return 0;
    }
    test = regexec(&size_regex,param,0,NULL,0);
    regfree(&size_regex);
    if (test == REG_NOMATCH)
    {
        //printf("Invalid argument %s for -size option.\n",param);
        return 0;
    }
    return 1;
}

int check_date_param(char *param)
{
    regex_t date_regex;
    int test = regcomp(&date_regex,"[+]\\?[0-9]\\+[jhm]",0);
    if (test){
        printf("Regex error\n");
        regfree(&date_regex);
        return 0;
    }
    test = regexec(&date_regex,param,0,NULL,0);
    regfree(&date_regex);
    if (test == REG_NOMATCH)
    {
        //printf("Invalid argument %s for -date option.\n",param);
        return 0;
    }
    return 1;
}

// int check_mime_param(char *param);

int check_ctc_param(char *param)
{
    return (strlen(param)>0);
}

int check_dir_param(char *param)
{
    return 1;
}

// int check_color_param(char *param);

int check_perm_param(char *param)
{
    regex_t perm_regex;
    int test = regcomp(&perm_regex,"[0-7][0-7][0-7]",0);
    if (test){
        printf("Regex error\n");
        regfree(&perm_regex);
        return 0;
    }
    test = regexec(&perm_regex,param,0,NULL,0);
    regfree(&perm_regex);
    if (test == REG_NOMATCH)
    {
        //printf("Invalid argument %s for -perm option.\n",param);
        return 0;
    }
    return 1;
}

// int check_link_param(char *param);

int check_threads_param(char *param)
{
    regex_t thread_regex;
    int test = regcomp(&thread_regex,"[0-9]\\+",0);
    if (test){
        printf("Regex error\n");
        regfree(&thread_regex);
        return 0;
    }
    test = regexec(&thread_regex,param,0,NULL,0);
    regfree(&thread_regex);
    if (test == REG_NOMATCH)
    {
        //printf("Invalid argument %s for -thread option.\n",param);
        return 0;
    }
    return 1;
};

// int check_ou_param(char *param);
// int check_test_param(char *param);
