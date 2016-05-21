
set(SRCS
  queue.c
  csr_matrix.c
  mesh.c
  )

add_library(rcm SHARED ${SRCS})

install(TARGETS rcm DESTINATION lib)
