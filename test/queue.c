
#include <assert.h>

#include <rcm/queue.h>

int main(int argc, char ** argv)
{
  queue_t * q = queue_new();

  assert(queue_size(q) == 0);

  queue_push(q, 86);

  assert(queue_size(q) == 1);
  assert(queue_front(q) == 86);

  const size_t k = queue_pop(q);
  assert(k == 86);
  assert(queue_size(q) == 0);

  for (size_t i = 0; i < 52; ++i)
    queue_push(q, i);

  assert(queue_size(q) == 52);

  queue_free(&q);
  assert(!q);

  return 0;
}
