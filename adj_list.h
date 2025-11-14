//
// Created by adrie on 06/11/2025.
//

#ifndef PROJET_GRAPHE_C_ADJ_LIST_H
#define PROJET_GRAPHE_C_ADJ_LIST_H

typedef struct AdjList {
    int n;
    List *lists;
} AdjList;

AdjList create_adjlist(int n);
void free_adjlist(AdjList *G);
void add_edge(AdjList *G, int depart, int arrivee, float prob);
void print_adjlist(const AdjList *G);
AdjList readGraph(const char *filename);
int verify_markov(const AdjList *G);

#endif //PROJET_GRAPHE_C_ADJ_LIST_H