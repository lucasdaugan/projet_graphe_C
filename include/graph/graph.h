//
// Created by lucas on 24/10/2025.
//

#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include "graph/list.h"

typedef struct {
    int nbsommets;
    t_list *adjlist; // tableau de listes
} graph;

// Crée un graphe vide
graph *createGraph(int sommets);

// Ajoute une arête
void addEdge(graph *g, int from, int to, float proba);

// Affiche la liste d’adjacence complète
void printGraph(const graph *g);

// Lit un graphe à partir d’un fichier texte
graph *readGraph(const char *filename);

// Libère la mémoire
void freeGraph(graph *g);

#endif // GRAPH_GRAPH_H
