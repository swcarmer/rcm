
set(SRCS
  cs_graph.c
  queue.c
  csr_matrix.c
  )

add_library(rcm SHARED ${SRCS})

install(TARGETS rcm DESTINATION lib)
