/**
 * @file produce_consumer.c
 * @brief Producer-Consumer using Circular Buffer
 */

#include "circular_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MIN_HEART_RATE 44
#define MAX_HEART_RATE 185
#define EMA_ALPHA 0.2

// Generate a random heart rate value
int generate_heart_rate() {
    return MIN_HEART_RATE + rand() % (MAX_HEART_RATE - MIN_HEART_RATE + 1);
}

// Producer thread function
void* producer(void* arg) {
   
    CircularBuffer* cb = (CircularBuffer*)arg;

    while (1) {
        printf("[Consumer] Running producer\n");
        int heart_rate = generate_heart_rate();
        if (circular_buffer_add(cb, heart_rate)) {
            printf("[Producer] Produced: %d BPM\n", heart_rate);
        } else {
            printf("[Producer] Error adding heart rate.\n");
        }
        if (cb->size <= 20) {
            visualizeCircularBuffer(cb);
        }
        sleep(1);
    }

    return NULL;
}

// Consumer thread function
void* consumer(void* arg) {
    CircularBuffer* cb = (CircularBuffer*)arg;

    while (1) {
        printf("[Consumer] Running consumer\n");
        double ema = calculate_ema(cb, EMA_ALPHA);
        printf("[Consumer] EMA: %.2f BPM\n", ema);
        int heart_rate;
        if (circular_buffer_remove_nonblock(cb, &heart_rate)) {
            printf("[Consumer] Consumed: %d BPM\n", heart_rate);
        } else {
            printf("[Consumer] Buffer is empty. Waiting for data.\n");
        }
        if (cb->size <= 20) {
            visualizeCircularBuffer(cb);
        }
        sleep(3);
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <buffer_size>\n", argv[0]);
        return 1;
    }

    size_t buffer_size = atoi(argv[1]);
    if (buffer_size < 1) {
        printf("Buffer size must be positive\n");
        return 1;
    }

    srand(time(NULL));

    // Initialize circular buffer
    CircularBuffer* cb = circular_buffer_init(buffer_size);
    if (!cb) {
        printf("Failed to initialize circular buffer\n");
        return 1;
    }

    printf("Starting producer-consumer simulation...\n");
    printf("Press Ctrl+C to exit\n\n");

    pthread_t producer_thread, consumer_thread;

    // Create producer and consumer threads
    pthread_create(&consumer_thread, NULL, consumer, (void*)cb);
    pthread_create(&producer_thread, NULL, producer, (void*)cb);

    // Wait for threads to finish (they won't in this case)
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Cleanup
    circular_buffer_free(cb);
    return 0;
}
