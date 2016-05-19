
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include <rcm/csr_matrix.h>

int main(int argc, char ** argv)
{
  size_t n = 4;
  size_t edges[] = {0, 1,
                    1, 2,
                    2, 3,
                    3, 0,
                    0, 2};

  struct csr_matrix A = make_csr_matrix(n, n, n + 1, edges);

  assert(csr_matrix_get(&A, 0, 2));

  size_t p[] = {1, 2, 3, 0};
  struct csr_matrix B = csr_matrix_permute(&A, p);

  assert(csr_matrix_get(&B, 1, 3));

  for (size_t i = 0; i < n; ++i)
    for (size_t j = 0; j < n; ++j)
      assert(fabs(csr_matrix_val(&A, i, j)
                  - csr_matrix_val(&B, p[i], p[j])) < 1.0e-15);

  csr_matrix_free(&A);
  csr_matrix_free(&B);

  return 0;
}
