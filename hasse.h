#ifndef __HASSE_H__
#define __HASSE_H__

#include "adj_list.h"   // Types : adjlist_t, cell_t
#include "tarjan.h"     // Types : t_partition, t_classe


/* ======== Structures ======== */

// Structure d'un lien entre deux classes */
typedef struct {
    int from; // index de la classe source
    int to;   // index de la classe destination
} t_link;

// Tableau dynamique de liens 
typedef struct {
    t_link *links;    // tableau de liens
    int log_size;     // nombre de liens utilisés
    int phys_size;    // capacité allouée
} t_link_array;


/* ======== Fonctions ======== */

// Supprime les liens transitifs dans un tableau 
void removeTransitiveLinks(t_link_array *p_link_array);

// Construit un tableau de liens entre classe
t_link_array hasse_build_links(t_partition *part, adjlist_t *graph);

// Libère la mémoire occupée par un t_link_array
void hasse_free_links(t_link_array *A);

// Exporte le diagramme de Hasse au format Mermaid
int hasse_write_mermaid(t_partition *part, t_link_array *links, const char *filename);


#endif // __HASSE_H__
