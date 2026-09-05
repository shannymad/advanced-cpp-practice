#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>

// Atomically do the following:
//    if (*(uint64_t*)addr == expected_value) {
//        sleep_on_address(addr)
//    }
void FutexWait(void *addr, uint64_t expected_value);

// Wakeup 1 thread sleeping on the given address
void FutexWakeOne(void *addr);

// Wakeup all threads sleeping on the given address
void FutexWakeAll(void *addr);

class Mutex {
public:
    void Lock() {
        mutex_.lock();
    }

    void Unlock() {
        mutex_.unlock();
    }

private:
    std::mutex mutex_;
};
