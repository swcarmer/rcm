
#ifndef RCM_QUEUE_H
#define RCM_QUEUE_H

#include <stddef.h>

typedef struct queue_t queue_t;

queue_t * queue_new(void);
void queue_free(queue_t ** q);
size_t queue_size(const queue_t * q);
size_t queue_front(const queue_t * q);
void queue_push(queue_t * q, size_t i);
size_t queue_pop(queue_t * q);

#endif
