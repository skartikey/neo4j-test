#ifndef GAPFREETRACKERIMPL_H
#define GAPFREETRACKERIMPL_H

#include "GapFreeTracker.h"
#include <vector>
#include <atomic>
#include <mutex>

// Implementation of GapFreeTracker to manage transaction completion.
class GapFreeTrackerImpl final : public GapFreeTracker {
private:
    std::atomic<long> highestGapFreeTransaction;  // Atomic to track the highest gap-free transaction ID.
    std::vector<std::atomic<bool>> completedTransactions;  // Vector to mark which transactions are completed.
    std::mutex lock;  // Mutex to protect updates to the highestGapFreeTransaction.

public:
    // Constructor initializing the highest gap-free transaction and transaction completion vector.
    GapFreeTrackerImpl();

    // Override the base class method to mark a transaction as completed.
    void transactionCompleted(long id) override;

    // Override the base class method to get the current highest gap-free transaction ID.
    long highestGapFree() override;
};

#endif // GAPFREETRACKERIMPL_H
