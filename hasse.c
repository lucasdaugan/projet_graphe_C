#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hasse.h"

/* =========================
   fournie dans l'énoncé
   ========================= */

void removeTransitiveLinks(t_link_array *p_link_array)
{
    int i = 0;
    while (i < p_link_array->log_size)
    {
        t_link link1 = p_link_array->links[i];
        int j = 0;
        int to_remove = 0;
        while (j < p_link_array->log_size && !to_remove)
        {
            if (j != i)
            {
                t_link link2 = p_link_array->links[j];
                if (link1.from == link2.from)
                {
                    // look for a link from link2.to to link1.to
                    int k = 0;
                    while (k < p_link_array->log_size && !to_remove)
                    {
                        if (k != j && k != i)
                        {
                            t_link link3 = p_link_array->links[k];
                            if ((link3.from == link2.to) && (link3.to == link1.to))
                            {
                                to_remove = 1;
                            }
                        }
                        k++;
                    }
                }
            }
            j++;
        }
        if (to_remove)
        {
            // remove link1 by replacing it with the last link
            p_link_array->links[i] = p_link_array->links[p_link_array->log_size - 1];
            p_link_array->log_size--;
        }
        else
        {
            i++;
        }
    }
}

/* =========================
   Helpers internes
   ========================= */

static void links_init(t_link_array *A)
{
    A->links = NULL;
    A->log_size = 0;
    A->phys_size = 0;
}

static void links_reserve(t_link_array *A, int min_cap)
{
    if (A->phys_size >= min_cap) return;
    int new_cap = A->phys_size ? A->phys_size * 2 : 8;
    if (new_cap < min_cap) new_cap = min_cap;
    A->links = (t_link*)realloc(A->links, sizeof(t_link) * new_cap);
    if (!A->links) { perror("realloc"); exit(EXIT_FAILURE); }
    A->phys_size = new_cap;
}

static int links_contains(const t_link_array *A, int from, int to)
{
    for (int i = 0; i < A->log_size; ++i)
        if (A->links[i].from == from && A->links[i].to == to) return 1;
    return 0;
}

static void links_add_unique(t_link_array *A, int from, int to)
{
    if (from == to) return;                 // pas de boucle de classe sur elle-même
    if (links_contains(A, from, to)) return;
    links_reserve(A, A->log_size + 1);
    A->links[A->log_size].from = from;
    A->links[A->log_size].to   = to;
    A->log_size++;
}

/* =========================
   API publique
   ========================= */

t_link_array hasse_build_links(const t_partition *part, const adjlist_t *graph)
{
    t_link_array A; links_init(&A);

    /* Construire un tableau sommet(0..n-1) -> index_de_classe(0..part->count-1) */
    int n = graph->size;
    int *v2c = (int*)malloc(sizeof(int) * n);
    if (!v2c) { perror("malloc"); exit(EXIT_FAILURE); }

    for (int ci = 0; ci < part->count; ++ci) {
        const t_classe *C = &part->classes[ci];
        for (int k = 0; k < C->count; ++k) {
            int v1 = C->verts[k];  // 1-based
            int v0 = v1 - 1;       // 0-based
            v2c[v0] = ci;
        }
    }

    /* Pour chaque arête i -> j, si classe(i) != classe(j), ajouter lien Ci -> Cj */
    for (int i = 0; i < n; ++i) {
        int Ci = v2c[i];
        for (cell_t *cur = graph->lists[i].head; cur; cur = cur->next) {
            int j0 = cur->dest - 1;   // tes dest sont 1-based dans les cellules
            int Cj = v2c[j0];
            if (Ci != Cj) links_add_unique(&A, Ci, Cj);
        }
    }

    free(v2c);
    return A;
}

void hasse_free_links(t_link_array *A)
{
    if (!A) return;
    free(A->links);
    A->links = NULL;
    A->log_size = 0;
    A->phys_size = 0;
}
int hasse_write_mermaid(const t_partition *part, const t_link_array *links, const char *filename)
{
    FILE *f = fopen(filename, "wt");
    if (!f) return -1;

    /* En-tête Mermaid */
    fprintf(f, "---\nconfig:\n   layout: elk\n   theme: neo\n   look: neo\n---\n\nflowchart LR\n");

    /* 1) Déclarer les nœuds de classes C1, C2, ... avec label SANS accolades -> ex: "1,5,7" */
    for (int ci = 0; ci < part->count; ++ci) {
        const t_classe *C = &part->classes[ci];

        // Construire le label "1,5,7" (sans {})
        char label[512];
        int pos = 0;
        for (int k = 0; k < C->count; ++k) {
            pos += snprintf(label + pos, sizeof(label) - pos, "%d", C->verts[k]);
            if (k + 1 < C->count)
                pos += snprintf(label + pos, sizeof(label) - pos, ",");
        }

        // Nom de la classe
        char cname[32];
        if (C->name[0]) snprintf(cname, sizeof(cname), "%s", C->name);
        else snprintf(cname, sizeof(cname), "C%d", ci + 1);

        /* Recommandé: format rectangle + guillemets => robuste avec la virgule */
        fprintf(f, "%s[\"%s\"]\n", cname, label);
        // Variante arrondie possible: fprintf(f, "%s([\"%s\"])\n", cname, label);
    }

    /* 2) Écrire tous les liens Cx --> Cy (déjà construits par hasse_build_links) */
    for (int i = 0; i < links->log_size; ++i) {
        int from = links->links[i].from;
        int to   = links->links[i].to;

        char cname_from[32], cname_to[32];

        if (part->classes[from].name[0]) snprintf(cname_from, sizeof(cname_from), "%s", part->classes[from].name);
        else snprintf(cname_from, sizeof(cname_from), "C%d", from + 1);

        if (part->classes[to].name[0]) snprintf(cname_to, sizeof(cname_to), "%s", part->classes[to].name);
        else snprintf(cname_to, sizeof(cname_to), "C%d", to + 1);

        fprintf(f, "%s --> %s\n", cname_from, cname_to);
    }

    fclose(f);
    return 0;
}
