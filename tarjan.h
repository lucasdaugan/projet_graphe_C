//
// Created by adrie on 17/11/2025.
//

#ifndef PROJET_GRAPHE_C_TARJAN_H
#define PROJET_GRAPHE_C_TARJAN_H

#ifndef TARJAN_H
#define TARJAN_H

#include "adj_list.h"

/* ======== Structures ======== */

// Infos par sommet pour l'algo de Tarjan
typedef struct {
    int id;       // identifiant interne (index 0..n-1)
    int num;      // numéro de découverte (init -1)
    int lowlink;  // plus petit num accessible (init -1)
    int onStack;  // booléen 0/1
} t_tarjan_vertex;

// Pile simple d'indices de sommets (0..n-1)
typedef struct {
    int *data;
    int top;   // prochain emplacement libre
    int cap;   // capacité
} t_stack;

// Une classe (CFC) : nom + liste dynamique de sommets (1-based pour affichage facile)
typedef struct {
    char name[16]; // "C1", "C2", ...
    int *verts;    // tableau des sommets en notation 1-based (ex: 3, 7, 5)
    int  count;
    int  cap;
} t_classe;

// Partition = tableau dynamique de classes
typedef struct {
    t_classe *classes;
    int count;
    int cap;
} t_partition;

/* ======== API Tarjan ======== */

// Alloue et initialise le tableau des états Tarjan
t_tarjan_vertex *tarjan_init_vertices(const adjlist_t *G);

// Pile
void stack_init(t_stack *s, int capacity);
void stack_free(t_stack *s);
int  stack_empty(const t_stack *s);
void stack_push(t_stack *s, int v);
int  stack_pop(t_stack *s);
int  stack_peek(const t_stack *s);

// Classe / Partition
void classe_init(t_classe *c, const char *name);
void classe_add_vertex(t_classe *c, int v1_based);
void classe_free(t_classe *c);

void partition_init(t_partition *p);
int  partition_add_empty_class(t_partition *p, const char *name); // retourne l'index
void partition_free(t_partition *p);

// Tarjan : calcule les CFC et remplit P
void tarjan_compute(const adjlist_t *G, t_partition *P);

// (interne) DFS de Tarjan
void tarjan_dfs(int v0,
                const adjlist_t *G,
                t_tarjan_vertex *V,
                t_stack *S,
                int *time_counter,
                t_partition *P);

#endif // TARJAN_H
#endif //PROJET_GRAPHE_C_TARJAN_H