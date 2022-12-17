#include "parser.h"

option *init_option(char *opt_name, int (*check_opt_parameter)(char *), int (*opt_filter)(char *, struct dirent *, option *))
/*
    Fonction qui instancie une option.

    Paramètres :
    - opt_name (char *) : Nom de l'option
    - check_opt_parameter() (int *) : Fonction (pointeur) de vérification du paramètre du flag
    - check_opt_parameter() (int *) : Fonction (pointeur) de filtrage par rapport au paramètre du flag

    La valeur de retour est une structure de type option.
*/
{
    option *opt = calloc(1, sizeof(option));
    opt->name = opt_name;
    opt->activated = 0;
    opt->check_opt_parameter = *check_opt_parameter;
    opt->opt_filter = *opt_filter;
    return opt;
}

void destroy_option(option *opt)
/*
    Fonction qui détruit l'option passée en paramètre

    Paramètres :
    - opt (option *) : Structure d'option à détruire
*/
{
    free(opt);
}

int update_option(option_table opt_table, const char *opt, int index)
/*
    Fonction qui vérifie si l'option passée en paramètre existe et modifie l'attribut activated de celle-ci.

    Paramètres :
    - opt_table (option_table) : Tableau contenant toutes les options de la commande
    - opt (const char *) : Nom de l'option dont on vérifie l'existence
    - index (int) : Position de l'option dans la variable argv du main

    La valeur de retour est un entier qui vaut -1 si l'option n'existe pas, l'indice de l'option dans opt_table sinon.
*/
{
    for (unsigned int j = 0; j < 13; j++)
    {
        if (!strcmp(opt_table[j]->name, opt))
        {
            opt_table[j]->activated = index;
            return j;
        }
    }
    return -1;
}

int parser(option_table opt_table, int argc, const char *argv[])
/*
    Fonction qui parcourt l'entrée standard', active les options rencontrées et leur affecte le paramètre correspondant.
    Si l'option n'existe pas, elle affiche une erreur et renvoie faux.

    Paramètres :
    - opt_table (option_table) : Tableau contenant toutes les options de la commande
    - argc (int) : Nombre d'éléments dans la variable argv[]
    - argv[] (char *) : Entrée standard

    La valeur de retour est un booléen.
*/
{
    int flag = 12;
    for (int i = 0; i < argc; i++)
    {
        // Cas où on a un paramètre d'option à mettre (si cette option n'a pas déjà de paramètres)
        if ((opt_table[flag]->check_opt_parameter((char *)argv[i])) & (!strlen(opt_table[flag]->parameter_value)))
        {
            strcpy(opt_table[flag]->parameter_value, argv[i]);
        }

        // Cas où on rencontre une option
        else if (argv[i][0] == '-')
        {
            flag = update_option(opt_table, argv[i]+1, i+1);
            if (flag == -1)
            {
                printf("Le flag -%s n'est pas correct\n", argv[i] + 1);
                return 0;
            }
        }
    }

    return 1;
}

void destroy_option_table(option_table opt_table, int size)
/*
    Fonction qui détruit l'option passée en paramètre

    Paramètres :
    - opt_table (option_table) : Tableau contenant toutes les options de la commande
    - size (int) : Taille du tableau
*/
{
    for (int i = 0; i < size; i++)
    {
        destroy_option(opt_table[i]);
    }
}
