#include "GapFreeTrackerImpl.h"

// Constructor initializing the highest gap-free transaction to 0.
// Initializing completedTransactions with a large size, assuming a bounded transaction space.
GapFreeTrackerImpl::GapFreeTrackerImpl() : highestGapFreeTransaction(0), completedTransactions(1000000) {
    for (auto& completed : completedTransactions) {
        completed.store(false); // Initialize all transactions as incomplete.
    }
}

void GapFreeTrackerImpl::transactionCompleted(const long id) {
    if (id <= 0) return;  // Ignore invalid transaction IDs.

    if (id >= completedTransactions.size()) {
        // Optionally handle out-of-bounds transactions here.
        // A dynamic resizing approach could be implemented, if required.
        throw std::out_of_range("Transaction ID out of bounds.");
    }

    // Atomically mark the transaction as completed.
    completedTransactions[id].store(true, std::memory_order_release);

    // Lock to safely update highestGapFreeTransaction.
    std::lock_guard<std::mutex> guard(lock);

    // Check and update the highest gap-free transaction ID.
    while (completedTransactions[highestGapFreeTransaction.load() + 1].load(std::memory_order_acquire)) {
        highestGapFreeTransaction.fetch_add(1, std::memory_order_relaxed);  // Increment atomically.
    }
}

long GapFreeTrackerImpl::highestGapFree() {
    // Return the current value of highest gap-free transaction ID.
    return highestGapFreeTransaction.load(std::memory_order_acquire);
}
