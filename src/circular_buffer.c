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

double calculate_ema(const CircularBuffer* cb, double alpha) {
    if (!cb || circular_buffer_is_empty(cb)) {
        return 0.0;
    }

    double ema = cb->buffer[cb->tail];
    size_t current = (cb->tail + 1) % cb->size;

    while (current != cb->head) {
        ema = alpha * cb->buffer[current] + (1 - alpha) * ema;
        current = (current + 1) % cb->size;
        if (current == cb->head) break;
    }

    return ema;
}

// Function to visualize the circular buffer
void visualizeCircularBuffer(const CircularBuffer* cb) {
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
}

