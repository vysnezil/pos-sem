#ifndef SLL_H
#define SLL_H

#include <pthread.h>

typedef struct sll_node {
    struct sll_node* next_;
    void* data_;
} sll_node;

typedef struct sll {
    sll_node* head_;
    sll_node* tail_;
    size_t size_;
    size_t dataSize_;
    pthread_mutex_t mutex_;
} sll;

void sll_init(sll* this, size_t dataSize);
void sll_clear(sll* this);
void sll_destroy(sll* this);
size_t sll_get_size(const sll* this);
_Bool sll_get(sll* this, size_t index, void* out);
_Bool sll_add(sll* this, void* data);
void sll_for_each(sll* this, void(*process_item)(void*));

_Bool sll_set(sll* this, size_t index, void* data);
_Bool sll_insert(sll* this, size_t index, void* data);
_Bool sll_remove(sll* this, size_t index);

#endif