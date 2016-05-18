
#ifndef RCM_CSR_MATRIX_H
#define RCM_CSR_MATRIX_H


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
                                  size_t * entries);

void csr_matrix_free(struct csr_matrix * A);



size_t csr_matrix_num_nonzeros(const struct csr_matrix * A);

size_t csr_matrix_degree(const struct csr_matrix * A, size_t i);

size_t csr_matrix_neighbor(const struct csr_matrix * A, size_t i, size_t k);



double csr_matrix_val(const struct csr_matrix * A, size_t i, size_t j);

double * csr_matrix_get(struct csr_matrix * A, size_t i, size_t j);



void csr_matrix_mult_add(const struct csr_matrix * A,
                         const double * x,
                         double * y);

void csr_matrix_mult_add_trans(const struct csr_matrix * A,
                               const double * x,
                               double * y);

#endif
