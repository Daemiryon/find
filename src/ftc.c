#include "parser.h"
#include "check_param.h"
#include <dirent.h>

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

#define NBOPT 12

int check_arg(option **table, int argc, const char *argv[])
{
    if (argc == 1)
    {
        printf("Missing argument\n");
        return 0;
    }
    DIR *d = opendir(argv[1]);
    if (!d)
    {
        printf("%s directory not found.\n", argv[1]);
        free(d);
        return 0;
    }
    free(d);
    if (argc > 2)
    {
        return parser(table, argc - 2, argv + 2);
    }

    return 1;
}

int parcour(const char *name, option_table table, int depth)
{
    int found = 0;
    struct dirent *current;
    DIR *d = opendir(name);

    while (d && (current = readdir(d)) != NULL)
    {
        if (current->d_name[0] == '.')
        {
            continue;
        }

        printf("%s/%s\n", name, current->d_name);
        // printf("\33[%dC├ %s\n", depth * 3, current->d_name);
        if (current->d_type == 4)
        {
            char PATH[512];
            strcpy(PATH, name);
            if (PATH[strlen(PATH) - 1] != '/')
            {
                strcat(PATH, "/");
            }

            strcat(PATH, current->d_name);

            found = found | parcour(PATH, table, depth + 1);
        }
    }
    closedir(d);
    return found;
}

int main(int argc, const char *argv[])
{
    option *optable[NBOPT];

    optable[NAME] = init_option("name", &check_name_param);
    optable[SIZE] = init_option("size", &check_no_param);
    optable[DATE] = init_option("date", &check_no_param);
    optable[MIME] = init_option("mime", &check_no_param);
    optable[CTC] = init_option("ctc", &check_no_param);
    optable[_DIR] = init_option("dir", &check_no_param);
    optable[COLOR] = init_option("color", &check_no_param);
    optable[PERM] = init_option("perm", &check_no_param);
    optable[LINK] = init_option("link", &check_no_param);
    optable[THREADS] = init_option("threads", &check_no_param);
    optable[OU] = init_option("ou", &check_no_param);
    optable[TEST] = init_option("test", &check_no_param);

    if (!check_arg(optable, argc, argv))
    {
        exit(EXIT_FAILURE);
    }

    if (optable[TEST]->activated)
    {
        for (int i = 0; i < NBOPT; i++)
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
