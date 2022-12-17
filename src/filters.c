#include "filters.h"
#include "parser.h"
#include "MegaMimes.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysmacros.h>

// Fonctions auxiliaires
long get_size(char *path)
{
    struct stat sb;
    if (stat(path, &sb) == -1)
    {
        perror("stat");
        return 0;
    }
    return (long)sb.st_size;
}

int filter_with_regex(char *regex, char *data)
/*
    Fonction qui vérifie que la regex match avec la donnée.

    Paramètres :
    - regex (char *) : Regex
    - data (char *) : Donnée à comparer avec la regex

    La valeur de retour est un booléen.
*/
{
    regex_t regex_filter;
    int test = regcomp(&regex_filter, regex, REG_EXTENDED);
    if (test)
    {
        printf("Regex error\n");
        regfree(&regex_filter);
        return 0;
    }
    test = regexec(&regex_filter, data, 0, NULL, 0);
    regfree(&regex_filter);
    if (test == REG_NOMATCH)
    {
        return 0;
    }
    return 1;
}

int filter_with_regex_except_ctc(char *regex, char *data)
/*
    Fonction qui vérifie que la regex match avec la donnée.

    Paramètres :
    - regex (char *) : Regex
    - data (char *) : Donnée à comparer avec la regex

    La valeur de retour est un booléen.
*/
{
    regex_t regex_filter;
    char reg[512] = "^";
    strcat(reg, regex);
    strcat(reg, "$");
    int test = regcomp(&regex_filter, reg, REG_EXTENDED);
    if (test)
    {
        printf("Regex error\n");
        regfree(&regex_filter);
        return 0;
    }
    test = regexec(&regex_filter, data, 0, NULL, 0);
    regfree(&regex_filter);
    if (test == REG_NOMATCH)
    {
        return 0;
    }
    return 1;
}

int parse_size_param(char param[256])
/*
    Fonction qui convertit le paramtre du flag -size en bytes.

    Paramètres :
    - param (char[256]) : Paramètre du flag

    La valeur de retour est un entier.
*/
{
    char temp[256];
    char unit = param[strlen(param) - 1];
    switch (unit)
    {
    case 'c':
        strncpy(temp, param, strlen(param) - 1);
        return atoi(temp);

    case 'k':
        strncpy(temp, param, strlen(param) - 1);
        return atoi(temp) * 1024;

    case 'M':
        strncpy(temp, param, strlen(param) - 1);
        return atoi(temp) * 1048576;

    case 'G':
        strncpy(temp, param, strlen(param) - 1);
        return atoi(temp) * 1073741824;

    default:
        return atoi(param);
    }
}

int parse_date_param(char param[256])
/*
    Fonction qui convertit le paramtre du flag -date en secondes.

    Paramètres :
    - param (char[256]) : Paramètre du flag

    La valeur de retour est un entier.
*/
{
    char temp[256];
    char unit = param[strlen(param) - 1];
    switch (unit)
    {
    case 'j':
        strncpy(temp, param, strlen(param) - 1);
        return atoi(temp) * 86400;

    case 'h':
        strncpy(temp, param, strlen(param) - 1);
        return atoi(temp) * 3600;

    case 'm':
        strncpy(temp, param, strlen(param) - 1);
        return atoi(temp) * 60;

    default:
        return atoi(param);
    }
}

// -----
// Flags sans filtres
int no_filter(char *path, struct dirent *file, option *opt)
/*
    Fonction utile aux flags qui n'effectuent aucun filtrage des résultats du parcours.
    Les paramètres de cette option n'existe que pour respecter le formalisme de la structure "option" définie dans le fichier "parser.h".

    Paramètres :
    - path (char *) : chemin du fichier
    - file (struct dirent *) : fichier
    - opt (option *) : Flag

    La valeur de retour est un booléen (vrai).
*/
{
    return 1;
}

// -----
// Flags avec filtres
int name_filter(char *path, struct dirent *file, option *opt)
/*
    Fonction qui filtre les résultats du parcours suivant le paramètre du flag -name.

    Paramètres :
    - path (char *) : chemin du fichier
    - file (struct dirent *) : fichier
    - opt (option *) : Flag

    La valeur de retour est un booléen.
*/
{
    return filter_with_regex_except_ctc(opt->parameter_value, file->d_name);
}

int size_filter(char *path, struct dirent *file, option *opt)
/*
    Fonction qui filtre les résultats du parcours suivant le paramètre du flag -size.

    Paramètres :
    - path (char *) : chemin du fichier
    - file (struct dirent *) : fichier
    - opt (option *) : Flag

    La valeur de retour est un booléen.
*/
{
    struct stat sb;
    int size;
    int filter;
    if (stat(path, &sb) == -1)
    {
        perror("stat");
        return 0;
    }
    switch (opt->parameter_value[0])
    {
    case '+':
        filter = parse_size_param(opt->parameter_value + 1);
        size = (int)sb.st_size;
        return (size > filter);

    case '-':
        filter = parse_size_param(opt->parameter_value + 1);
        size = (int)sb.st_size;
        return (size < filter);

    default:
        filter = parse_size_param(opt->parameter_value);
        size = (int)sb.st_size;
        return (size == filter);
    }
}

