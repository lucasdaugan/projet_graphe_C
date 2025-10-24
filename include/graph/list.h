//
// Created by lucas on 24/10/2025.
//

#ifndef GRAPH_LIST_H
#define GRAPH_LIST_H

#include "graph/cell.h"

typedef struct s_list {
    t_cell *head;
} t_list;

// Crée une liste vide
t_list *createList(void);

// Ajoute une cellule en tête
void addCell(t_list *list, int dest, float proba);

// Affiche la liste
void printList(const t_list *list);

#endif // GRAPH_LIST_H
