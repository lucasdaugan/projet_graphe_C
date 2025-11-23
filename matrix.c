#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include "matrix.h"
#include "tarjan.h"

t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index){
    t_classe *C = &part.classes[compo_index];
    int new_size = C->count;

    t_matrix sub_mat = matrix_create_zero(new_size);

    int i_new; int j_new;

    for (i_new = 0; i_new < new_size; i_new++){
        int v_num_i = C->verts[i_new];
        int i_old = v_num_i - 1; 

        for (j_new = 0; j_new < new_size; j_new++){
            int v_num_j = C->verts[j_new];
            int j_old = v_num_j - 1;
            double valeur_proba = matrix.data[i_old][j_old];
            sub_mat.data[i_new][j_new] = valeur_proba;
        }
    }

    return sub_mat;
}

t_matrix matrix_create_zero(int n){
    t_matrix mat;
    mat.size = n;

    mat.data = (double**)malloc(sizeof(double*) * n);

    int i; int j;

    for (i = 0; i < n; i++) {
        mat.data[i] = (double*)malloc(sizeof(double) * n);
        for (j = 0; j < n; j++){mat.data[i][j] = 0.0;}
    }
    return mat;
}

void matrix_free(t_matrix *m) {
    int i;
    for (i = 0; i < m->size; i++) {free(m->data[i]);}
    free(m->data);
}

t_matrix matrix_from_adjlist(adjlist_t *g){
    t_matrix mat = matrix_create_zero(g->size);
    int i;
    for (i = 0; i < g->size; i++) {
        cell_t *curr = g->lists[i].head;
        
        while (curr != NULL) {
            int arrivee_index = curr->dest - 1;
            
            mat.data[i][arrivee_index] = curr->prob;

            curr = curr->next;
        }
    }
    return mat;
}

void matrix_copy(t_matrix *dest, t_matrix *src)
{
    int i; int j;

    for (i = 0; i < src->size; i++){
        for (j = 0; j < src->size; j++){
            dest->data[i][j] = src->data[i][j];
        }
    }
}

t_matrix matrix_multiply(t_matrix *A, t_matrix *B){
    int n = A->size;
    t_matrix result = matrix_create_zero(n);

    int i, j, k;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            double sum = 0.0;
            for (k = 0; k < n; k++) {sum = sum + (A->data[i][k] * B->data[k][j]);}
            result.data[i][j] = sum;
        }
    }
    return result;
}

double matrix_diff(t_matrix *A, t_matrix *B) {
    double total_diff = 0.0;
    int i, j;
    int n = A->size;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            double diff = A->data[i][j] - B->data[i][j];
            
            // On ajoute la valeur absolue (fabs) à la somme totale
            total_diff = total_diff + fabs(diff);
        }
    }

    return total_diff;
}

void matrix_print(t_matrix *m) {
    int i, j;
    for (i = 0; i < m->size; i++){
        printf("| ");
        for (j = 0; j < m->size; j++) {printf("%.2f ", m->data[i][j]);}
        printf("|\n");
    }
}