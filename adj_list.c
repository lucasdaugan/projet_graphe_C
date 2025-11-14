//
// Created by adrie on 06/11/2025.
//

#include "adj_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

cell_t *create_cell(int dest, float prob) {
    cell_t *c = (cell_t*)malloc(sizeof(cell_t));
    if (c == NULL) return NULL;
    c->dest = dest;
    c->prob = prob;
    c->next = NULL;
    return c;
}

list_t create_list() {
    list_t l;
    l.head = NULL;
    return l;
}

void free_list(list_t *lst) {
    cell_t *cur = lst->head;
    while (cur) {
        cell_t *tmp = cur;
        cur = cur->next;
        free(tmp);
    }
    lst->head = NULL;
}

adjlist_t create_adjlist(int n) {
    adjlist_t g;
    g.size = n;
    g.lists = (list_t*)malloc(sizeof(list_t)*n);
    if (g.lists == NULL) {
        g.size = 0;
        return g;
    }
    for (int i=0;i<n;i++) g.lists[i] = create_list();
    return g;
}

void free_adjlist(adjlist_t *g) {
    if (g  == NULL || g->lists  == NULL) return;
    for (int i=0;i<g->size;i++) free_list(&g->lists[i]);
    free(g->lists);
    g->lists = NULL;
    g->size = 0;
}

void add_cell_to_list(list_t *lst, int dest, float prob) {
    cell_t *c = create_cell(dest, prob);
    if (c == NULL) return;
    c->next = lst->head; // insertion en tête
    lst->head = c;
}


void print_list(const list_t *lst, int vertex) {
    printf("Liste pour le sommet %d:[head]", vertex);
    cell_t *cur = lst->head;
    while (cur) {
        printf(" -> (%d, %.2f)", cur->dest, cur->prob);
        cur = cur->next;
    }
    printf("\n");
}

void print_adjlist(const adjlist_t *g) {
    if (g == NULL) return;
    for (int i=0;i<g->size;i++) {
        print_list(&g->lists[i], i+1);
    }
}

adjlist_t readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (file == NULL) {
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }
    int nbvert;
    if (fscanf(file, "%d", &nbvert) != 1) {
        perror("Could not read number of vertices");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    adjlist_t g = create_adjlist(nbvert);
    int depart, arrivee;
    float proba;
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        if (depart < 1 || depart > nbvert || arrivee < 1 || arrivee > nbvert) {
            fprintf(stderr, "%d->%d out of range\n", depart, arrivee);
            continue;
        }
        // stocker en utilisant l'index depart-1
        add_cell_to_list(&g.lists[depart-1], arrivee, proba);
    }
    fclose(file);
    return g;
}

int verify_markov(const adjlist_t *g, float tol_low, float tol_high) {
    int ok = 1;
    for (int i = 0; i < g->size; i++) {
        float sum = 0.0f;
        cell_t *cur = g->lists[i].head;
        while (cur) {
            sum += cur->prob;
            cur = cur->next;
        }
        if (sum < tol_low || sum > tol_high) {
            printf("la somme des probabilités du sommet %d est %.2f\n", i + 1, sum);
            ok = 0;
        }
    }
    if (ok) {
        printf("Le graphe est un graphe de Markov\n");
    } else {
        printf("Le graphe n'est pas un graphe de Markov\n");
    }
    return ok;
}


char *getId(int num) {
    if (num <= 0) return NULL;
    char buf[64];
    int idx = 0;
    int n = num;
    char tmp[64];
    int t = 0;
    while (n > 0) {
        n--;
        int r = n % 26;
        tmp[t++] = 'A' + r;
        n /= 26;
    }
    for (int i = 0; i < t; i++) buf[i] = tmp[t - 1 - i];
    buf[t] = '\0';
    return strdup(buf);
}

int writeMermaid(const adjlist_t *g, const char *filename) {
    FILE *f = fopen(filename, "wt");
    if (f == NULL) return -1;
    fprintf(f, "---\nconfig:\n   layout: elk\n   theme: neo\n   look: neo\n---\n\nflowchart LR\n");
    // nodes
    for (int i=0;i<g->size;i++) {
        char *id = getId(i+1);
        fprintf(f, "%s((%d))\n", id, i+1);
        free(id);
    }

    // edges: iterate lists; remember lists store edges in reverse-insertion order (insertion en tête)
    for (int i=0;i<g->size;i++) {
        cell_t *cur = g->lists[i].head;
        char *src = getId(i+1);
        while (cur) {
            char *dst = getId(cur->dest);
            fprintf(f, "%s -->|%.2f|%s\n", src, cur->prob, dst);
            free(dst);
            cur = cur->next;
        }
        free(src);
    }
    fclose(f);
    return 0;
}
