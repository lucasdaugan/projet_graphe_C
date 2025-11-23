#ifndef MATRIX_H
#define MATRIX_H

#include "adj_list.h" 
#include "tarjan.h"

typedef struct {
    int size;
    double **data;  
} t_matrix;

t_matrix matrix_create_zero(int n); // Crée une matrice carrée NxN et initialise ses cases à 0
t_matrix matrix_from_adjlist(adjlist_t *g); // Convertit la liste d'adjacence d'un graphe en une matrice de transition
void matrix_copy(t_matrix *dest, t_matrix *src); // Copie toutes les valeurs d'une matrice source vers une matrice destination
t_matrix matrix_multiply(t_matrix *A, t_matrix *B); // Calcule le produit de deux matrices carrées A × B et retourne la matrice résultat
double matrix_diff(t_matrix *A, t_matrix *B); // Calcule la "différence" entre deux matrices.
void matrix_print(t_matrix *m); // Affiche la matrice
t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index); // Extrait une petite matrice ne gardant que les lignes et colonnes des sommets appartenant à une seule classe donnée
void matrix_free(t_matrix *m); // Libère la mémoire allouée pour la matrice.

#endif