#ifndef MATRIX_H
#define MATRIX_H

#include "adj_list.h" 
#include "tarjan.h"

typedef struct {
    int size;
    double **data;  
} t_matrix;

t_matrix matrix_create_zero(int n);
t_matrix matrix_from_adjlist(adjlist_t *g);
void matrix_copy(t_matrix *dest, t_matrix *src);
t_matrix matrix_multiply(t_matrix *A, t_matrix *B);
double matrix_diff(t_matrix *A, t_matrix *B);
void matrix_print(t_matrix *m);
t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index);
void matrix_free(t_matrix *m);

#endif