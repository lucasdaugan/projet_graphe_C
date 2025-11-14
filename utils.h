#ifndef __UTILS_H__
#define __UTILS_H__
#include "adj_list.h"

char *getId(int num);
void write_mermaid(const adjlist_t *G, const char *outfile);
#endif