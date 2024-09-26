#ifndef GAPFREETRACKER_H
#define GAPFREETRACKER_H

// Interface for tracking the highest gap-free transaction ID.
class GapFreeTracker {
public:
    // Mark a transaction as completed.
    virtual void transactionCompleted(long id) = 0;

    // Return the highest gap-free transaction ID.
    virtual long highestGapFree() = 0;

    // Virtual destructor for proper cleanup of derived classes.
    virtual ~GapFreeTracker() = default;
};

#endif // GAPFREETRACKER_H
