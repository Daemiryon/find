#include "check_param.h"
#include "MegaMimes.h"
#include <dirent.h>

int check_with_regex(char* param, char* regex){
    regex_t regex_struct;
    int test = regcomp(&regex_struct,regex,0);
    if (test){
        printf("Regex error\n");
        regfree(&regex_struct);
        return 0;
    }
    test = regexec(&regex_struct,param,0,NULL,0);
    regfree(&regex_struct);
    if (test == REG_NOMATCH)
    {
        printf("No match");
        return 0;
    }
    return 1;
}


int check_no_param(char *param)
{
    // if (strlen(param))
    // {
    //     // printf("Invalid argument %s : option does not take any argument.\n", param);
    //     return 0;
    // }
    // return 1;
    return !strlen(param);
}

int check_name_param(char *param)
{
    return (strlen(param)>0);
}

int check_size_param(char *param)
{
    return check_with_regex(param,"[+-]\\?[0-9]\\+[ckMG]");
}

int check_date_param(char *param)
{
    return check_with_regex(param,"[+]\\?[0-9]\\+[jhm]");
}

int check_mime_param(char *param){
    char mime[256];
    int test = 1;
    strcpy(mime,param);
    if (strchr(mime,'/') == NULL)                          // Cas où on a l'option "-mime type"
    {
        strcat(mime,"/*");  
    }
    const char **extensions = getMegaMimeExtensions(mime);
    if (extensions == NULL)
    {
        test = 0;
    }
    freeMegaStringArray( (char**) extensions);
    return test;
}

int check_ctc_param(char *param)
{
    return (strlen(param)>0);
}

int check_dir_param(char *param)
{
    return !(param[0] == '-');
}

// int check_color_param(char *param);     //= check_no_param()

int check_perm_param(char *param)
{
    return check_with_regex(param,"[0-7][0-7][0-7]");
}

// int check_link_param(char *param);     //= check_no_param()

int check_threads_param(char *param)
{
    return check_with_regex(param,"[0-9]\\+");
}

// int check_ou_param(char *param);       //= check_no_param()
// int check_test_param(char *param);     //= check_no_param()

int check_source_param(char *param)
{
    DIR *d = opendir(param);
    if (!d)
    {
        // printf("%s directory not found.\n", param);
        free(d);
        return 0;
    }
    free(d);
    return 1;
}
