
#ifndef RCM_QUEUE_H
#define RCM_QUEUE_H

#include <stddef.h>

struct queue
{
  size_t size;
  size_t capacity;
  size_t front_index;
  size_t * entries;
};

struct queue make_queue(void);

void queue_free(struct queue * q);

size_t queue_size(const struct queue * q);

size_t queue_front(const struct queue * q);

void queue_push(struct queue * q, size_t i);

size_t queue_pop(struct queue * q);

#endif
