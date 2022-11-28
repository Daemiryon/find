#include "filters.h"
#include "parser.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysmacros.h>

int filter_with_regex(char *regex,char* data)
{
    regex_t regex_filter;
    int test = regcomp(&regex_filter,regex,REG_EXTENDED);
    if (test){
        printf("Regex error\n");
        regfree(&regex_filter);
        return 0;
    }
    test = regexec(&regex_filter,data,0,NULL,0);
    regfree(&regex_filter);
    if (test == REG_NOMATCH)
    {
        return 0;
    }
    return 1;
}

int parse_size_param(char param[256])
{
    char temp[256];
    char unit = param[strlen(param)-1];
    switch (unit)
    {
        case 'c':
            strncpy(temp,param,strlen(param)-1);
            return atoi(temp);
        
        case 'k':
            strncpy(temp,param,strlen(param)-1);
            return atoi(temp)*1024;
        
        case 'M':
            strncpy(temp,param,strlen(param)-1);
            return atoi(temp)*1048576;
        
        case 'G':
            strncpy(temp,param,strlen(param)-1);
            return atoi(temp)*1073741824;
        
        default:
            return atoi(param);
    }
}


int no_filter(char* path, struct dirent *file, option *opt)
{
    return 1;
}

int name_filter(char* path, struct dirent *file, option *opt)
{
    return filter_with_regex(opt->parameter_value,file->d_name);
}

int size_filter(char* path, struct dirent *file, option *opt)
{
    struct stat sb;
    int size;
    int filter;
    if (stat(path, &sb) == -1) {
        perror("stat");
        return 0;
    }
    switch (opt->parameter_value[0])
    {
        case '+':
            filter = parse_size_param(opt->parameter_value+1);
            size = (int) sb.st_size;
            return (size > filter);

        case '-':
            filter = parse_size_param(opt->parameter_value+1);
            size = (int) sb.st_size;
            return (size < filter);

        default:
            filter = parse_size_param(opt->parameter_value);
            size = (int) sb.st_size;
            return (size == filter);
    }
}

// int date_filter(char* path, struct dirent *file, struct option opt);
// int mime_filter(char* path, struct dirent *file, struct option opt);
// int ctc_filter(char* path, struct dirent *file, struct option opt);

int dir_filter(char* path, struct dirent *file, option *opt)
{
    struct stat sb;
    if (stat(path, &sb) == -1) {
        perror("lstat");
        return 0;
    }
    if ((sb.st_mode & __S_IFMT) == __S_IFDIR)
    {
        if (strlen(opt->parameter_value))
        {
            return filter_with_regex(opt->parameter_value,file->d_name);
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return 0;
    }
}

// int color_filter(char* path, struct dirent *file, struct option opt);
// int perm_filter(char* path, struct dirent *file, struct option opt);
// int link_filter(char* path, struct dirent *file, struct option opt);
// int threads_filter(char* path, struct dirent *file, struct option opt);
// int ou_filter(char* path, struct dirent *file, struct option opt);
// int test_filter(char* path, struct dirent *file, struct option opt);
