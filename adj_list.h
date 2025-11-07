#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

typedef struct cell {
    int dest;
    float prob;
    struct cell *next;
} cell_t;

typedef struct list {
    cell_t *head;
} list_t;

typedef struct adjlist {
    int size;
    list_t *lists;
} adjlist_t;

cell_t *create_cell(int dest, float prob);
list_t create_list();
void free_list(list_t *lst);
adjlist_t create_adjlist(int n);
void free_adjlist(adjlist_t *g);

void add_cell_to_list(list_t *lst, int dest, float prob);
void print_list(const list_t *lst, int vertex);
void print_adjlist(const adjlist_t *g);

adjlist_t readGraph(const char *filename);
int writeMermaid(const adjlist_t *g, const char *filename);

int verify_markov(const adjlist_t *g, float tol_low, float tol_high);

char *getId(int i);

#endif 