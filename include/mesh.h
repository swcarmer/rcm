
#ifndef RCM_MESH_H
#define RCM_MESH_H

#include <stddef.h>

struct mesh
{
  size_t num_vertices;
  double * x;

  size_t num_triangles;
  size_t * triangles;
  int * adjacency;
};


void mesh_free(struct mesh * m);


const double * mesh_get_vertex(const struct mesh * m, size_t i);

const size_t * mesh_get_triangle(const struct mesh * m, size_t k);

const int * mesh_get_adjacent(const struct mesh * m, size_t k);


struct mesh mesh_from_file(const char * mesh_file_stem);

void mesh_to_file(const struct mesh * m, const char * mesh_file_stem);


#endif
