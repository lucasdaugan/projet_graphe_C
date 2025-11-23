#ifndef PROJET_GRAPHE_C_TARJAN_H
#define PROJET_GRAPHE_C_TARJAN_H

#include "adj_list.h"

/* ======== Structures ======== */

// Informations par sommet pour l'algo de Tarjan
typedef struct {
    int id;       // numéro du sommet dans le graphe (0..n-1)
    int num;      // numéro de découverte (init -1)
    int numAccess;  // plus petit num accessible (init -1)
    int inP;  // indique si le sommet est dans la pile (0=non, 1=oui)
} t_tarjan_vertex;


// Une classe : nom + liste dynamique de sommets
typedef struct {
    char name[16]; // Nom de la classe "C1", "C2", ...
    int *verts;    // Tableau des sommets
    int  count;    // Nombre de sommets de la classe
    int  cap;      // Capacité du tableau
} t_classe;

// Partition = tableau dynamique de classes
typedef struct {
    t_classe *classes;
    int count;
    int cap;
} t_partition;

// Pile
typedef struct {
    int *data;    // tableau dynamique d'entiers
    int sommet;   // index du sommet de la pile (top)
    int cap; // capacité du tableau
} t_pile;

/* ======== Fonctions ======== */

// Alloue et initialise le tableau des états Tarjan
t_tarjan_vertex *tarjan_init_vertices(adjlist_t *G);

// Gestion de la pile
void pile_init(t_pile *p, int cap);
void pile_free(t_pile *p);
int  pile_vide(t_pile *p);
void pile_empiler(t_pile *p, int val);
int  pile_depiler(t_pile *p);
int  pile_sommet(t_pile *p);

// Classe
void classe_init(t_classe *c, char *name);
void classe_add_vertex(t_classe *c, int v1_based);
void classe_free(t_classe *c);

// Partition
void partition_init(t_partition *p);
int  partition_add_empty_class(t_partition *p, char *name); // retourne l'index
void partition_free(t_partition *p);

// Tarjan : calcule les composantes et remplit P
void tarjan_compute(adjlist_t *G, t_partition *P);

// DFS de Tarjan
void tarjan_dfs(int sommet, adjlist_t *G, t_tarjan_vertex *V, t_pile *S, int *compteur, t_partition *partition);

#endif //PROJET_GRAPHE_C_TARJAN_H