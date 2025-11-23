#include "tarjan.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ======== Tarjan vertices ======== */
t_tarjan_vertex *tarjan_init_vertices(adjlist_t *G) {
    t_tarjan_vertex *V = (t_tarjan_vertex*)malloc(sizeof(t_tarjan_vertex) * G->size);
    if (V == NULL) {
        return NULL;
    }
    for (int i = 0; i < G->size; ++i) {
        V[i].id = i;
        V[i].num = -1;        
        V[i].numAccess = -1;  
        V[i].inP = 0;         
    }
    return V;
}
/* ======== Pile ======== */

void pile_init(t_pile *p, int cap) {
    p->data = (int*)malloc(sizeof(int) * (cap > 0 ? cap : 8));
    if (p->data == NULL) {
        p->cap = 0;
        p->sommet = 0;
        return;
    }
    p->cap = (cap > 0 ? cap : 8);
    p->sommet = 0;
}

void pile_free(t_pile *p) {
    free(p->data);
    p->data = NULL;
    p->cap = 0;
    p->sommet = 0;
}

int pile_vide(t_pile *p) {
    return (p->sommet == 0);
}

void pile_empiler(t_pile *p, int val) {
    if (p->sommet == p->cap) {
        int nouvelle_cap = p->cap * 2;
        int *nouveau_data = (int*)realloc(p->data, sizeof(int) * nouvelle_cap);
        if (nouveau_data == NULL) {
            return;
        }
        p->data = nouveau_data;
        p->cap = nouvelle_cap;
    }
    p->data[p->sommet++] = val;
}

int pile_depiler(t_pile *p) {
    if (pile_vide(p)) return -1;
    return p->data[--p->sommet];
}

int pile_sommet(t_pile *p) {
    if (pile_vide(p)) return -1;
    return p->data[p->sommet - 1];
}


/* ======== Classe / Partition ======== */
void classe_init(t_classe *c, char *name) {
    if (name == NULL) {
        snprintf(c->name, sizeof(c->name), "C?");
    } else {
        snprintf(c->name, sizeof(c->name), "%s", name);
    }
    c->verts = NULL;
    c->count = 0;
    c->cap = 0;
}

void classe_add_vertex(t_classe *c, int v1_based) {
    if (c->count == c->cap) {
        int nouvelle_cap = c->cap ? c->cap * 2 : 4;
        int *nouveau_verts = (int*)realloc(c->verts, sizeof(int) * nouvelle_cap);
        if (nouveau_verts == NULL) {
            return;
        }
        c->verts = nouveau_verts;
        c->cap = nouvelle_cap;
    }
    c->verts[c->count++] = v1_based;
}

void classe_free(t_classe *c) {
    free(c->verts);
    c->verts = NULL;
    c->count = 0;
    c->cap = 0;
}

void partition_init(t_partition *p) {
    p->classes = NULL;
    p->count = 0;
    p->cap = 0;
}

int partition_add_empty_class(t_partition *p, char *name) {
    if (p->count == p->cap) {
        int nouvelle_cap = p->cap ? p->cap * 2 : 4;
        t_classe *nouveau_classes = (t_classe*)realloc(p->classes, sizeof(t_classe) * nouvelle_cap);
        if (nouveau_classes == NULL) {
            return -1;
        }
        p->classes = nouveau_classes;
        p->cap = nouvelle_cap;
    }
    classe_init(&p->classes[p->count], name);
    return p->count++;
}

void partition_free(t_partition *p) {
    for (int i = 0; i < p->count; ++i) {
        classe_free(&p->classes[i]);
    }
    free(p->classes);
    p->classes = NULL;
    p->count = 0;
    p->cap = 0;
}

/* ======== Tarjan DFS ======== */

// Extrait une nouvelle composante à partir du sommet racine
void extraire_composante(int racine, t_pile *p, t_tarjan_vertex *V, t_partition *partition)
{
    // Construire le nom de la classe "Ck" avec k = nombre actuel de classes + 1
    char nom_classe[16];
    snprintf(nom_classe, sizeof(nom_classe), "C%d", partition->count + 1);

    int idx_classe = partition_add_empty_class(partition, nom_classe);

    // Dépiler jusqu’à inclure la racine
    while (1) {
        int sommet = pile_depiler(p);
        V[sommet].inP = 0;

        // Ajouter sommet à la classe 
        classe_add_vertex(&partition->classes[idx_classe], sommet + 1);
        if (sommet == racine) break;
    }
}


void tarjan_dfs(int sommet, adjlist_t *G, t_tarjan_vertex *V, t_pile *S, int *compteur, t_partition *partition)
{
    V[sommet].num = *compteur;
    V[sommet].numAccess = *compteur;
    (*compteur)++;

    pile_empiler(S, sommet);
    V[sommet].inP = 1;

    // Parcours des successeurs
    cell_t *courant = G->lists[sommet].head;
    while (courant != NULL) {
        int suivant = courant->dest - 1;

        if (V[suivant].num == -1) {
            // Sommet non visité
            tarjan_dfs(suivant, G, V, S, compteur, partition);
            if (V[suivant].numAccess < V[sommet].numAccess)
                V[sommet].numAccess = V[suivant].numAccess;
        }
        else if (V[suivant].inP) {
            // Sommet dans la pile
            if (V[suivant].num < V[sommet].numAccess)
                V[sommet].numAccess = V[suivant].num;
        }
        courant = courant->next;
    }

    if (V[sommet].numAccess == V[sommet].num) {
        extraire_composante(sommet, S, V, partition);
    }
}


/* ======== Lancement Tarjan ======== */
void tarjan_compute(adjlist_t *G, t_partition *P) {
    partition_init(P);
    t_tarjan_vertex *V = tarjan_init_vertices(G);
    if (V == NULL) {
        printf("Erreur : impossible d'allouer de la mémoire pour les sommets.\n");
        return;
    }

    t_pile S;
    pile_init(&S, G->size);

    int compteur_temps = 0;

    for (int i = 0; i < G->size; i++) {
        if (V[i].num == -1) {
            tarjan_dfs(i, G, V, &S, &compteur_temps, P);
        }
    }

    while (!pile_vide(&S)) {pile_depiler(&S);}
    pile_free(&S);
    free(V);
}