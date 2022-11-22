#include "filters.h"

int no_filter(char* path, struct dirent *file, option *opt){
    return 1;
}

// int name_filter(char* path, struct dirent *file, struct option opt);
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