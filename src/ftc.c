#include "parser.h"
#include "filters.h"
#include "check_param.h"
#include <sys/stat.h>

#define NAME 0
#define SIZE 1
#define DATE 2
#define MIME 3
#define CTC 4
#define _DIR 5
#define COLOR 6
#define PERM 7
#define LINK 8
#define THREADS 9
#define OU 10
#define TEST 11
#define SOURCE 12

#define NBOPT 13

int check_arg(option **table, int argc, const char *argv[])
{
    if (argc == 1)
    {
        printf("Missing argument\n");
        return 0;
    }

    return parser(table, argc - 1, argv + 1);

}

int filter(char *path, char *name, option_table table)
{
    int check = 1;
    struct stat *stat;
    lstat(path, stat);

    if (table[NAME]->activated)
    {
    }
    if (table[_DIR]->activated)
    {
    }
    if (table[SIZE]->activated)
    {
    }
    if (table[PERM]->activated)
    {
    }
    return check;
}

int parcour(const char *path, option_table table, int depth)
{
    int found = 0;
    struct dirent *current;
    DIR *d = opendir(path);

    while (d && (current = readdir(d)) != NULL)
    {
        char PATH[512];
        strcpy(PATH, path);
        if (PATH[strlen(PATH) - 1] != '/')
        {
            strcat(PATH, "/");
        }
        strcat(PATH, current->d_name);

        if (current->d_name[0] == '.')
        {
            continue;
        }

        if ((!table[NAME]->activated) | (strcmp(table[NAME]->parameter_value, current->d_name) == 0))
        {
            printf("%s\n", PATH);
        }

        if (current->d_type == 4)
        {
            found = found | parcour(PATH, table, depth + 1);
        }
    }

    closedir(d);
    return found;
}

int main(int argc, const char *argv[])
{
    option *optable[NBOPT];

    optable[NAME] = init_option("name", &check_name_param, &no_filter);
    optable[SIZE] = init_option("size", &check_size_param, &no_filter);
    optable[DATE] = init_option("date", &check_date_param, &no_filter);
    optable[MIME] = init_option("mime", &check_mime_param, &no_filter);
    optable[CTC] = init_option("ctc", &check_ctc_param, &no_filter);
    optable[_DIR] = init_option("dir", &check_dir_param, &no_filter);
    optable[COLOR] = init_option("color", &check_no_param, &no_filter);
    optable[PERM] = init_option("perm", &check_perm_param, &no_filter);
    optable[LINK] = init_option("link", &check_no_param, &no_filter);
    optable[THREADS] = init_option("threads", &check_threads_param, &no_filter);
    optable[OU] = init_option("ou", &check_no_param, &no_filter);
    optable[TEST] = init_option("test", &check_no_param, &no_filter);
    optable[SOURCE] = init_option("source", &check_source_param, &no_filter);

    optable[SOURCE]->activated=1;

    if (!check_arg(optable, argc, argv))
    {
        exit(EXIT_FAILURE);
    }

    if (optable[TEST]->activated)
    {
        for (int i = 0; i < NBOPT-2; i++)
        {
            if ((optable[i]->activated))
            {
                printf("La valeur du flag -%s est %s\n", optable[i]->name, optable[i]->parameter_value);
                break;
            }
        }
        destroy_optable(optable, NBOPT);
        exit(EXIT_SUCCESS);
    }


    parcour(argv[1], optable, 0);

    destroy_optable(optable, NBOPT);

    return 0;
}
