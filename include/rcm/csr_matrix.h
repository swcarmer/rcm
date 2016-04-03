
#ifndef RCM_CSR_MATRIX_H
#define RCM_CSR_MATRIX_H

#include <rcm/cs_graph.h>

typedef struct csr_matrix_s
{
  const cs_graph_t * g;
  double * values;
} csr_matrix_t;

csr_matrix_t * csr_matrix_new(const cs_graph_t * g);

void csr_matrix_free(csr_matrix_t ** A);

double csr_matrix_val(const csr_matrix_t * A, size_t i, size_t j);

double * csr_matrix_get(csr_matrix_t * A, size_t i, size_t j);

void csr_matrix_mult(const csr_matrix_t * A, const double * x, double * y);

#endif
