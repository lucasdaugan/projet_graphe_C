#include <stdio.h>
#include "adj_list.h"
#include "adj_list.h"

int
main() {
    
    const char *input = "data/exemple_valid_step3.txt";
    const char *out = "output/graph_mermaid.txt";

    adjlist_t g = readGraph(input);

    printf("--- Affichage de la liste d'adjacence ---\n");
    print_adjlist(&g);


    printf("--- Vérification Markov (tolérance 0.99-1.00) ---\n");
    verify_markov(&g, 0.99f, 1.00f);
    int output1 = writeMermaid(&g, out);

    if (output1 == 0) {
    printf("Fichier Mermaid écrit: %s\n", out);
    printf("Copiez-collez ce fichier dans https://www.mermaidchart.com/ pour visualiser le graphe.\n");
    } else if (output1 == -1) {
        fprintf(stderr, "Erreur ouverture fichier Mermaid\n");
    }
    else {
    fprintf(stderr, "Erreur écriture fichier Mermaid\n");
    }

    free_adjlist(&g);
    return 1;
}