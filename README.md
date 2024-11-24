# CircularBuffer
A Circular Buffer with Random Heart Rate Generator and Exponential Moving Average (EMA)

## Overview
This project implements a circular buffer data structure in C, used to store and process simulated heart rate data. The implementation includes a random heart rate generator and calculates the Exponential Moving Average (EMA) of the stored values.

## Design assumptions
The circular buffer will overwrite existing values if it is full, instead of waiting for the buffer to have empty space.
The reason I made this choice is because I thought it was more realistic for a realtime application to care about latest data.
I also added the bare minimum thread locking mechanisms to make the circular buffer thread safe.
In my example applications the buffer usually operates at its full capacity, since I went with a buffer that can overwrite itself.

## Building and Running

### Prerequisites
- GCC compiler
- Make build system
- Google Test framework (for unit tests)

### Compilation
```bash
# Build the main program and the producer_consumer program
make all

# Clean build artifacts
make clean

# Build and run tests
make test
```

### Running the Program
There are two executables that are being built. 

One called main which demonstrates a simple use of the circular buffer in a while loop.

The second called producer_consumer demonstrates the thread safety aspect of the circular buffer using a producer and consumer thread. The producer sleeps for only 1 second before producing and the consumer sleeps for 3s before calculating the EMA and removing the oldest entry in the buffer. It pretty much works in a similar fashion as the main program, it just does so using two threads. There is no reason for the consumer to remove anything from the buffer, since it can overwrite itself, I just added it for demonstrative purposes.

#### Main
Usage:
```bash
./main <buffer_size>
```
Example:
```bash
./main 10  # Creates a circular buffer of size 10
```

Output:
```bash
Starting heart rate monitoring...
Press Ctrl+C to exit

Current Heart Rate: 83 BPM | EMA: 83.00 BPM
Circular Buffer Visualization:
[T] 83 [H] 0 [ ] 0 [ ] 0 [ ] 0 [ ] 0 [ ] 0 [ ] 0 [ ] 0 [ ] 0 
Buffer Full: No
Head: 1, Tail: 0

Current Heart Rate: 52 BPM | EMA: 76.80 BPM
Circular Buffer Visualization:
[T] 83 [ ] 52 [H] 0 [ ] 0 [ ] 0 [ ] 0 [ ] 0 [ ] 0 [ ] 0 [ ] 0 
Buffer Full: No
Head: 2, Tail: 0

Current Heart Rate: 91 BPM | EMA: 79.64 BPM
Circular Buffer Visualization:
[T] 83 [ ] 52 [ ] 91 [H] 0 [ ] 0 [ ] 0 [ ] 0 [ ] 0 [ ] 0 [ ] 0 
Buffer Full: No
Head: 3, Tail: 0

Current Heart Rate: 135 BPM | EMA: 90.71 BPM
Circular Buffer Visualization:
[T] 83 [ ] 52 [ ] 91 [ ] 135 [H] 0 [ ] 0 [ ] 0 [ ] 0 [ ] 0 [ ] 0 
Buffer Full: No
Head: 4, Tail: 0
```

#### Producer Consumer
Usage:
```bash
./producer_consumer <buffer_size>
```
Example:
```bash
./producer_consumer 10  # Creates a circular buffer of size 10
```
Output:
```bash
Starting producer-consumer simulation...
Press Ctrl+C to exit

[Producer] Running producer
[Producer] Produced: 120 BPM
Circular Buffer Visualization:
[ ] 48 [ ] 109 [ ] 171 [ ] 52 [ ] 168 [ ] 155 [ ] 166 [ ] 122 [ ] 120 [H] 129 
Buffer Full: Yes
Head: 9, Tail: 9

[Consumer] Running consumer
[Consumer] EMA: 130.27 BPM
[Consumer] Consumed: 129 BPM
Circular Buffer Visualization:
[T] 48 [ ] 109 [ ] 171 [ ] 52 [ ] 168 [ ] 155 [ ] 166 [ ] 122 [ ] 120 [H] 129 
Buffer Full: No
Head: 9, Tail: 0
```

## Design Decisions
1. Buffer Implementation:
   - Used a struct-based approach for encapsulation
   - Implemented overflow handling by overwriting oldest data
   - Separate head and tail pointers for efficient operations
   - Mutex mechanism for thread safety

2. Memory Management:
   - Dynamic memory allocation for buffer creation
   - Proper cleanup in destructor to prevent memory leaks

3. Error Handling:
   - Null pointer checks throughout the implementation
   - Boundary checking for buffer operations

4. EMA Calculation:
   - Using standard exponential moving average formula
   - Alpha value of 0.2 for balanced smoothing

## Testing
The unit tests cover:
- Buffer initialization
- Adding elements
- Removing elements
- Removing elements non blocking
- Full/empty state checking
- EMA calculation

## License
This project is provided as-is under the MIT license.