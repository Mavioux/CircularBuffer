/**
 * @file main.c
 * @brief Heart rate monitoring application
 */

#include "circular_buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MIN_HEART_RATE 44
#define MAX_HEART_RATE 185
#define EMA_ALPHA 0.2

int generate_heart_rate() {
    return MIN_HEART_RATE + rand() % (MAX_HEART_RATE - MIN_HEART_RATE + 1);
}

int main(int argc, char *argv[]) {
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

    CircularBuffer* cb = circular_buffer_init(buffer_size);
    if (!cb) {
        printf("Failed to initialize circular buffer\n");
        return 1;
    }    

    printf("Starting heart rate monitoring...\n");
    printf("Press Ctrl+C to exit\n\n");

    while (1) {
        int heart_rate = generate_heart_rate();
        circular_buffer_add(cb, heart_rate);
        
        double ema = calculate_ema(cb, EMA_ALPHA);
        
        printf("Current Heart Rate: %d BPM | EMA: %.2f BPM\n", heart_rate, ema);
        sleep(1);

        if (buffer_size <= 20) {
            visualizeCircularBuffer(cb);
        }
    }

    circular_buffer_free(cb);
    return 0;
}