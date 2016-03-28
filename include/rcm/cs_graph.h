
#ifndef RCM_CS_GRAPH_H
#define RCM_CS_GRAPH_H

#include <stddef.h>
#include <stdbool.h>

typedef struct cs_graph_s
{
  size_t num_vertices;
  size_t * offsets;
  size_t * vertices;
} cs_graph_t;

cs_graph_t *
cs_graph_new(size_t num_vertices, size_t num_edges, const size_t * edges);

void cs_graph_free(cs_graph_t ** g);

bool cs_graph_connected(const cs_graph_t * g, size_t i, size_t j);

size_t cs_graph_degree(const cs_graph_t * g, size_t i);

size_t cs_graph_neighbor(const cs_graph_t * g, size_t i, size_t k);

size_t cs_graph_num_edges(const cs_graph_t * g);

cs_graph_t * cs_graph_permute(const cs_graph_t * g, const size_t * p);

size_t * breadth_first_order(const cs_graph_t * g);

#endif
