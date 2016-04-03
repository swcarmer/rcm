
#include <stdlib.h>

#include <rcm/csr_matrix.h>

csr_matrix_t * csr_matrix_new(const cs_graph_t * g)
{
  csr_matrix_t * A = malloc(sizeof(csr_matrix_t));
  A->g = g;
  A->values = calloc(cs_graph_num_edges(g), sizeof(double));

  return A;
}

void csr_matrix_free(csr_matrix_t ** A_)
{
  csr_matrix_t * A = *A_;

  free(A->values);
  A->g = NULL;

  free(A);

  *A_ = NULL;
}

double csr_matrix_val(const csr_matrix_t * A, size_t i, size_t j)
{
  for (size_t k = A->g->offsets[i]; k < A->g->offsets[i + 1]; ++k)
    if (A->g->vertices[k] == j)
      return A->values[k];

  return 0.0;
}

double * csr_matrix_get(csr_matrix_t * A, size_t i, size_t j)
{
  for (size_t k = A->g->offsets[i]; k < A->g->offsets[i + 1]; ++k)
    if (A->g->vertices[k] == j)
      return A->values + k;

  return NULL;
}

void csr_matrix_mult(const csr_matrix_t * A, const double * x, double * y)
{
  for (size_t i = 0; i < A->g->num_vertices; ++i) {
    double z = 0.0;

    for (size_t k = A->g->offsets[i]; k < A->g->offsets[i + 1]; ++k) {
      const size_t j = A->g->vertices[k];
      z += A->values[k] * x[j];
    }

    y[i] = z;
  }
}
