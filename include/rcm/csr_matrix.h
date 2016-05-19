
#ifndef RCM_CSR_MATRIX_H
#define RCM_CSR_MATRIX_H

#include <stddef.h>
#include <stdio.h>

struct csr_matrix
{
  size_t num_rows;
  size_t num_cols;
  size_t * offsets;
  size_t * columns;
  double * values;
};



struct csr_matrix make_csr_matrix(size_t num_rows,
                                  size_t num_cols,
                                  size_t num_entries,
                                  const size_t * entries);

void csr_matrix_free(struct csr_matrix * A);



size_t csr_matrix_num_nonzeros(const struct csr_matrix * A);

size_t csr_matrix_degree(const struct csr_matrix * A, size_t i);

const size_t * csr_matrix_neighbors(const struct csr_matrix * A, size_t i);



double csr_matrix_val(const struct csr_matrix * A, size_t i, size_t j);

double * csr_matrix_get(struct csr_matrix * A, size_t i, size_t j);



struct csr_matrix csr_matrix_permute(struct csr_matrix * A, const size_t * p);



void csr_matrix_mult_add(const struct csr_matrix * A,
                         const double * x,
                         double * y);

void csr_matrix_mult_add_trans(const struct csr_matrix * A,
                               const double * x,
                               double * y);



void csr_matrix_to_file(const struct csr_matrix * A, FILE * file);

struct csr_matrix csr_matrix_from_file(FILE * file);

#endif
