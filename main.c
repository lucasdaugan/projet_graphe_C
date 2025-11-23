#include <stdio.h>
#include <stdlib.h>
#include "adj_list.h"
#include "utils.h"
#include "tarjan.h"
#include "hasse.h"
#include "matrix.h"
#include <windows.h>

int main(void)
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    /* ===== Partie 1 (inchangée) ===== */
    const char *input_graph = "data/exemple_valid_step3.txt";
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

    
    /* ===== PARTIE 3 (Matrice) ===== */
    printf("\n========== PARTIE 3 : MATRICES ==========\n");
    
    const char *input_graph_mat = "data/exemple_meteo.txt";

    adjlist_t gm = readGraph(input_graph_mat);
    
    t_matrix M = matrix_from_adjlist(&gm);
    
    printf("Matrice M :\n");
    matrix_print(&M);
    
    // 2. Calcul de M^2
    t_matrix M2 = matrix_multiply(&M, &M);
    
    printf("\nMatrice M^2 (Apres 2 etapes) :\n");
    matrix_print(&M2);
    
    // 2. Calcul de M^2
    t_matrix M3 = matrix_multiply(&M2, &M);
    
    printf("\nMatrice M^3 (Apres 3 etapes) :\n");
    matrix_print(&M3);
    
    // 3. Recherche de M^N
    printf("\n--- Recherche de M^n ---\n");
    
    t_matrix M_actuelle = matrix_create_zero(M.size);
    matrix_copy(&M_actuelle, &M); // On commence avec M
    
    t_matrix M_suivante;
    
    double diff = 1.0; // On initialise diff à 1
    double epsilon = 0.01; // Le seuil de tolérance demandé
    int k = 1;
    int max_iterations = 999; // Sécurité au cas où il y est une boucle infinie
    
    // Tant que la différence est grande, on continue
    while (diff > epsilon && k < max_iterations){
        M_suivante = matrix_multiply(&M_actuelle, &M);
        diff = matrix_diff(&M_actuelle, &M_suivante);
        
        printf("Etape %d -> diff = %f\n", k, diff);
        
        matrix_free(&M_actuelle);
        M_actuelle = M_suivante;
        k++;
    }
    
    if (k >= max_iterations) {printf("Non atteint apres %d iterations\n", max_iterations);}
    else {
        printf("\nAtteint a la puissance k = %d\n", k);
        printf("Difference finale : %f\n", diff);
        printf("\nM^n approximative :\n");
        matrix_print(&M_actuelle);
    }
    
    matrix_free(&M);
    matrix_free(&M2);
    matrix_free(&M_actuelle);

    return 0;
}