int date_filter(char *path, struct dirent *file, option *opt)
/*
    Fonction qui filtre les résultats du parcours suivant le paramètre du flag -date.

    Paramètres :
    - path (char *) : chemin du fichier
    - file (struct dirent *) : fichier
    - opt (option *) : Flag

    La valeur de retour est un booléen.
*/
{
    struct stat sb;
    long int dateFromLastAccess;
    long int filter;
    time_t current_time = 0;
    time(&current_time);
    if (stat(path, &sb) == -1)
    {
        perror("stat");
        return 0;
    }
    switch (opt->parameter_value[0])
    {
    case '+':
        filter = (long int)parse_date_param(opt->parameter_value + 1);
        dateFromLastAccess = current_time - ((long int)sb.st_atime);
        return (dateFromLastAccess >= filter);

    default:
        filter = (long int)parse_date_param(opt->parameter_value);
        dateFromLastAccess = current_time - ((long int)sb.st_atime);
        return (dateFromLastAccess <= filter);
    }
}

int mime_filter(char *path, struct dirent *file, option *opt)
/*
    Fonction qui filtre les résultats du parcours suivant le paramètre du flag -mime.

    Paramètres :
    - path (char *) : chemin du fichier
    - file (struct dirent *) : fichier
    - opt (option *) : Flag

    La valeur de retour est un booléen.
*/
{
    const char *mimetype = getMegaMimeType(file->d_name);
    char filter[256];
    int test;
    strcpy(filter, opt->parameter_value);
    if (mimetype == NULL)
    {
        return 0;
    }
    if ((strchr(opt->parameter_value, '/') == NULL)) // Cas où on a l'option "-mime type"
    {
        strcat(filter, "/");
    }
    test = !strncmp(filter, mimetype, strlen(filter));
    return test;
}

int ctc_filter(char *path, struct dirent *file, option *opt)
/*
    Fonction qui filtre les résultats du parcours suivant le paramètre du flag -ctc.

    Paramètres :
    - path (char *) : chemin du fichier
    - file (struct dirent *) : fichier
    - opt (option *) : Flag

    La valeur de retour est un booléen.
*/
{
    long length = get_size(path);
    int match = 0;
    FILE *f = fopen(path, "r");

    if (!f)
    {
        return 0;
    }

    char *buffer = calloc(1, length);

    while (fgets(buffer, length, f) && !match)
    {
        match = filter_with_regex(opt->parameter_value, buffer);
    }

    fclose(f);
    free(buffer);
    return match;
}

int dir_filter(char *path, struct dirent *file, option *opt)
/*
    Fonction qui filtre les résultats du parcours
    - en vérifiant qu'il s'agisse bien de répertoires
    - suivant le paramètre du flag -dir (s'il y en a un).

    Paramètres :
    - path (char *) : chemin du fichier
    - file (struct dirent *) : fichier
    - opt (option *) : Flag

    La valeur de retour est un booléen.
*/
{
    struct stat sb;
    if (stat(path, &sb) == -1)
    {
        perror("lstat");
        return 0;
    }
    if ((sb.st_mode & __S_IFMT) == __S_IFDIR)
    {
        if (strlen(opt->parameter_value))
        {
            return filter_with_regex_except_ctc(opt->parameter_value, file->d_name);
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

int perm_filter(char *path, struct dirent *file, option *opt)
/*
    Fonction qui filtre les résultats du parcours suivant le paramètre du flag -perm.

    Paramètres :
    - path (char *) : chemin du fichier
    - file (struct dirent *) : fichier
    - opt (option *) : Flag

    La valeur de retour est un booléen.
*/
{
    struct stat sb;
    char str[255];
    if (stat(path, &sb) == -1)
    {
        perror("lstat");
        return 0;
    }
    __uintmax_t mode = ((__uintmax_t)sb.st_mode);
    sprintf(str, "%jo", mode);
    char perm[4];
    for (int i = 3; i > 0; i--)
    {
        perm[3 - i] = str[strlen(str) - i];
    }
    char *filter = opt->parameter_value;
    return (!strcmp(filter, perm));
}

// -----
// int color_filter(char* path, struct dirent *file, struct option opt);
// int link_filter(char* path, struct dirent *file, struct option opt);
// int threads_filter(char* path, struct dirent *file, struct option opt);
// int ou_filter(char* path, struct dirent *file, struct option opt);
// int test_filter(char* path, struct dirent *file, struct option opt);

// -----