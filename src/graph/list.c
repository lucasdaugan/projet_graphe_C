//
// Created by lucas on 24/10/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include "graph/list.h"

t_list *createList(void) {
    t_list *list = (t_list *)malloc(sizeof(t_list));
    if (!list) {
        printf("Erreur malloc list");
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    return list;
}

void addCell(t_list *list, int dest, float proba) {
    t_cell *newCell = createCell(dest, proba);
    newCell->next = list->head;
    list->head = newCell;
}

void printList(const t_list *list) {
    t_cell *curr = list->head;
    while (curr) {
        printf("(%d, %.2f) -> ", curr->dest, curr->proba);
        curr = curr->next;
    }
    printf("NULL\n");
}
