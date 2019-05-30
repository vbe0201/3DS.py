#pragma once

#include <sys/time.h>

#include "3ds/synchronization.h"
#include "3ds/svc.h"

typedef struct {
    LightLock *lock;
} CondVar;

static inline void CondVar_Init(CondVar *condvar, LightLock *lock) {
    if (lock == NULL) {
        LightLock_Init(condvar->lock);
    } else {
        LightLock_Init(lock);
        condvar->lock = lock;
    }
    
}

inline Result CondVar_NotifyOne(CondVar *condvar) {
    Handle arbiter = __sync_get_arbiter();

    return svcArbitrateAddress(arbiter, (u32)condvar->lock, ARBITRATION_SIGNAL, 1, 0);
}

inline Result CondVar_NotifyAll(CondVar *condvar) {
    if (condvar->lock == NULL)
        return 0;

    Handle arbiter = __sync_get_arbiter();

    return svcArbitrateAddress(arbiter, (u32)condvar->lock, ARBITRATION_SIGNAL, -1, 0);
}

/*
 * To avoid linker errors, we only provide definitions for these functions if this file
 * gets included in Python/condvar.h.
 */
#ifndef Py_HAVE_CONDVAR

int CondVar_Wait(CondVar *condvar, LightLock *lock) {
    if (condvar->lock != lock) {
        if (condvar->lock != NULL)
            return -1;

        __atomic_compare_exchange_n(condvar->lock, lock, 0, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
    }

    LightLock_Unlock(lock);

    Handle arbiter = __sync_get_arbiter();
    svcArbitrateAddress(arbiter, (u32)condvar->lock, ARBITRATION_WAIT_IF_LESS_THAN, 2, 0);

    LightLock_Lock(lock);

    return 1;
}

int CondVar_WaitTimeout(CondVar *condvar, LightLock *lock, s64 timeout) {
    if (condvar->lock != lock) {
        if (condvar->lock != NULL)
            return -1;

        __atomic_compare_exchange_n(condvar->lock, lock, 0, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
    }

    clock_t start, end;
    s64 elapsed;
    start = clock();

    LightLock_Unlock(lock);

    Handle arbiter = __sync_get_arbiter();
    svcArbitrateAddress(arbiter, (u32)condvar->lock, ARBITRATION_WAIT_IF_LESS_THAN_TIMEOUT, 2, timeout);

    LightLock_Lock(lock);

    end = clock();
    elapsed = (s64)((double) (end - start)) * 1000;
    return elapsed < timeout;
}

#else

int CondVar_Wait(CondVar *condvar, LightLock *lock);
int CondVar_WaitTimeout(CondVar *condvar, LightLock *lock, s64 timeout);

#endif
