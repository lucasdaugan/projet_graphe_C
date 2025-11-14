//
// Created by adrie on 06/11/2025.
//

#ifndef PROJET_GRAPHE_C_LIST_H
#define PROJET_GRAPHE_C_LIST_H

typedef struct Cell {
    int dest;
    float prob;
    struct Cell *next;
} Cell;

typedef struct List {
    Cell *head;
} List;

List create_empty_list(void);
Cell *create_cell(int dest, float prob);
void add_cell_to_list(List *L, Cell *c);
void print_list(const List *L);


#endif //PROJET_GRAPHE_C_LIST_H