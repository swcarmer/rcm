
#include <assert.h>
#include <math.h>

#include <rcm/csr_matrix.h>

int main(int argc, char ** argv)
{

  const size_t n = 4;
  const size_t nnz = 5;
  const size_t edges[] = {0, 1,
                          1, 2,
                          2, 3,
                          3, 0,
                          0, 2};

  struct csr_matrix A = make_csr_matrix(n, n, nnz, edges);
  for (size_t i = 0; i < n; ++i) {
    const size_t * js = csr_matrix_neighbors(&A, i);
    for (size_t k = 0; k < csr_matrix_degree(&A, i); ++k) {
      const size_t j = js[k];
      double * a = csr_matrix_get(&A, i, j);
      *a = n * i + j;
    }
  }

  FILE * output_file = fopen("matrix.txt", "w");
  assert(output_file);
  csr_matrix_to_file(&A, output_file);

  fclose(output_file);

  FILE * input_file = fopen("matrix.txt", "r");
  assert(input_file);
  struct csr_matrix B = csr_matrix_from_file(input_file);

  for (size_t i = 0; i < n; ++i) {
    assert(csr_matrix_degree(&A, i) == csr_matrix_degree(&B, i));

    for (size_t j = 0; j < n; ++j)
      assert(fabs(csr_matrix_val(&A, i, j) - csr_matrix_val(&B, i, j)) < 1.e-16);
  }

  return 0;
}
