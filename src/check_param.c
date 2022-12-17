#include "check_param.h"
#include "MegaMimes.h"
#include <dirent.h>

// -----
// Fonctions auxilliaires
int check_with_regex(char *param, char *regex)
/*
    Fonction qui vérifie que la regex match avec le paramètre du flag.

    Paramètres :
    - param (char *) : Paramètre du flag
    - regex (char *) : Regex que doit vérifier le paramètre du flag

    La valeur de retour est un booléen.
*/
{
    regex_t regex_struct;
    int test = regcomp(&regex_struct,regex,REG_EXTENDED);
    if (test){
        printf("Regex error\n");
        regfree(&regex_struct);
        return 0;
    }
    test = regexec(&regex_struct,param,0,NULL,0);
    regfree(&regex_struct);
    if (test == REG_NOMATCH)
    {
        return 0;
    }
    return 1;
}

// -----
// Flags sans paramètres
/*
    Les flags -link, -color, -ou et -test ne prennent pas de paramètres.
    La vérification des paramètres fera donc appel à la fonction check_no_param() pour ces flags.
*/
/*
int check_link_param(char *param);
int check_color_param(char *param);
int check_ou_param(char *param);
int check_test_param(char *param);
*/
int check_no_param(char *param)
/*
    Fonction qui vérifie qu'aucun paramètre n'est affecté au flag correspondant.
    Autrement dit, vérifie que le paramètre du flag correspondant a pour longueur 0.

    Paramètres :
    - param (char *) : Paramètre du flag

    La valeur de retour est un booléen.
*/
{
    return !strlen(param);
}

// -----
// Flags avec paramètres
int check_name_param(char *param)
/*
    Fonction qui vérifie que le flag -name a bien un paramètre.
    Autrement dit, vérifie que le paramètre de -name a une longueur positive non nulle.

    Paramètres :
    - param (char *) : Paramètre du flag

    La valeur de retour est un booléen.
*/
{
    return (strlen(param)>0);
}

int check_size_param(char *param)
/*
    Fonction qui vérifie que le paramètre du flag -size respecte le format imposé.
    C'est-à-dire :
    - Le caractère '+' ou le caractère '-' ou rien
    - Au moins un chiffre
    - Et pour l'unité, soit c ou rien (B), soit k (KiB), soit M (MiB), soit G (GiB)

    Paramètres :
    - param (char *) : Paramètre du flag

    La valeur de retour est un booléen.
*/
{
    return check_with_regex(param,"[+-]?[0-9]+[ckMG]");
}

int check_date_param(char *param)
/*
    Fonction qui vérifie que le paramètre du flag -date respecte le format imposé.
    C'est-à-dire :
    - Le caractère '+' ou rien
    - Au moins un chiffre
    - Et pour l'unité, soit j (jours), soit h (heures), soit m (minutes), soit rien (secondes)

    Paramètres :
    - param (char *) : Paramètre du flag

    La valeur de retour est un booléen.
*/
{
    return check_with_regex(param,"[+]?[0-9]+[jhm]");
}

int check_mime_param(char *param)
/*
    Fonction qui vérifie que le paramètre du flag -mime est bien un type mime
    Celui-ci doit être de la forme :
    - type
    - type/sous-type.

    Paramètres :
    - param (char *) : Paramètre du flag

    La valeur de retour est un booléen.
*/
{
    char mime[256];
    int test = 1;
    strcpy(mime,param);
    if (strchr(mime,'/') == NULL)                          // Cas où on a l'option "-mime type"
    {
        strcat(mime,"/*");  
    }
    const char **extensions = getMegaMimeExtensions(mime);
    if (extensions == NULL)
    {
        test = 0;
    }
    freeMegaStringArray((char**) extensions);
    return test;
}

int check_ctc_param(char *param)
/*
    Fonction qui vérifie que le flag -ctc a bien un paramètre.
    Autrement dit, vérifie que le paramètre de -ctc a une longueur positive non nulle.

    Paramètres :
    - param (char *) : Paramètre du flag

    La valeur de retour est un booléen.
*/
{
    return (strlen(param)>0);
}

int check_dir_param(char *param)
/*
    Fonction qui vérifie que le flag -dir a bien un paramètre qui ne commence pas par le caractère '-'.
    On considèrera un répertoire comme valide pour l'option s'il ne commence donc pas par '-'.

    Paramètres :
    - param (char *) : Paramètre du flag

    La valeur de retour est un booléen.
*/
{
    return !(param[0] == '-');
}

int check_perm_param(char *param)
/*
    Fonction qui vérifie que le paramètre du flag -perm respecte le format imposé.
    C'est-à-dire :
    - 3 chiffres entre 0 et 7.

    Paramètres :
    - param (char *) : Paramètre du flag

    La valeur de retour est un booléen.
*/
{
    return check_with_regex(param,"[0-7][0-7][0-7]");
}

int check_threads_param(char *param)
/*
    Fonction qui vérifie que le paramètre du flag -thread respecte le format imposé.
    C'est-à-dire :
    - Un entier strictement positif.

    Paramètres :
    - param (char *) : Paramètre du flag

    La valeur de retour est un booléen.
*/
{
    return check_with_regex(param,"[0-9]+");
}

int check_source_param(char *param)
/*
    Fonction qui vérifie que le paramètre "starting-point" de la commande "ftc" un répertoire qui existe.

    Paramètres :
    - param (char *) : Paramètre du flag

    La valeur de retour est un booléen.
*/
{
    DIR *d = opendir(param);
    if (!d)
    {
        free(d);
        return 0;
    }
    free(d);
    return 1;
}

// -----
