
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <rcm/mesh.h>

int main(int argc, char ** argv)
{
  size_t num_vertices = 4;
  double * x = calloc(2 * num_vertices, sizeof(double));
  x[0] = 0.0; x[1] = 0.0;
  x[2] = 1.0; x[3] = 0.0;
  x[4] = 1.0; x[5] = 1.0;
  x[6] = 0.0; x[7] = 1.0;

  size_t num_triangles = 2;
  size_t * triangles = calloc(3 * num_triangles, sizeof(size_t));
  int * adjacency = calloc(3 * num_triangles, sizeof(int));

  triangles[0] = 0; triangles[1] = 1; triangles[2] = 2;
  triangles[3] = 2; triangles[4] = 3; triangles[5] = 0;

  adjacency[0] = -1; adjacency[1] = 1; adjacency[2] = -1;
  adjacency[3] = -1; adjacency[4] = 0; adjacency[5] = -1;

  struct mesh m = {.num_vertices = num_vertices,
                   .x = x,
                   .num_triangles = num_triangles,
                   .triangles = triangles,
                   .adjacency = adjacency};

  const char mesh_file_stem[] = "mesh";
  mesh_to_file(&m, mesh_file_stem);

  mesh_free(&m);

  m = mesh_from_file(mesh_file_stem);

  assert(m.triangles[0] == 0 && m.triangles[1] == 1 && m.triangles[2] == 2);
  assert(m.triangles[3] == 2 && m.triangles[4] == 3 && m.triangles[5] == 0);

  assert(m.adjacency[0] == -1 && m.adjacency[1] == 1 && m.adjacency[2] == -1);
  assert(m.adjacency[3] == -1 && m.adjacency[4] == 0 && m.adjacency[5] == -1);

  mesh_free(&m);

  return 0;
}
