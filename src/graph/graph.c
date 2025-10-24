//
// Created by lucas on 24/10/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include "graph/graph.h"


graph *createGraph(int sommets) {
    graph *g = malloc(sizeof(graph));
    if (!g) {
        printf("Erreur malloc Graph");
        exit(EXIT_FAILURE);
    }
    g->nbsommets = sommets;
    g->adjlist = malloc(sommets * sizeof(t_list *));
    if (!g->nbsommets) {
        perror("Erreur malloc adjLists");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < sommets; i++) {
        g->adjlist[i] = createList();
    }
    return g;
}

void addEdge(graph *g, int from, int to, float proba) {
    addCell(g->adjlist[from - 1], to, proba); // indices 1..N
}

void printGraph(const graph *g) {
    for (int i = 0; i < g->nbsommets; i++) {
        printf("Liste du sommet %d: ", i + 1);
        printList(g->adjlist[i]);
    }
}

graph *readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (!file) {
        perror("Impossible d’ouvrir le fichier");
        exit(EXIT_FAILURE);
    }

    int nbVert, from, to;
    float proba;

    if (fscanf(file, "%d", &nbVert) != 1) {
        perror("Erreur lecture nb sommets");
        exit(EXIT_FAILURE);
    }

    graph *g = createGraph(nbVert);

    while (fscanf(file, "%d %d %f", &from, &to, &proba) == 3) {
        addEdge(g, from, to, proba);
    }

    fclose(file);
    return g;
}

void freeGraph(graph *g) {
    for (int i = 0; i < g->nbsommets; i++) {
        t_cell *curr = g->adjlist[i]->head;
        while (curr) {
            t_cell *tmp = curr;
            curr = curr->next;
            free(tmp);
        }
        free(g->adjlist[i]);
    }
    free(g->adjlist);
    free(g);
}
