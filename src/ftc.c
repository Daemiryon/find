#include "parser.h"
#include "filters.h"
#include "check_param.h"
#include <sys/stat.h>
#include <dirent.h>

// Attribution à chaque option d'un indice donnant sa position dans le tableau opt_table
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

// Nombre d'options de la commande (source incluse)
#define NBOPT 13

// Définition des couleurs
#define COLOR_FILE 34
#define COLOR_FOLDER 33
#define COLOR_ERROR 31

void colored_print(char *txt, int color)
/*
    Fonction qui affiche en sortie standard le paramètre txt avec la couleur souhaitée. Utilise la coloration avec les escape characters.

    Paramètres :
    - txt (char *) : Chaîne de caractère à afficher
    - color (int) : Valeur de la couleur
*/
{
    printf("\33[%dm%s\33[0m\n", color, txt);
}

int check_arg(option **opt_table, int argc, const char *argv[])
/*
    Fonction qui vérifie que le paramètre de la commande a bien été indiqué (renvoie une erreur sinon) et appelle la fonction parser (cf fichiers parser.h et parser.c).

    Paramètres :
    - txt (option **) : Chaîne de caractère à afficher
    - color (int) : Valeur de la couleur

    La valeur de retour est un booléen.
*/
{
    if (argc == 1)
    {
        printf("Missing argument\n");
        return 0;
    }
    return parser(opt_table, argc-1, argv+1);
}

int filter(char *path, struct dirent *file, option_table opt_table)
/*
    Fonction qui passe le fichier en paramètre dans les filtres de toutes les options.
    La valeur de retour dépend de si l'option -OU est activée. Dans ce cas, au moins un des filtres doit accepter le fichier.
    Dans le cas contraire, tous les filtres doivent accepter le fichier.

    Paramètres :
    - path (char *) : chemin du fichier
    - file (struct dirent) : fichier
    - opt_table (option_table) : Tableau des options de la commande

    La valeur de retour est un booléen.
*/
{
    int ANDcheck = 1;
    int ORcheck = 0;

    for (int i = 0; i < 12; i++)
    {

        if ((opt_table[i]->opt_filter != NULL) & (opt_table[i]->activated > 0))
        {
            int b = opt_table[i]->opt_filter(path, file, opt_table[i]);
            ANDcheck = ANDcheck & b;
            ORcheck = ORcheck | b;
        }
    }
    int check = opt_table[OU]->activated ? ORcheck : ANDcheck;
    return check;
}

int parcour(char *path, option_table opt_table, int depth)
/*
    Fonction qui parcourt l'arborescence des fichiers à partir du chemin spécifié.

    Paramètres :
    - path (char *) : chemin utilisé comme point de départ du parcours
    - opt_table (option_table) : Tableau des options de la commande
    - depth (int) : Profondeur de l'arbre

    La valeur de retour est un booléen.
*/
{
    int found = 0;
    struct dirent *current;
    DIR *d = opendir(path);
    if (path[strlen(path) - 1] != '/')
    {
        strcat(path, "/");
    }

    while (d && (current = readdir(d)) != NULL)
    {
        char PATH[512];
        strcpy(PATH, path);
        strcat(PATH, current->d_name);

        if (current->d_name[0] == '.')
        {
            // Cas du fichier "."
            if (strlen(current->d_name) == 1)
            {
                if ((!depth) & (filter(path, current, opt_table)))
                {
                    if (opt_table[COLOR]->activated)
                    {
                        colored_print(path, COLOR_FOLDER);
                    }
                    else
                    {
                        printf("%s\n", path);
                    }
                    found = 1;
                }
                continue;
            }
            // Fichier ".." non prix en compte
            if (strcmp(current->d_name, "..") == 0)
            {
                continue;
            }
        }

        // Autres fichiers (hors répertoires)
        if (filter(PATH, current, opt_table))
        {
            if (opt_table[COLOR]->activated)
            {
                int color = current->d_type == 4 ? COLOR_FOLDER : COLOR_FILE;
                colored_print(PATH, color);
            }
            else
            {
                printf("%s\n", PATH);
            }
            found = 1;
        }

        // Cas des répertoires
        if (current->d_type == 4)
        {
            found = found | parcour(PATH, opt_table, depth + 1);
        }
    }

    closedir(d);
    return found;
}

int main(int argc, const char *argv[])
{
    // Initialisation du tableau d'option
    option *opt_table[NBOPT];

    opt_table[NAME] = init_option("name", &check_name_param, &name_filter);
    opt_table[SIZE] = init_option("size", &check_size_param, &size_filter);
    opt_table[DATE] = init_option("date", &check_date_param, &date_filter);
    opt_table[MIME] = init_option("mime", &check_mime_param, &mime_filter);
    opt_table[CTC] = init_option("ctc", &check_ctc_param, &ctc_filter);
    opt_table[_DIR] = init_option("dir", &check_dir_param, &dir_filter);
    opt_table[COLOR] = init_option("color", &check_no_param, NULL);
    opt_table[PERM] = init_option("perm", &check_perm_param, &perm_filter);
    opt_table[LINK] = init_option("link", &check_no_param, &no_filter);
    opt_table[THREADS] = init_option("threads", &check_threads_param, NULL);
    opt_table[OU] = init_option("ou", &check_no_param, NULL);
    opt_table[TEST] = init_option("test", &check_no_param, NULL);
    opt_table[SOURCE] = init_option("source", &check_source_param, NULL);

    opt_table[SOURCE]->activated = 1;

    // Parcours de l'argv et vérification des paramètres des options
    if (!check_arg(opt_table, argc, argv))
    {
        exit(EXIT_FAILURE);
    }

    // Cas où l'option -test est activée. Dans ce cas, affiche sur l'entrée standard le paramètre de la première option activée rencontrée lors du parcours de opt_table
    if (opt_table[TEST]->activated)
    {
        for (int i = 0; i < NBOPT - 2; i++)
        {
            if ((opt_table[i]->activated))
            {
                printf("La valeur du flag -%s est %s\n", opt_table[i]->name, argv[opt_table[i]->activated + 1]);
                break;
            }
        }
        destroy_option_table(opt_table, NBOPT);
        exit(EXIT_SUCCESS);
    }

    // Affichage des messages d'erreurs indiquant que les paramètres des options activées sont incorrects (Si c'est le cas)
    int arg_check = 1;
    for (int i = 0; i < NBOPT; i++)
    {
        if (opt_table[i]->activated)
        {
            if (!opt_table[i]->check_opt_parameter(opt_table[i]->parameter_value))
            {
                int color = COLOR_ERROR * (opt_table[COLOR]->activated > 0);
                printf("\33[%dmError : Invalid argument for flag -%s \33[0m\n", color, opt_table[i]->name);
                arg_check = 0;
            }
        }
    }
    if (!arg_check)
    {
        exit(EXIT_FAILURE);
    }

    // Parcours et filtrage des fichiers de l'arborescence
    parcour(opt_table[SOURCE]->parameter_value, opt_table, 0);

    destroy_option_table(opt_table, NBOPT);
    return 0;
}
