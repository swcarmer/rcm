
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <rcm/cs_graph.h>

int main(int argc, char ** argv)
{
  /* Make a graph */

  size_t num_vertices = 16;
  size_t * edges = calloc(2 * num_vertices, sizeof(size_t));

  for (size_t k = 0; k < num_vertices; ++k) {
    edges[2*k] = k;
    edges[2*k+1] = (k + 1) % num_vertices;
  }

  cs_graph_t * g = cs_graph_new(num_vertices, num_vertices, edges);
  free(edges);


  /* Test whether the basic graph operations work */

  assert(cs_graph_num_edges(g) == num_vertices);

  for (size_t i = 0; i < num_vertices; ++i)
    assert(cs_graph_degree(g, i) == 1);

  for (size_t i = 0; i < num_vertices; ++i)
    for (size_t j = 0; j < num_vertices; ++j)
      assert(cs_graph_connected(g, i, j) ==
             (j == (i + 1) % num_vertices));

  for (size_t i = 0; i < num_vertices; ++i)
    assert(cs_graph_neighbor(g, i, 0) == (i + 1) % num_vertices);


  /* Test whether reordering the graph works */

  size_t * p = breadth_first_order(g);

  bool * found = calloc(num_vertices, sizeof(size_t));

  for (size_t k = 0; k < num_vertices; ++k) {
    assert(!found[p[k]]);
    found[p[k]] = true;
  }

  free(found);

  cs_graph_t * h = cs_graph_permute(g, p);

  for (size_t i = 0; i < num_vertices; ++i)
    assert(cs_graph_degree(g, i) == 1);


  /* Cleanup */

  free(p);

  cs_graph_free(&g);
  assert(!g);

  cs_graph_free(&h);

  return 0;
}
