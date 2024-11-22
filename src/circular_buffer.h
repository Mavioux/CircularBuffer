/**
 * @file circular_buffer.h
 * @brief Circular buffer interface
 */

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    int* buffer;
    size_t head;
    size_t tail;
    size_t size;
    bool full;
} CircularBuffer;

// Initialize circular buffer
CircularBuffer* circular_buffer_init(size_t size);

// Add element to buffer
bool circular_buffer_add(CircularBuffer* cb, int element);

// Remove element from buffer
bool circular_buffer_remove(CircularBuffer* cb, int* element);

// Check if buffer is full
bool circular_buffer_is_full(const CircularBuffer* cb);

// Check if buffer is empty
bool circular_buffer_is_empty(const CircularBuffer* cb);

// Free buffer memory
void circular_buffer_free(CircularBuffer* cb);

#endif // CIRCULAR_BUFFER_H