
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <rcm/csr_matrix.h>

int main(int argc, char ** argv)
{
  /* Make a graph */
  size_t num_vertices = 16;
  size_t * edges = calloc(6 * num_vertices, sizeof(size_t));

  for (size_t k = 0; k < num_vertices; ++k) {
    edges[6*k  ] = k;
    edges[6*k+1] = (k + 1) % num_vertices;
    edges[6*k+2] = (k + 1) % num_vertices;
    edges[6*k+3] = k;
    edges[6*k+4] = k;
    edges[6*k+5] = k;
  }

  struct csr_matrix A =
    make_csr_matrix(num_vertices, num_vertices, 3*num_vertices, edges);

  free(edges);

  for (size_t i = 0; i < num_vertices; ++i) {
    const size_t j = (i + 1) % num_vertices;

    double * aij = csr_matrix_get(&A, i, j);
    assert(aij);
    *aij = -1.0;

    double * aji = csr_matrix_get(&A, j, i);
    assert(aji);
    *aji = -1.0;

    double * aii = csr_matrix_get(&A, i, i);
    assert(aii);
    *aii = 2.0;
  }

  for (size_t i = 0; i < num_vertices; ++i) {
    assert(fabs(csr_matrix_val(&A, i, i) - 2.0) < 1.0e-16);
    assert(fabs(csr_matrix_val(&A, i, (i + 1) % num_vertices) + 1.0) < 1.0e-16);
  }

  double * x = calloc(num_vertices, sizeof(double));
  double * y = calloc(num_vertices, sizeof(double));

  for (size_t i = 0; i < num_vertices; ++i) {
    x[i] = 1.0;
    y[i] = 0.0;
  }

  csr_matrix_mult_add(&A, x, y);

  for (size_t i = 0; i < num_vertices; ++i)
    assert(fabs(y[i]) < 1.0e-16);

  csr_matrix_free(&A);
  assert(A.num_rows == 0 && A.num_cols == 0);
  assert(!(A.offsets || A.columns || A.values));

  free(x);
  free(y);

  return 0;
}
