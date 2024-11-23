/**
 * @file circular_buffer.c
 * @brief Circular buffer implementation
 */

#include "circular_buffer.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

CircularBuffer* circular_buffer_init(size_t size) {
    CircularBuffer* cb = (CircularBuffer*)malloc(sizeof(CircularBuffer));
    if (!cb) {
        return NULL;
    }

    cb->buffer = (int*)malloc(size * sizeof(int));
    if (!cb->buffer) {
        free(cb);
        return NULL;
    }

    if (pthread_mutex_init(&cb->mutex, NULL) != 0) {
        free(cb->buffer);
        free(cb);
        return NULL;
    }

    if (pthread_cond_init(&cb->not_empty, NULL) != 0) {
        pthread_mutex_destroy(&cb->mutex);
        free(cb->buffer);
        free(cb);
        return NULL;
    }

    cb->head = 0;
    cb->tail = 0;
    cb->size = size;
    cb->full = false;
    cb->running = true;

    return cb;
}

bool circular_buffer_add(CircularBuffer* cb, int element) {
    if (!cb || !cb->running) {
        return false;
    }

    pthread_mutex_lock(&cb->mutex);

    // Always add the new element at head
    cb->buffer[cb->head] = element;
    
    if (cb->full) {
        // If buffer was full, move both head and tail
        cb->head = (cb->head + 1) % cb->size;
        cb->tail = (cb->tail + 1) % cb->size;
    } else {
        // If buffer wasn't full, just move head
        cb->head = (cb->head + 1) % cb->size;
        // Check if buffer is now full
        cb->full = (cb->head == cb->tail);
    }

    // Signal that new data is available
    pthread_cond_signal(&cb->not_empty);
    pthread_mutex_unlock(&cb->mutex);

    return true;
}

bool circular_buffer_remove(CircularBuffer* cb, int* element) {
    if (!cb || !element) {
        return false;
    }

    pthread_mutex_lock(&cb->mutex);

    while (circular_buffer_is_empty(cb) && cb->running) {
        pthread_cond_wait(&cb->not_empty, &cb->mutex);
    }

    if (!cb->running || circular_buffer_is_empty(cb)) {
        pthread_mutex_unlock(&cb->mutex);
        return false;
    }

    *element = cb->buffer[cb->tail];
    cb->full = false;
    cb->tail = (cb->tail + 1) % cb->size;

    pthread_mutex_unlock(&cb->mutex);
    return true;
}

bool circular_buffer_remove_nonblock(CircularBuffer* cb, int* element) {
    if (!cb || !element) {
        return false;
    }

    pthread_mutex_lock(&cb->mutex);

    if (circular_buffer_is_empty(cb) || !cb->running) {
        pthread_mutex_unlock(&cb->mutex);
        return false;
    }

    *element = cb->buffer[cb->tail];
    cb->full = false;
    cb->tail = (cb->tail + 1) % cb->size;

    pthread_mutex_unlock(&cb->mutex);
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

void circular_buffer_shutdown(CircularBuffer* cb) {
    if (!cb) {
        return;
    }

    pthread_mutex_lock(&cb->mutex);
    cb->running = false;
    pthread_cond_broadcast(&cb->not_empty);
    pthread_mutex_unlock(&cb->mutex);
}

void circular_buffer_free(CircularBuffer* cb) {
    if (cb) {
        circular_buffer_shutdown(cb);
        pthread_mutex_destroy(&cb->mutex);
        pthread_cond_destroy(&cb->not_empty);
        free(cb->buffer);
        free(cb);
    }
}

double calculate_ema(CircularBuffer* cb, double alpha) {
    if (!cb) {
        return 0.0;
    }

    pthread_mutex_lock(&cb->mutex);
    
    if (circular_buffer_is_empty(cb)) {
        pthread_mutex_unlock(&cb->mutex);
        return 0.0;
    }

    double ema = cb->buffer[cb->tail];
    size_t current = (cb->tail + 1) % cb->size;
    
    while (current != cb->head) {
        ema = alpha * cb->buffer[current] + (1 - alpha) * ema;
        current = (current + 1) % cb->size;
        if (current == cb->head) break;
    }

    pthread_mutex_unlock(&cb->mutex);
    return ema;
}

void visualizeCircularBuffer(CircularBuffer* cb) {
    if (!cb) {
        return;
    }

    pthread_mutex_lock(&cb->mutex);
    
    printf("Circular Buffer Visualization:\n");
    for (size_t i = 0; i < cb->size; ++i) {
        if (i == cb->head && i == cb->tail && !cb->full) {
            printf("[H,T] ");
        } else if (i == cb->head) {
            printf("[H] ");
        } else if (i == cb->tail) {
            printf("[T] ");
        } else {
            printf("[ ] ");
        }
        printf("%d ", cb->buffer[i]);
    }
    printf("\nBuffer Full: %s\n", cb->full ? "Yes" : "No");
    printf("Head: %zu, Tail: %zu\n\n", cb->head, cb->tail);

    pthread_mutex_unlock(&cb->mutex);
}
