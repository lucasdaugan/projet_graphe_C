#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tarjan.h"
#include "hasse.h"
#include "utils.h"

// Affiche les composantes au format demandé
void print_partition(t_partition *P)
{
    int ci;
    int k;

    // On parcourt toutes les classes
    for (ci = 0; ci < P->count; ci++) {
        t_classe *C = &P->classes[ci];
        
        if (C->name[0] != 0){printf("Composante %s: {", C->name);}
        else {printf("Composante (C?): {");}

        // Affichage des sommets de la classe
        for (k = 0; k < C->count; k++)
        {
            printf("%d", C->verts[k]);
            if (k + 1 < C->count){printf(",");}
        }
        printf("}\n");
    }
}

// Détermine si une classe est transitoire
int classe_est_transitoire(int idx_classe, t_link_array *L)
{
    int i;
    for (i = 0; i < L->log_size; i++){
        if (L->links[i].from == idx_classe){return 1;}
    } return 0;
}

// Compte les liens sortants d'une classe
int degre_sortant(int idx_classe, t_link_array *L)
{
    int d = 0;
    int i;
    for (i = 0; i < L->log_size; i++){
        if (L->links[i].from == idx_classe) {d++;}
    }
    return d;
}

// Affiche les caractéristiques demandées (transitoire/persistante, absorbant, irréductible)
void print_characteristics(t_partition *P, t_link_array *L)
{
    int irreductible = 0;
    if (P->count == 1) {irreductible = 1;}

    int any_absorbant = 0;
    int ci;

    printf("\n--- Caracteristiques du graphe ---\n");
    for (ci = 0; ci < P->count; ci++){
        t_classe *C = &P->classes[ci];
        
        // Gestion du nom simplifiee
        char *cname = C->name; 
        if (cname[0] == 0) {cname = "(C?)";}

        // On regarde si c'est transitoire
        int trans = classe_est_transitoire(ci, L);
        if (trans == 1){printf("La classe %s est transitoire", cname);}
        else {printf("La classe %s est persistante", cname);}

        // Verification pour l'etat absorbant :
        if (trans == 0 && C->count == 1) {
            printf(" - l'etat %d est absorbant", C->verts[0]); 
            any_absorbant = 1;
        }

        int nb_sorties = degre_sortant(ci, L);
        printf(" (|C|=%d, sorties=%d)\n", C->count, nb_sorties);
    }

    // Affichage final
    printf("\nEtat(s) absorbant(s) : ");
    if (any_absorbant == 1) {printf("OUI\n");}
    else {printf("NON\n");}

    printf("Graphe irreductible : ");
    if (irreductible == 1) {printf("OUI (une seule classe)\n");}
    else {printf("NON\n");}
}

