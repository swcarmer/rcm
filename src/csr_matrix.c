
#include <stdlib.h>

#include <rcm/csr_matrix.h>

struct csr_matrix make_csr_matrix(size_t num_rows,
                                  size_t num_cols,
                                  size_t num_entries,
                                  size_t * entries)
{
  size_t * counts = calloc(num_rows, sizeof(size_t));

  for (size_t k = 0; k < num_entries; ++k) {
    const size_t i = entries[2*k];
    counts[i] += 1;
  }

  size_t * offsets = calloc(num_rows + 1, sizeof(size_t));
  size_t * columns = calloc(num_entries, sizeof(size_t));

  for (size_t i = 0; i < num_rows; ++i)
    offsets[i + 1] = offsets[i] + counts[i];

  for (size_t k = 0; k < num_entries; ++k) {
    const size_t i = entries[2*k];
    const size_t j = entries[2*k + 1];

    counts[i] -= 1;
    columns[offsets[i] + counts[i]] = j;
  }

  free(counts);

  double * values = calloc(num_entries, sizeof(double));

  struct csr_matrix A = {.num_rows = num_rows,
                         .num_cols = num_cols,
                         .offsets = offsets,
                         .columns = columns,
                         .values = values};

  return A;
}


void csr_matrix_free(struct csr_matrix * A)
{
  if (A->offsets) free(A->offsets);
  if (A->columns) free(A->columns);
  if (A->values) free(A->values);

  A->num_rows = 0;
  A->num_cols = 0;
  A->offsets = NULL;
  A->columns = NULL;
  A->values = NULL;
}


double csr_matrix_val(const struct csr_matrix * A, size_t i, size_t j)
{
  for (size_t k = A->offsets[i]; k < A->offsets[i + 1]; ++k)
    if (A->columns[k] == j)
      return A->values[k];

  return 0.0;
}


double * csr_matrix_get(struct csr_matrix * A, size_t i, size_t j)
{
  for (size_t k = A->offsets[i]; k < A->offsets[i + 1]; ++k)
    if (A->columns[k] == j)
      return A->values + k;

  return NULL;
}


void csr_matrix_mult_add(const struct csr_matrix * A,
                         const double * x,
                         double * y)
{
  for (size_t i = 0; i < A->num_rows; ++i) {
    double z = 0.0;

    for (size_t k = A->offsets[i]; k < A->offsets[i + 1]; ++k) {
      const size_t j = A->columns[k];
      z += A->values[k] * x[j];
    }

    y[i] += z;
  }
}


void csr_matrix_mult_add_trans(const struct csr_matrix * A,
                               const double * x,
                               double * y)
{
  for (size_t i = 0; i < A->num_rows; ++i) {
    const double z = x[i];

    for (size_t k = A->offsets[i]; k < A->offsets[i + 1]; ++k) {
      const size_t j = A->columns[k];
      y[j] += A->values[k] * z;
    }
  }
}
