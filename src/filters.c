#include "filters.h"
#include "parser.h"

int no_filter(char* path, struct dirent *file, option *opt)
{
    return 1;
}

int name_filter(char* path, struct dirent *file, option *opt)
{
    regex_t name_filter_regex;
    int test = regcomp(&name_filter,opt->parameter_value,REG_EXTENDED);
    if (test){
        printf("Regex error\n");
        regfree(&name_filter_regex);
        return 0;
    }
    test = regexec(&name_filter_regex,file->d_name,0,NULL,0);
    regfree(&name_filter_regex);
    if (test == REG_NOMATCH)
    {
        return 0;
    }
    return 1;
}

// int size_filter(char* path, struct dirent *file, struct option opt);
// int date_filter(char* path, struct dirent *file, struct option opt);
// int mime_filter(char* path, struct dirent *file, struct option opt);
// int ctc_filter(char* path, struct dirent *file, struct option opt);
// int dir_filter(char* path, struct dirent *file, struct option opt);
// int color_filter(char* path, struct dirent *file, struct option opt);
// int perm_filter(char* path, struct dirent *file, struct option opt);
// int link_filter(char* path, struct dirent *file, struct option opt);
// int threads_filter(char* path, struct dirent *file, struct option opt);
// int ou_filter(char* path, struct dirent *file, struct option opt);
// int test_filter(char* path, struct dirent *file, struct option opt);