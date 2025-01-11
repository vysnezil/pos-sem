#include "syn_buffer.h"
#include "stdlib.h"
#include "string.h"

void syn_buffer_init(syn_buffer* this, int capacity, int el_size) {
    this->capacity = capacity;
    this->el_size = el_size;
    this->data = calloc(this->capacity, el_size);
    this->in = 0;
    this->out = 0;
    this->size = 0;
    pthread_mutex_init(&this->mutex, NULL);
    pthread_cond_init(&this->produce, NULL);
    pthread_cond_init(&this->consume, NULL);
}

void syn_buffer_add(syn_buffer* this, void* data) {
    pthread_mutex_lock(&this->mutex);
    while (this->size == this->capacity)
        pthread_cond_wait(&this->produce, &this->mutex);
    memcpy(this->data + (this->in++ * this->el_size), data, this->el_size);
    this->size++;
    this->in %= this->capacity;
    pthread_mutex_unlock(&this->mutex);
    pthread_cond_broadcast(&this->consume);
}

void syn_buffer_get(syn_buffer* this, void* data) {
    pthread_mutex_lock(&this->mutex);
    while (this->size == 0)
        pthread_cond_wait(&this->consume, &this->mutex);
    memcpy(data, this->data + (this->out++ * this->el_size), this->el_size);
    this->size--;
    this->out %= this->capacity;
    pthread_mutex_unlock(&this->mutex);
    pthread_cond_broadcast(&this->produce);
}

void syn_buffer_free(syn_buffer* this) {
    free(this->data);
    pthread_mutex_destroy(&this->mutex);
    pthread_cond_destroy(&this->produce);
    pthread_cond_destroy(&this->consume);
}