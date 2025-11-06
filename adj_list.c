//
// Created by adrie on 06/11/2025.
//

#include "adj_list.h"
#include <stdio.h>
#include <stdlib.h>

AdjList create_adjlist(int n) {
    AdjList G;
    G.n = n;
    G.lists = malloc(sizeof(List) * n);
    if (!G.lists) {
        perror("malloc lists");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; ++i)
        G.lists[i] = create_empty_list();
    return G;
}

void free_adjlist(AdjList *G) {
    for (int i = 0; i < G->n; ++i) {
        Cell *cur = G->lists[i].head;
        while (cur) {
            Cell *tmp = cur;
            cur = cur->next;
            free(tmp);
        }
    }
    free(G->lists);
}

void add_edge(AdjList *G, int depart, int arrivee, float prob) {
    Cell *c = create_cell(arrivee, prob);
    add_cell_to_list(&G->lists[depart - 1], c);
}

void print_adjlist(const AdjList *G) {
    for (int i = 0; i < G->n; ++i) {
        printf("Liste du sommet %d : ", i+1);
        print_list(&G->lists[i]);
    }
}

AdjList readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (!file) { perror("open file"); exit(EXIT_FAILURE); }

    int n, d, a;
    float p;
    fscanf(file, "%d", &n);
    AdjList G = create_adjlist(n);

    while (fscanf(file, "%d %d %f", &d, &a, &p) == 3)
        add_edge(&G, d, a, p);

    fclose(file);
    return G;
}

int verify_markov(const AdjList *G) {
    int ok = 1;
    for (int i = 0; i < G->n; ++i) {
        double sum = 0;
        for (Cell *c = G->lists[i].head; c; c=c->next)
            sum += c->prob;
        if (!(sum >= 0.99 && sum <= 1.01)) {
            printf("Sommet %d : somme = %.4f ❌\n", i+1, sum);
            ok = 0;
        }
    }
    if (ok) printf("✅ Graphe de Markov valide.\n");
    else    printf("❌ Graphe NON valide.\n");
    return ok;
}