#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
// représente une arrete avec sa destination ainsi que de sa probabilité.
typedef struct cell {
    int dest;
    float prob;
    struct cell *next;
} cell_t;

// represente toutes les arretes qui partent d'un sommet.
typedef struct list {
    cell_t *head;
} list_t;

//represente tout le graphe avec size qui correspond au nombre de sommet.
typedef struct adjlist {
    int size;
    list_t *lists;
} adjlist_t;
//Permet de créer une arrete qui va etre diriger vers un sommet avec une probabilité
cell_t *create_cell(int dest, float prob);
//Créer une list d'arrete vide
list_t create_list();
//libération de la memoire
void free_list(list_t *lst);
//créer un graphe en prenant n sommets.
adjlist_t create_adjlist(int n);
//libérer la mémoire pour tout les graphes.
void free_adjlist(adjlist_t *g);

//Permet d'ajouter une nouvelle arrete dans une liste.
void add_cell_to_list(list_t *lst, int dest, float prob);
//affiche toutes les arretes d'un sommet
void print_list(list_t *lst, int vertex);
//affiche le graphe entierement
void print_adjlist(adjlist_t *g);

// permet de lire et ainsi par la suite de créer le graphe
adjlist_t readGraph(const char *filename);
// Permet d'écrire le graphe dans un fichier au format mermaid.
int writeMermaid(adjlist_t *g,const char *filename);

// Permet de verifier si la somme des probabilités qui pointe vers un sommet sont egale a 1.
int verify_markov(adjlist_t *g, float tol_low, float tol_high);
//permet d'attribuer une lettre a un sommet.
char *getId(int i);

#endif