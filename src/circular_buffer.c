/**
 * @file circular_buffer.c
 * @brief Circular buffer implementation
 */

#include "circular_buffer.h"

#include <stdlib.h>
#include <string.h>

CircularBuffer* circular_buffer_init(size_t size) {
    CircularBuffer* cb = malloc(sizeof(CircularBuffer));
    if (!cb) {
        return NULL;
    }

    cb->buffer = malloc(size * sizeof(int));
    if (!cb->buffer) {
        free(cb);
        return NULL;
    }

    cb->head = 0;
    cb->tail = 0;
    cb->size = size;
    cb->full = false;

    return cb;
}

bool circular_buffer_add(CircularBuffer* cb, int element) {
    if (!cb) {
        return false;
    }

    cb->buffer[cb->head] = element;
    
    if (cb->full) {
        cb->tail = (cb->tail + 1) % cb->size;
    }

    cb->head = (cb->head + 1) % cb->size;
    cb->full = cb->head == cb->tail;

    return true;
}

bool circular_buffer_remove(CircularBuffer* cb, int* element) {
    if (!cb || circular_buffer_is_empty(cb)) {
        return false;
    }

    *element = cb->buffer[cb->tail];
    cb->full = false;
    cb->tail = (cb->tail + 1) % cb->size;

    return true;
}

bool circular_buffer_is_full(const CircularBuffer* cb) {
    if (!cb) {
        return false;
    }

    return cb->full;
}

bool circular_buffer_is_empty(const CircularBuffer* cb) {
    if (!cb) {
        return false;
    }

    return !cb->full && (cb->head == cb->tail);
}

void circular_buffer_free(CircularBuffer* cb) {
    if (cb) {
        free(cb->buffer);
        free(cb);
    }
}

