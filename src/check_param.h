#ifndef __CHECKE_PARAM_H__
#define __CHECKE_PARAM_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

int check_no_param(char *param);

int check_name_param(char *param);
int check_size_param(char *param);
int check_date_param(char *param);
int check_mime_param(char *param);
int check_ctc_param(char *param);
int check_dir_param(char *param);
// int check_color_param(char *param);      // = check_no_param()
int check_perm_param(char *param);
// int check_link_param(char *param);       // = check_no_param()
int check_threads_param(char *param);
// int check_ou_param(char *param);         // = check_no_param()
// int check_test_param(char *param);       // = check_no_param()
int check_source_param(char *param);

#endif
