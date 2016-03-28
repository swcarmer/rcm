
#include <stdlib.h>

#include <rcm/cs_graph.h>
#include <rcm/queue.h>

cs_graph_t *
cs_graph_new(size_t num_vertices, size_t num_edges, const size_t * edges)
{
  size_t * counts = calloc(num_vertices, sizeof(size_t));
  for (size_t k = 0; k < num_edges; ++k) {
    const size_t i = edges[2*k];
    counts[i] += 1;
  }

  size_t * offsets = calloc(num_vertices + 1, sizeof(size_t));
  for (size_t i = 0; i < num_vertices; ++i) {
    offsets[i + 1] = offsets[i] + counts[i];
    counts[i] = 0;
  }

  size_t * vertices = calloc(num_edges, sizeof(size_t));
  for (size_t k = 0; k < num_edges; ++k) {
    const size_t i = edges[2*k], j = edges[2*k + 1];
    vertices[offsets[i] + counts[i]] = j;
    counts[i] += 1;
  }

  free(counts);

  cs_graph_t * g = malloc(sizeof(cs_graph_t));
  g->num_vertices = num_vertices;
  g->offsets = offsets;
  g->vertices = vertices;

  return g;
}


void cs_graph_free(cs_graph_t ** g_)
{
  cs_graph_t * g = *g_;
  if (!g) return;

  g->num_vertices = 0;
  free(g->offsets);
  free(g->vertices);
  free(g);

  *g_ = NULL;
}


bool cs_graph_connected(const cs_graph_t * g, size_t i, size_t j)
{
  for (size_t k = g->offsets[i]; k < g->offsets[i + 1]; ++k)
    if (g->vertices[k] == j) return true;

  return false;
}


size_t cs_graph_degree(const cs_graph_t * g, size_t i)
{
  return g->offsets[i + 1] - g->offsets[i];
}


size_t cs_graph_neighbor(const cs_graph_t * g, size_t i, size_t k)
{
  return g->vertices[g->offsets[i] + k];
}


size_t cs_graph_num_edges(const cs_graph_t * g)
{
  return g->offsets[g->num_vertices];
}


cs_graph_t * cs_graph_permute(const cs_graph_t * g, const size_t * p)
{
  size_t * counts = calloc(g->num_vertices, sizeof(size_t));
  for (size_t i = 0; i < g->num_vertices; ++i)
    counts[p[i]] = cs_graph_degree(g, i);

  size_t * offsets = calloc(g->num_vertices + 1, sizeof(size_t));
  for (size_t i = 0; i < g->num_vertices; ++i) {
    offsets[i + 1] = offsets[i] + counts[i];
    counts[i] = 0;
  }

  size_t * vertices = calloc(cs_graph_num_edges(g), sizeof(size_t));
  for (size_t i = 0; i < g->num_vertices; ++i)
    for (size_t k = g->offsets[i]; k < g->offsets[i + 1]; ++k) {
      const size_t j = g->vertices[k];
      vertices[offsets[p[i]] + counts[p[i]]] = p[j];
    }

  free(counts);

  cs_graph_t * h = malloc(sizeof(cs_graph_t));
  *h = (cs_graph_t){g->num_vertices, offsets, vertices};

  return h;
}


static
size_t max_degree_vertex(const cs_graph_t * g)
{
  size_t imax = 0;
  for (size_t i = 0; i < g->num_vertices; ++i)
    if (cs_graph_degree(g, i) > cs_graph_degree(g, imax))
      imax = i;

  return imax;
}


size_t * breadth_first_order(const cs_graph_t * g)
{
  bool * visited = calloc(g->num_vertices, sizeof(bool));
  size_t * p = calloc(g->num_vertices, sizeof(size_t));
  size_t num_visited = 0;

  queue_t * q = queue_new();
  queue_push(q, max_degree_vertex(g));

  while (queue_size(q) > 0) {
    const size_t i = queue_pop(q);
    visited[i] = true;

    p[num_visited] = i;
    num_visited += 1;

    for (size_t k = 0; k < cs_graph_degree(g, i); ++k) {
      const size_t j = cs_graph_neighbor(g, i, k);
      if (!visited[j]) queue_push(q, j);
    }
  }

  free(visited);
  queue_free(&q);

  return p;
}
