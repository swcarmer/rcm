
#include <assert.h>
#include <stdlib.h>

#include <rcm/csr_matrix.h>

struct csr_matrix make_csr_matrix(size_t num_rows,
                                  size_t num_cols,
                                  size_t num_entries,
                                  const size_t * entries)
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


size_t csr_matrix_num_nonzeros(const struct csr_matrix * A)
{
  return A->offsets[A->num_rows];
}


size_t csr_matrix_degree(const struct csr_matrix * A, size_t i)
{
  return A->offsets[i + 1] - A->offsets[i];
}


const size_t * csr_matrix_neighbors(const struct csr_matrix * A, size_t i)
{
  return A->columns + A->offsets[i];
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


struct csr_matrix csr_matrix_permute(struct csr_matrix * A, const size_t * p)
{
  size_t * counts = calloc(A->num_rows, sizeof(size_t));
  for (size_t i = 0; i < A->num_rows; ++i)
    counts[p[i]] = A->offsets[i + 1] - A->offsets[i];

  size_t * offsets = calloc(A->num_rows + 1, sizeof(size_t));
  for (size_t i = 0; i < A->num_rows; ++i)
    offsets[i + 1] = offsets[i] + counts[i];

  const size_t nnz = csr_matrix_num_nonzeros(A);
  size_t * columns = calloc(nnz, sizeof(size_t));
  double * values = calloc(nnz, sizeof(double));

  for (size_t i = 0; i < A->num_rows; ++i) {
    for (size_t k = A->offsets[i]; k < A->offsets[i + 1]; ++k) {
      const size_t j = A->columns[k];

      counts[p[i]] -= 1;
      columns[offsets[p[i]] + counts[p[i]]] = p[j];
      values[offsets[p[i]] + counts[p[i]]] = A->values[k];
    }
  }

  free(counts);

  struct csr_matrix B = {.num_rows = A->num_rows,
                         .num_cols = A->num_cols,
                         .offsets = offsets,
                         .columns = columns,
                         .values = values};

  return B;
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


void csr_matrix_to_file(const struct csr_matrix * A, FILE * file)
{
  fprintf(file, "%zu %zu %zu\n",
          A->num_rows, A->num_cols, csr_matrix_num_nonzeros(A));

  for (size_t i = 0; i < A->num_rows; ++i) {
    fprintf(file, "%zu, %zu: ", i, csr_matrix_degree(A, i));

    for (size_t k = A->offsets[i]; k < A->offsets[i + 1]; ++k)
      fprintf(file, "%zu %lf ", A->columns[k], A->values[k]);

    fprintf(file, "\n");
  }
}


struct csr_matrix csr_matrix_from_file(FILE * file)
{
  size_t num_rows = 0, num_cols = 0, nnz = 0, num_read = 0;

  num_read = fscanf(file, "%zu %zu %zu\n", &num_rows, &num_cols, &nnz);
  assert(num_read == 3);
  num_read = 0;

  size_t * offsets = calloc(num_rows + 1, sizeof(size_t));
  size_t * columns = calloc(nnz, sizeof(size_t));
  double * values = calloc(nnz, sizeof(double));

  for (size_t i = 0; i < num_rows; ++i) {
    size_t degree = 0, j = 0;

    num_read = fscanf(file, "%zu, %zu: ", &j, &degree);
    assert(num_read == 2);
    assert(j == i);

    offsets[i + 1] = offsets[i] + degree;

    for (size_t k = 0; k < degree; ++k) {
      double z = 0.0;

      num_read = fscanf(file, "%zu %lf ", &j, &z);
      assert(num_read == 2);

      columns[offsets[i] + k] = j;
      values[offsets[i] + k] = z;
    }

    fscanf(file, "\n");
  }

  struct csr_matrix A = {.num_rows = num_rows,
                         .num_cols = num_cols,
                         .offsets = offsets,
                         .columns = columns,
                         .values = values};

  return A;
}
