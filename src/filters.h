#ifndef __FILTERS_H__
#define __FILTERS_H__

#include "parser.h"
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <regex.h>

int no_filter(char* path, struct dirent *file, option *opt);

int name_filter(char* path, struct dirent *file, option *opt);
int size_filter(char* path, struct dirent *file, option *opt);
int date_filter(char* path, struct dirent *file, option *opt);
int mime_filter(char* path, struct dirent *file, option *opt);
int ctc_filter(char* path, struct dirent *file, option *opt);
int dir_filter(char* path, struct dirent *file, option *opt);
int color_filter(char* path, struct dirent *file, option *opt);
int perm_filter(char* path, struct dirent *file, option *opt);
int link_filter(char* path, struct dirent *file, option *opt);
int threads_filter(char* path, struct dirent *file, option *opt);
int ou_filter(char* path, struct dirent *file, option *opt);
int test_filter(char* path, struct dirent *file, option *opt);

#endif
