//
// Created by lucas on 24/10/2025.
//
#include <stdlib.h>
#include <stdio.h>
#include "graph/cell.h"


t_cell *createCell(int dest, float proba) {
    t_cell *newCell = (t_cell *)malloc(sizeof(t_cell));
    if (!newCell) {
        perror("Erreur malloc cell");
        exit(EXIT_FAILURE);
    }
    newCell->dest = dest;
    newCell->proba = proba;
    newCell->next = NULL;
    return newCell;
}
