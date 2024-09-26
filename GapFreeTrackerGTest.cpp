#include "GapFreeTrackerImpl.h"
#include <gtest/gtest.h>

// Test case for simple transaction completion
TEST(GapFreeTrackerTest, BasicTransactionCompletion) {
    GapFreeTracker* t = new GapFreeTrackerImpl();

    t->transactionCompleted(1);
    EXPECT_EQ(t->highestGapFree(), 1);

    t->transactionCompleted(4);
    t->transactionCompleted(2);
    EXPECT_EQ(t->highestGapFree(), 2);

    t->transactionCompleted(3);
    EXPECT_EQ(t->highestGapFree(), 4);

    t->transactionCompleted(5);
    t->transactionCompleted(9);
    t->transactionCompleted(8);
    EXPECT_EQ(t->highestGapFree(), 5);

    t->transactionCompleted(7);
    t->transactionCompleted(6);
    EXPECT_EQ(t->highestGapFree(), 9);

    delete t;
}

// Test case to check concurrent transaction completion
TEST(GapFreeTrackerTest, ConcurrentTransactionCompletion) {
    GapFreeTracker* t = new GapFreeTrackerImpl();

    // Lambda function for completing transactions in a thread
    auto completeTransactions = [&](const std::vector<long>& transactions) {
        for (const long id : transactions) {
            t->transactionCompleted(id);
        }
    };

    // Create threads to mark transactions as completed
    std::thread t1(completeTransactions, std::vector<long>{1, 3, 5, 7, 9});
    std::thread t2(completeTransactions, std::vector<long>{2, 4, 6, 8, 10});

    // Join the threads to ensure they complete before we check the result
    t1.join();
    t2.join();

    // After both threads have completed, the highest gap-free transaction should be 10
    EXPECT_EQ(t->highestGapFree(), 10);

    delete t;
}

// Test case to check concurrent highestGapFree calls
TEST(GapFreeTrackerTest, ConcurrentHighestGapFreeAccess) {
    GapFreeTracker* t = new GapFreeTrackerImpl();

    // Lambda function for completing transactions
    auto completeTransactions = [&](const std::vector<long>& transactions) {
        for (const long id : transactions) {
            t->transactionCompleted(id);
        }
    };

    // Lambda function for reading the highest gap-free transaction
    auto readHighestGapFree = [&]() {
        for (int i = 0; i < 100; ++i) {
            long highest = t->highestGapFree();
            EXPECT_GE(highest, 0);  // Ensure the result is never negative
        }
    };

    // Create multiple threads to complete transactions
    std::thread t1(completeTransactions, std::vector<long>{1, 2, 3, 4, 5});
    std::thread t2(completeTransactions, std::vector<long>{6, 7, 8, 9, 10});
    std::thread t3(readHighestGapFree);
    std::thread t4(readHighestGapFree);

    // Join all threads
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    // The highest gap-free transaction should be 10 after all transactions complete
    EXPECT_EQ(t->highestGapFree(), 10);

    delete t;
}

// Test case for heavy concurrent transaction completion with 500 threads.
TEST(GapFreeTrackerTest, MultiThreadedPerformanceTest) {
    GapFreeTracker* tracker = new GapFreeTrackerImpl();

    constexpr int numThreads = 500;
    std::vector<std::thread> threads;

    // Lambda function to simulate transaction completions by threads.
    auto threadFunc = [&tracker](const long startId) {
        tracker->transactionCompleted(startId);
        tracker->transactionCompleted(startId + 1);
    };

    // Start 500 threads, each completing 2 transactions.
    threads.reserve(numThreads);
for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(threadFunc, i * 2 + 1);
    }

    // Join all threads to ensure they complete.
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    // After all threads have completed their transactions, the highest gap-free transaction
    // should be equal to numThreads * 2.
    EXPECT_EQ(tracker->highestGapFree(), numThreads * 2);

    delete tracker;
}

// Additional tests for edge cases
TEST(GapFreeTrackerTest, NoTransactionCompleted) {
    GapFreeTracker* t = new GapFreeTrackerImpl();
    EXPECT_EQ(t->highestGapFree(), 0);
    delete t;
}

TEST(GapFreeTrackerTest, TransactionWithGap) {
    GapFreeTracker* t = new GapFreeTrackerImpl();
    t->transactionCompleted(2);
    EXPECT_EQ(t->highestGapFree(), 0);  // Since transaction 1 is not completed
    delete t;
}

// Main function for running all tests
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
