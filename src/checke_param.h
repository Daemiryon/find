#ifndef __CHECKE_PARAM_H__
#define __CHECKE_PARAM_H__

#include "parser.h"

int check_no_param(option *opt);

int check_name_param(option *opt);
int check_size_param(option *opt);
int check_date_param(option *opt);
int check_mime_param(option *opt);
int check_ctc_param(option *opt);
int check_dir_param(option *opt);
int check_color_param(option *opt);
int check_perm_param(option *opt);
int check_link_param(option *opt);
int check_threads_param(option *opt);
int check_ou_param(option *opt);
int check_test_param(option *opt);

#endif
