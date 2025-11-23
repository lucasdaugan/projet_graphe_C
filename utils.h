#ifndef __UTILS_H__
#define __UTILS_H__
#include "adj_list.h"
#include "tarjan.h"
#include "hasse.h"

void print_partition(t_partition *P);
int classe_est_transitoire(int idx_classe, t_link_array *L);
int degre_sortant(int idx_classe, t_link_array *L);
void print_characteristics(t_partition *P, t_link_array *L);
#endif