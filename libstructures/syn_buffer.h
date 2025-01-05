#ifndef SYN_BUFFER_H
#define SYN_BUFFER_H

#include <pthread.h>

typedef struct syn_buffer {
  int size, capacity;
  int in, out;
  int el_size;
  void* data;
  pthread_mutex_t mutex;
  pthread_cond_t produce;
  pthread_cond_t consume;
} syn_buffer;

void syn_buffer_init(syn_buffer* this, int capacity, int el_size);
void syn_buffer_add(syn_buffer* this, void* data);
void syn_buffer_get(syn_buffer* this, void* data);
void syn_buffer_free(syn_buffer* this);

#endif
