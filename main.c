#include <stdio.h>
#include <stdlib.h>
#include "adj_list.h"
#include "tarjan.h"
#include "hasse.h"
#include <windows.h>

/* ----- Petites fonctions d'affichage utiles (Partie 2) ----- */

// Affiche les composantes au format demandé
static void print_partition(const t_partition *P) {
    for (int ci = 0; ci < P->count; ++ci) {
        const t_classe *C = &P->classes[ci];
        const char *cname = (C->name[0] ? C->name : "(C?)");
        printf("Composante %s: {", cname);
        for (int k = 0; k < C->count; ++k) {
            printf("%d", C->verts[k]);
            if (k + 1 < C->count) printf(",");
        }
        printf("}\n");
    }
}

// Détermine si une classe est transitoire (au moins un lien sortant)
static int classe_est_transitoire(int idx_classe, const t_link_array *L) {
    for (int i = 0; i < L->log_size; ++i) {
        if (L->links[i].from == idx_classe) return 1;
    }
    return 0;
}

// Compte les liens sortants d'une classe
static int degre_sortant(int idx_classe, const t_link_array *L) {
    int d = 0;
    for (int i = 0; i < L->log_size; ++i) {
        if (L->links[i].from == idx_classe) d++;
    }
    return d;
}

// Affiche les caractéristiques demandées (transitoire/persistante, absorbant, irréductible)
static void print_characteristics(const t_partition *P, const t_link_array *L) {
    int irreductible = (P->count == 1);
    int any_absorbant = 0;

    printf("\n--- Caractéristiques du graphe ---\n");
    for (int ci = 0; ci < P->count; ++ci) {
        const t_classe *C = &P->classes[ci];
        const char *cname = (C->name[0] ? C->name : "(C?)");
        int trans = classe_est_transitoire(ci, L);
        if (trans) {
            printf("La classe %s est transitoire", cname);
        } else {
            printf("La classe %s est persistante", cname);
        }

        if (!trans && C->count == 1) {
            // classe persistante de taille 1 => état absorbant
            printf(" – l'état %d est absorbant", C->verts[0]);
            any_absorbant = 1;
        }
        printf(" (|C|=%d, sorties=%d)\n", C->count, degre_sortant(ci, L));
    }

    printf("\nÉtat(s) absorbant(s) : %s\n", any_absorbant ? "OUI" : "NON");
    printf("Graphe irréductible : %s\n", irreductible ? "OUI (une seule classe)" : "NON");
}

int main(void)
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    /* ===== Partie 1 (inchangée) ===== */
    const char *input_graph = "data/exemple_hasse1.txt";
    const char *out_graph_mmd = "output/graph_mermaid6.mmd";

    adjlist_t g = readGraph(input_graph);

    printf("--- Affichage de la liste d'adjacence ---\n");
    print_adjlist(&g);

    printf("\n--- Vérification Markov (tolérance 0.99-1.00) ---\n");
    verify_markov(&g, 0.99f, 1.00f);

    int output1 = writeMermaid(&g, out_graph_mmd);
    if (output1 == 0) {
        printf("Fichier Mermaid écrit: %s\n", out_graph_mmd);
        printf("Copiez-collez ce fichier dans https://www.mermaidchart.com/ pour visualiser le graphe.\n");
    } else if (output1 == -1) {
        fprintf(stderr, "Erreur ouverture fichier Mermaid\n");
    } else {
        fprintf(stderr, "Erreur écriture fichier Mermaid\n");
    }

    /* ===== Partie 2 (ajout) ===== */
    printf("\n========== PARTIE 2 ==========\n");

    // 1) Tarjan : calcul des classes (partition)
    t_partition part;
    tarjan_compute(&g, &part);

    printf("\n--- Composantes fortement connexes (Tarjan) ---\n");
    print_partition(&part);

    // 2) Hasse : liens entre classes
    t_link_array links = hasse_build_links(&part, &g);

    // (Option) : enlever les redondances transitives
    removeTransitiveLinks(&links);

    // 2bis) Export du diagramme de Hasse en Mermaid
    const char *out_hasse_mmd = "output/hasse_mermaid5.mmd";
    int hasse_ok = hasse_write_mermaid(&part, &links, out_hasse_mmd);
    if (hasse_ok == 0) {
        printf("Diagramme de Hasse Mermaid écrit: %s\n", out_hasse_mmd);
    } else {
        fprintf(stderr, "Erreur écriture du diagramme de Hasse Mermaid\n");
    }

    // 3) Caractéristiques (transitoire/persistante, absorbant, irréductible)
    print_characteristics(&part, &links);

    /* ===== Ménage ===== */
    hasse_free_links(&links);
    partition_free(&part);
    free_adjlist(&g);

    return 0;
}

