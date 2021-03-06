
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#include <rcm/queue.h>

struct queue make_queue(void)
{
  const size_t initial_capacity = 4;
  struct queue q = {.size = 0,
                    .capacity = initial_capacity,
                    .front_index = 0,
                    .entries = calloc(initial_capacity, sizeof(size_t))};

  return q;
}


void queue_free(struct queue * q)
{
  q->size = 0;
  q->front_index = 0;
  free(q->entries);
}


size_t queue_size(const struct queue * q)
{
  return q->size;
}


size_t queue_front(const struct queue * q)
{
  return q->entries[q->front_index];
}


void queue_push(struct queue * q, size_t i)
{
  // If the queue is at capacity, resize its internal storage
  if (q->size == q->capacity) {
    size_t * entries = calloc(2 * q->capacity, sizeof(size_t));
    for (size_t k = 0; k < q->size; ++k)
      entries[k] = q->entries[(q->front_index + k) % q->capacity];

    free(q->entries);
    q->entries = entries;
    q->capacity = 2 * q->capacity;
    q->front_index = 0;
  }

  q->entries[(q->front_index + q->size) % q->capacity] = i;
  q->size += 1;
}


size_t queue_pop(struct queue * q)
{
  assert(queue_size(q) > 0);
  size_t i = queue_front(q);

  // TODO: reallocate the queue so that it takes up less room if its size is
  // less than some fraction of capacity

  q->front_index = (q->front_index + 1) % q->capacity;
  q->size -= 1;

  return i;
}
