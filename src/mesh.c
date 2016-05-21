
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rcm/mesh.h>


void mesh_free(struct mesh * m)
{
  if (m->x)
    free(m->x);
  m->x = NULL;

  m->num_vertices = 0;

  if (m->triangles)
    free(m->triangles);
  m->triangles = NULL;

  if (m->adjacency)
    free(m->adjacency);
  m->adjacency = NULL;

  m->num_triangles = 0;
}


const double * mesh_get_vertex(const struct mesh * m, size_t i)
{
  assert(i < m->num_vertices);
  return m->x + 2*i;
}


const size_t * mesh_get_triangle(const struct mesh * m, size_t k)
{
  assert(k < m->num_triangles);
  return m->triangles + 3*k;
}


const int * mesh_get_adjacent(const struct mesh * m, size_t k)
{
  assert(k < m->num_triangles);
  return m->adjacency + 3*k;
}


/**
 * Mesh I/O routines
 */

static void write_node(const struct mesh * m, FILE * file);

static void write_ele(const struct mesh * m, FILE * file);

static void write_neigh(const struct mesh * m, FILE * file);

void mesh_to_file(const struct mesh * m, const char * mesh_file_stem)
{
  const size_t stem_length = strlen(mesh_file_stem);
  char * file_name = calloc(stem_length + strlen(".neigh") + 1, sizeof(char));

  strcpy(file_name, mesh_file_stem);
  strcat(file_name, ".node");
  FILE * node_file = fopen(file_name, "w");
  assert(node_file);
  write_node(m, node_file);
  fclose(node_file);

  strcpy(file_name, mesh_file_stem);
  strcat(file_name, ".ele");
  FILE * ele_file = fopen(file_name, "w");
  assert(ele_file);
  write_ele(m, ele_file);
  fclose(ele_file);

  strcpy(file_name, mesh_file_stem);
  strcat(file_name, ".neigh");
  FILE * neigh_file = fopen(file_name, "w");
  assert(neigh_file);
  write_neigh(m, neigh_file);
  fclose(neigh_file);

  free(file_name);
}


struct vertices
{
  size_t num_vertices;
  double * x;
};

static struct vertices read_node(FILE * file);


struct triangles
{
  size_t num_triangles;
  size_t * triangles;
};

struct adjacency
{
  size_t num_triangles;
  int * triangles;
};

static struct triangles read_ele(FILE * file);

static struct adjacency read_neigh(FILE * file);


struct mesh mesh_from_file(const char * mesh_file_stem)
{
  const size_t stem_length = strlen(mesh_file_stem);
  char * file_name = calloc(stem_length + strlen(".neigh") + 1, sizeof(char));

  strcpy(file_name, mesh_file_stem);
  strcat(file_name, ".node");
  FILE * node_file = fopen(file_name, "r");
  assert(node_file);
  struct vertices vs = read_node(node_file);
  fclose(node_file);

  strcpy(file_name, mesh_file_stem);
  strcat(file_name, ".ele");
  FILE * ele_file = fopen(file_name, "r");
  assert(ele_file);
  struct triangles tri = read_ele(ele_file);
  fclose(ele_file);

  strcpy(file_name, mesh_file_stem);
  strcat(file_name, ".neigh");
  FILE * neigh_file = fopen(file_name, "r");
  assert(neigh_file);
  struct adjacency adj = read_neigh(neigh_file);
  fclose(neigh_file);

  free(file_name);

  assert(tri.num_triangles == adj.num_triangles);

  struct mesh m = {.num_vertices = vs.num_vertices,
                   .x = vs.x,
                   .num_triangles = tri.num_triangles,
                   .triangles = tri.triangles,
                   .adjacency = adj.triangles};

  return m;
}


/**
 * Helper routines for mesh I/O
 */

void write_node(const struct mesh * m, FILE * file)
{
  fprintf(file, "%zu 2 0 1\n", m->num_vertices);

  for (size_t i = 0; i < m->num_vertices; ++i)
    fprintf(file, "%zu %lf %lf 0\n", i+1, m->x[2*i], m->x[2*i + 1]);
}

void write_ele(const struct mesh * m, FILE * file)
{
  fprintf(file, "%zu 3 0\n", m->num_triangles);

  for (size_t k = 0; k < m->num_triangles; ++k) {
    size_t * tri = m->triangles + 3*k;
    fprintf(file, "%zu %zu %zu %zu\n", k+1, tri[0]+1, tri[1]+1, tri[2]+1);
  }
}

void write_neigh(const struct mesh * m, FILE * file)
{
  fprintf(file, "%zu 0\n", m->num_triangles);

  for (size_t k = 0; k < m->num_triangles; ++k) {
    int * adj = m->adjacency + 3*k;
    for (size_t i = 0; i < 3; ++i) {
      int j = m->adjacency[3*k + i];
      adj[i] = (j >= 0) ? j + 1 : -1;
    }
    fprintf(file, "%zu %d %d %d\n", k+1, adj[0], adj[1], adj[2]);
  }
}

static struct vertices read_node(FILE * file)
{
  size_t num_vertices = 0, _ = 0, num_read = 0;
  num_read = fscanf(file, "%zu %zu %zu %zu\n", &num_vertices, &_, &_, &_);
  assert(num_read == 4);

  double * x = calloc(2 * num_vertices, sizeof(double));

  for (size_t i = 0; i < num_vertices; ++i) {
    num_read = fscanf(file, "%zu %lf %lf %zu", &_, x + 2*i, x + 2*i + 1, &_);
    assert(num_read == 4);
  }

  struct vertices vs = {.num_vertices = num_vertices,
                        .x = x};

  return vs;
}


static struct triangles read_ele(FILE * file)
{
  size_t num_triangles = 0, _ = 0, num_read = 0;

  num_read = fscanf(file, "%zu %zu %zu", &num_triangles, &_, &_);
  assert(num_read == 3);

  size_t * triangles = calloc(3 * num_triangles, sizeof(size_t));

  for (size_t k = 0; k < num_triangles; ++k) {
    size_t * tri = triangles + 3*k;
    num_read = fscanf(file, "%zu %zu %zu %zu", &_, tri, tri + 1, tri + 2);
    assert(num_read == 4);

    for (size_t i = 0; i < 3; ++i)
      triangles[3*k +i] -= 1;
  }

  struct triangles ts = {.num_triangles = num_triangles,
                         .triangles = triangles};

  return ts;
}


static struct adjacency read_neigh(FILE * file)
{
  size_t num_triangles = 0, _ = 0, num_read = 0;

  num_read = fscanf(file, "%zu %zu", &num_triangles, &_);
  assert(num_read == 2);

  int * adjacency = calloc(3 * num_triangles, sizeof(int));

  int tri[] = {0, 0, 0};
  for (size_t k = 0; k < num_triangles; ++k) {
    num_read = fscanf(file, "%zu %d %d %d", &_, tri, tri + 1, tri + 2);
    assert(num_read == 4);

    for (size_t i = 0; i < 3; ++i)
      adjacency[3*k + i] = (tri[i] > 0) ? tri[i] - 1 : -1;
  }

  struct adjacency ad = {.num_triangles = num_triangles,
                         .triangles = adjacency};

  return ad;
}
