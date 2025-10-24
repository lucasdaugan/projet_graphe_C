//
// Created by lucas on 24/10/2025.
//

#include <stdio.h>
#include "graph/graph.h"
#include "proba/proba.h"

int isMarkovGraph(const Graph *g) {
    int ok = 1;
    for (int i = 0; i < g->nbVertices; i++) {
        float sum = 0;
        for (Cell *c = g->adjLists[i]->head; c != NULL; c = c->next)
            sum += c->proba;
        if (sum < 0.99 || sum > 1.01) {
            printf("Sommet %d : somme = %.2f ❌\n", i + 1, sum);
            ok = 0;
        }
    }
    if (ok) printf("✅ Le graphe est un graphe de Markov\n");
    return ok;
}
