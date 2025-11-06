//
// Created by adrie on 06/11/2025.
//

#include "list.h"
#include <stdio.h>
#include <stdlib.h>

List create_empty_list(void) {
    List L;
    L.head = NULL;
    return L;
}

Cell *create_cell(int dest, float prob) {
    Cell *c = malloc(sizeof(Cell));
    if (!c) {
        perror("malloc cell");
        exit(EXIT_FAILURE);
    }
    c->dest = dest;
    c->prob = prob;
    c->next = NULL;
    return c;
}

void add_cell_to_list(List *L, Cell *c) {
    c->next = L->head;
    L->head = c;
}

void print_list(const List *L) {
    const Cell *cur = L->head;
    printf("[head] ");
    while (cur) {
        printf("-> (%d, %.2f) ", cur->dest, cur->prob);
        cur = cur->next;
    }
    printf("\n");
}