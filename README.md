# Gap-Free Transaction Tracker

## Problem Overview

The **Gap-Free Transaction Tracker** is designed to handle billions of transactions concurrently. The task is to track the highest completed transaction ID, ensuring that every transaction before it has also been completed. Transactions start consecutively but can complete out of order, making it essential to track the highest gap-free transaction ID, where all preceding transactions have also finished.

### Key Problem Characteristics:
- **Consecutive Start, Out-of-Order Completion**: Transactions start in a sequential order but can complete in any order.
- **Concurrency**: Multiple threads (hundreds) may complete transactions concurrently, so the solution must efficiently handle concurrent updates.
- **Gap-Free Completion**: The goal is to identify the highest completed transaction where all previous transactions have been completed.

## Solution

This implementation uses an efficient combination of **atomic operations** and a **mutex** to ensure thread safety when accessing shared resources. The main components of the solution are:

- **Atomic Operations**: An `atomic<long>` is used to track the highest gap-free transaction ID to ensure thread-safe concurrent access.
- **Completion Tracker**: A vector of `atomic<bool>` values tracks which transactions have been completed. Each index in the vector corresponds to a transaction ID, and a `true` value at a given index signifies the completion of that transaction.
- **Mutex Lock**: A mutex protects the critical section when updating the highest gap-free transaction ID, ensuring that only one thread can update it at a time.

### Solution Walkthrough:

1. **Transaction Completion**:  
   The function `transactionCompleted(long id)` marks a transaction as complete and updates the status in a vector of atomic booleans. Then, a mutex ensures thread-safe updates to the highest gap-free transaction ID.

2. **Concurrency Handling**:  
   Atomic operations (`store`, `load`, `fetch_add`) ensure that marking transactions and checking their status is fast and thread-safe. A mutex is only used briefly when the highest gap-free transaction needs updating, reducing contention between threads.

3. **Performance**:  
   Using a vector of `atomic<bool>` ensures that marking and checking transactions are O(1) operations. The use of a mutex is minimized to prevent bottlenecks in a highly concurrent environment.

## Future Improvements

While the current implementation handles concurrency efficiently, there are potential improvements that could be considered:

### 1. **Dynamic Resizing of the Vector**:
The current solution assumes a fixed size for the vector (`completedTransactions`). For real-world applications handling billions of transactions, a dynamic resizing mechanism could be implemented to extend the vector when needed.

### 2. **Lock-Free Updates to Highest Gap-Free Transaction**:
The use of a mutex ensures thread safety but introduces some contention. A future improvement could involve exploring **lock-free algorithms** (e.g., a Compare-And-Swap approach) to update the highest gap-free transaction without the need for a mutex, further improving concurrency performance.

### 3. **Persistent Storage for Crash Recovery**:
In a real-world system, ensuring the gap-free tracker’s state is persisted would be essential for crash recovery. Implementing a method to persist the tracker state to disk could be a worthwhile future enhancement.

## Installation and Testing

### Prerequisites
To compile and run this project, ensure that you have the following dependencies installed:
- **C++17 or higher** (C++20 recommended)
- **CMake** (Version 3.29 or higher)
- **Google Test** (Automatically fetched by CMake)

### Steps to Build and Run Tests

1. **Clone the repository**:
   ```bash
   git clone <repository-url>
   cd <repository-directory>
   ```

2. **Configure the project with CMake**:
   In the project directory, create a build folder and configure the project:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

3. **Build the project**:
   ```bash
   make
   ```

4. **Run the tests**:
   After building, run the tests using CTest (CMake's testing tool):
   ```bash
   ctest
   ```

   Alternatively, you can run the tests directly by executing the test binary:
   ```bash
   ./neo4j_gtest
   ```

### Performance Test (500 Threads)
We have included a performance test to simulate concurrent transaction completions using 500 threads. This test validates the thread safety and performance of the `GapFreeTrackerImpl` class. It can be executed as part of the test suite.

## Alternative Approaches

### 1. **Lock-Free Data Structures**:
A more advanced solution could involve lock-free queues or other lock-free data structures to reduce mutex contention and further improve throughput.

### 2. **Hierarchical Locking**:
Instead of a single global lock, a more sophisticated approach could involve hierarchical locking or dividing the transaction range into smaller buckets, each with its own lock. This would reduce contention and improve scalability in systems with a massive number of transactions.
