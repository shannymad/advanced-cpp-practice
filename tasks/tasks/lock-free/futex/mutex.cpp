#include <mutex.h>
#include <cstdint>

/* Implementation of futex syscalls for Linux & MacOS */

#if defined(__linux__)

#include <unistd.h>
#include <linux/futex.h>
#include <sys/syscall.h>

void FutexWait(void *addr, uint64_t expected_value) {
    syscall(SYS_futex, addr, FUTEX_WAIT_PRIVATE, expected_value, nullptr, nullptr, 0);
}

void FutexWakeOne(void *addr) {
    syscall(SYS_futex, addr, FUTEX_WAKE_PRIVATE, 1, nullptr, nullptr, 0);
}

void FutexWakeAll(void *addr) {
    syscall(SYS_futex, addr, FUTEX_WAKE_PRIVATE, INT_MAX, nullptr, nullptr, 0);
}

#elif defined(__APPLE__)

// NOLINTBEGIN(readability-identifier-naming)
extern "C" int __ulock_wait(uint32_t operation, void *addr, uint64_t value, uint32_t timeout);
extern "C" int __ulock_wake(uint32_t operation, void *addr, uint64_t wake_value);
// NOLINTEND(readability-identifier-naming)

#define UL_COMPARE_AND_WAIT 1
#define ULF_WAKE_ALL        0x00000100

void FutexWait(void *addr, uint64_t expected_value) {
    __ulock_wait(UL_COMPARE_AND_WAIT, addr, expected_value, 0);
}

void FutexWakeOne(void *addr) {
    __ulock_wake(UL_COMPARE_AND_WAIT, addr, 0);
}

void FutexWakeAll(void *addr) {
    __ulock_wake(UL_COMPARE_AND_WAIT | ULF_WAKE_ALL, addr, 0);
}

#undef ULF_WAKE_ALL
#undef UL_COMPARE_AND_WAIT

#else

#error "Only Linux & MacOS platforms are supported!"

#endif
