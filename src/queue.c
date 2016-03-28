
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

typedef struct queue_t
{
  size_t size;
  size_t capacity;
  size_t front_index;
  size_t * entries;
} queue_t;


queue_t * queue_new(void)
{
  queue_t * q = malloc(sizeof(queue_t));
  q->size = 0;
  q->capacity = 4;
  q->front_index = 0;
  q->entries = calloc(q->capacity, sizeof(size_t));

  return q;
}

void queue_free(queue_t ** q_)
{
  queue_t * q = *q_;
  if (!q) return;

  q->size = 0;
  q->front_index = 0;
  free(q->entries);

  free(q);

  *q_ = NULL;
}


size_t queue_size(const queue_t * q)
{
  return q->size;
}


size_t queue_front(const queue_t * q)
{
  return q->entries[q->front_index];
}


void queue_push(queue_t * q, size_t i)
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


size_t queue_pop(queue_t * q)
{
  assert(queue_size(q) > 0);
  size_t i = queue_front(q);

  // TODO: reallocate the queue so that it takes up less room if its size is
  // less than some fraction of capacity

  q->front_index = (q->front_index + 1) % q->capacity;
  q->size -= 1;

  return i;
}
