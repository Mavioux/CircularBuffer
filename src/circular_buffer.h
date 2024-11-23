/**
 * @file circular_buffer.h
 * @brief Circular buffer interface
 */

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    int* buffer;
    size_t head;
    size_t tail;
    size_t size;
    bool full;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;  // Only need not_empty now since we always accept new data
    bool running;
} CircularBuffer;

// Initialize circular buffer
CircularBuffer* circular_buffer_init(size_t size);

// Add element to buffer
bool circular_buffer_add(CircularBuffer* cb, int element);

// Remove element from buffer (blocking)
bool circular_buffer_remove(CircularBuffer* cb, int* element);

// Remove element from buffer (non-blocking)
bool circular_buffer_remove_nonblock(CircularBuffer* cb, int* element);

// Check if buffer is full
bool circular_buffer_is_full(const CircularBuffer* cb);

// Check if buffer is empty
bool circular_buffer_is_empty(const CircularBuffer* cb);

// Free buffer memory
void circular_buffer_free(CircularBuffer* cb);

// Calculate EMA
double calculate_ema(CircularBuffer* cb, double alpha);

// Function to visualize the circular buffer
void visualizeCircularBuffer(CircularBuffer* cb);

// Shutdown the circular buffer
void circular_buffer_shutdown(CircularBuffer* cb);

#ifdef __cplusplus
}
#endif

#endif // CIRCULAR_BUFFER_H