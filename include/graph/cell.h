//
// Created by lucas on 24/10/2025.
//

#ifndef GRAPH_CELL_H
#define GRAPH_CELL_H

typedef struct s_cell {
    int dest;              // sommet d’arrivée
    float proba;           // probabilité de transition
    struct s_cell *next;     // pointeur vers la cellule suivante
} t_cell;

// Crée une nouvelle cellule
t_cell *createCell(int dest, float proba);

#endif // GRAPH_CELL_H
