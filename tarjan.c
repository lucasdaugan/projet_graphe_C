#include "tarjan.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ======== Tarjan vertices ======== */
t_tarjan_vertex *tarjan_init_vertices(const adjlist_t *G) {
    t_tarjan_vertex *V = (t_tarjan_vertex*)malloc(sizeof(*V) * G->size);
    if (!V) { perror("malloc"); exit(EXIT_FAILURE); }
    for (int i = 0; i < G->size; ++i) {
        V[i].id = i;
        V[i].num = -1;
        V[i].lowlink = -1;
        V[i].onStack = 0;
    }
    return V;
}

/* ======== Stack ======== */
void stack_init(t_stack *s, int capacity) {
    s->data = (int*)malloc(sizeof(int) * (capacity > 0 ? capacity : 8));
    if (!s->data) { perror("malloc"); exit(EXIT_FAILURE); }
    s->cap = (capacity > 0 ? capacity : 8);
    s->top = 0;
}
void stack_free(t_stack *s) {
    free(s->data); s->data = NULL; s->cap = 0; s->top = 0;
}
int  stack_empty(const t_stack *s) { return s->top == 0; }
void stack_push(t_stack *s, int v) {
    if (s->top == s->cap) {
        s->cap *= 2;
        s->data = (int*)realloc(s->data, sizeof(int) * s->cap);
        if (!s->data) { perror("realloc"); exit(EXIT_FAILURE); }
    }
    s->data[s->top++] = v;
}
int  stack_pop(t_stack *s) { return s->data[--s->top]; }
int  stack_peek(const t_stack *s) { return s->data[s->top - 1]; }

/* ======== Classe / Partition ======== */
void classe_init(t_classe *c, const char *name) {
    snprintf(c->name, sizeof(c->name), "%s", name ? name : "C?");
    c->verts = NULL; c->count = 0; c->cap = 0;
}
void classe_add_vertex(t_classe *c, int v1_based) {
    if (c->count == c->cap) {
        c->cap = c->cap ? c->cap * 2 : 4;
        c->verts = (int*)realloc(c->verts, sizeof(int) * c->cap);
        if (!c->verts) { perror("realloc"); exit(EXIT_FAILURE); }
    }
    c->verts[c->count++] = v1_based;
}
void classe_free(t_classe *c) {
    free(c->verts); c->verts = NULL; c->count = c->cap = 0;
}

void partition_init(t_partition *p) {
    p->classes = NULL; p->count = 0; p->cap = 0;
}
int partition_add_empty_class(t_partition *p, const char *name) {
    if (p->count == p->cap) {
        p->cap = p->cap ? p->cap * 2 : 4;
        p->classes = (t_classe*)realloc(p->classes, sizeof(t_classe) * p->cap);
        if (!p->classes) { perror("realloc"); exit(EXIT_FAILURE); }
    }
    classe_init(&p->classes[p->count], name);
    return p->count++;
}
void partition_free(t_partition *p) {
    for (int i = 0; i < p->count; ++i) classe_free(&p->classes[i]);
    free(p->classes); p->classes = NULL; p->count = p->cap = 0;
}

/* ======== Tarjan DFS ========
   Adapté à ton graphe :
   - index interne des sommets : 0..n-1
   - adjlist.lists[i] -> cell_t { dest (1-based), prob, next }
   - on convertit dest en 0-based à la volée.
*/
static void create_new_class_from_stack_root(int root,
                                             t_stack *S,
                                             t_tarjan_vertex *V,
                                             t_partition *P)
{
    // Créer "Ck" où k = P->count + 1
    char cname[16];
    snprintf(cname, sizeof(cname), "C%d", P->count + 1);
    int ci = partition_add_empty_class(P, cname);

    // Dépiler jusqu'à inclure 'root'
    while (1) {
        int w = stack_pop(S);
        V[w].onStack = 0;
        // stocker en 1-based pour affichage
        classe_add_vertex(&P->classes[ci], w + 1);
        if (w == root) break;
    }

    // (Optionnel) trier les sommets pour une sortie stable
    // Tri simple (insertion ou qsort)
    t_classe *C = &P->classes[ci];
    for (int i = 1; i < C->count; ++i) {
        int key = C->verts[i];
        int j = i - 1;
        while (j >= 0 && C->verts[j] > key) {
            C->verts[j+1] = C->verts[j];
            j--;
        }
        C->verts[j+1] = key;
    }
}

void tarjan_dfs(int v0,
                const adjlist_t *G,
                t_tarjan_vertex *V,
                t_stack *S,
                int *time_counter,
                t_partition *P)
{
    V[v0].num = V[v0].lowlink = (*time_counter)++;
    stack_push(S, v0);
    V[v0].onStack = 1;

    // Parcours des successeurs de v0
    for (cell_t *cur = G->lists[v0].head; cur; cur = cur->next) {
        int w = cur->dest - 1; // dest est 1-based -> 0-based
        if (V[w].num == -1) {
            // Successeur non visité : DFS
            tarjan_dfs(w, G, V, S, time_counter, P);
            if (V[w].lowlink < V[v0].lowlink) V[v0].lowlink = V[w].lowlink;
        } else if (V[w].onStack) {
            // Arc de retour dans la pile : mise à jour lowlink
            if (V[w].num < V[v0].lowlink) V[v0].lowlink = V[w].num;
        }
    }

    // Si v0 est racine d’une CFC
    if (V[v0].lowlink == V[v0].num) {
        create_new_class_from_stack_root(v0, S, V, P);
    }
}

/* ======== Lancement Tarjan ======== */
void tarjan_compute(const adjlist_t *G, t_partition *P) {
    partition_init(P);
    t_tarjan_vertex *V = tarjan_init_vertices(G);
    t_stack S; stack_init(&S, G->size);
    int time_counter = 0;

    for (int v = 0; v < G->size; ++v) {
        if (V[v].num == -1) {
            tarjan_dfs(v, G, V, &S, &time_counter, P);
        }
    }

    // Sécurité : la pile devrait être vide ici
    if (!stack_empty(&S)) {
        // En théorie, impossible ; on vide si besoin
        while (!stack_empty(&S)) (void)stack_pop(&S);
    }

    stack_free(&S);
    free(V);
}