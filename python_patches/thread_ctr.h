#include <sys/reent.h>

#include "3ds/result.h"
#include "3ds/svc.h"
#include "3ds/synchronization.h"
#include "3ds/thread.h"
#include "3ds/types.h"

#include "condvars_ctr.h"

#define THREAD_STACK_SIZE 0x1000
#define THREAD_PRIORITY 0x20
#define THREAD_CPU_ID -2

/* copied from libctru/source/internal.h */

#define THREADVARS_MAGIC 0x21545624 // !TV$

// Keep this structure under 0x80 bytes
typedef struct
{
	// Magic value used to check if the struct is initialized
	u32 magic;

	// Pointer to the current thread (if exists)
	Thread thread_ptr;

	// Pointer to this thread's newlib state
	struct _reent* reent;

	// Pointer to this thread's thread-local segment
	void* tls_tp; // !! Keep offset in sync inside __aeabi_read_tp !!
} ThreadVars;

static inline ThreadVars* getThreadVars(void)
{
	return (ThreadVars*)getThreadLocalStorage();
}

/* end copy */

/*
 * Initialization.
 */
static void
_noop(void)
{
}

static void
PyThread__init_thread(void)
{
    Thread t = threadCreate((void *)_noop, NULL, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_CPU_ID, false);
    threadJoin(t, U64_MAX);
    threadFree(t);
}

/*
 * Thread support.
 */

typedef struct {
    ThreadFunc func;
    void *arg;
} _wrapper_args;


static void
_wrapper_func(_wrapper_args *args)
{
    args->func(args->arg);
}

long
PyThread_start_new_thread(void (*func)(void *), void *arg)
{
    dprintf(("PyThread_start_new_thread called.\n"));
    if (!initialized)
        PyThread_init_thread();

    _wrapper_args wargs;
    wargs.arg = arg;
    wargs.func = func;

    PyMem_RawMalloc(sizeof(Thread));
    Thread t = threadCreate((ThreadFunc)_wrapper_func, &wargs, THREAD_STACK_SIZE, THREAD_PRIORITY, THREAD_CPU_ID, true);
    
    return !t ? -1 : 0;
}

long
PyThread_get_thread_ident(void)
{
    if (!initialized)
        PyThread_init_thread();

    Thread t = getThreadVars()->thread_ptr;
    return (long)t + 1;
}

void
PyThread_exit_thread(void)
{
    dprintf(("PyThread_exit_thread called.\n"));

    if (!initialized)
        exit(0);

    Thread t = getThreadVars()->thread_ptr;
    threadExit(!t ? -1 : 0);
    PyMem_RawFree((void *)t);
}

/*
 * Lock support.
 */

typedef struct {
    char locked;
    CondVar cv;
    LightLock lock;
} _thread_lock;

PyThread_type_lock
PyThread_allocate_lock(void)
{
    dprintf(("PyThread_allocate_lock called.\n"));

    if (!initialized)
        PyThread_init_thread();

    _thread_lock *lock = (_thread_lock *)PyMem_RawMalloc(sizeof(_thread_lock));
    lock->locked = 0;

    LightLock_Init(&(lock->lock));

    CondVar_Init(&(lock->cv), &(lock->lock));

    return (PyThread_type_lock)lock;
}

void
PyThread_free_lock(PyThread_type_lock lock)
{
    _thread_lock *thread_lock = (_thread_lock *)lock;

    if (!thread_lock)
        return;

    PyMem_RawFree((void *)thread_lock);
}

int
PyThread_acquire_lock(PyThread_type_lock lock, int waitflag)
{
    return PyThread_acquire_lock_timed(lock, waitflag ? -1 : 0, 0);
}

PyLockStatus
PyThread_acquire_lock_timed(PyThread_type_lock lock, PY_TIMEOUT_T microseconds,
                            int intr_flag)
{
    PyLockStatus success;
    int status;
    _thread_lock *thread_lock = (_thread_lock *)lock;
    dprintf(("PyThread_acquire_lock_timed(%p, %lld, %d) called.\n",
            lock, microseconds, intr_flag));

    if (thread_lock->locked == 1 && microseconds == 0) {
        success = PY_LOCK_FAILURE;
        return success;
    }

    LightLock_Lock(&(thread_lock->lock));
    if (thread_lock->locked == 0)
        success = PY_LOCK_ACQUIRED;
    else if (microseconds == 0)
        success = PY_LOCK_FAILURE;
    else {
        u64 ns = microseconds * 1000;

        success = PY_LOCK_FAILURE;
        while (success == PY_LOCK_FAILURE) {
            if (microseconds > 0) {
                status = CondVar_WaitTimeout(&(thread_lock->cv), &(thread_lock->lock), ns);
                if (!status) // timeout
                    break;
            } else {
                status = CondVar_Wait(&(thread_lock->cv), &(thread_lock->lock));
            }

            if (intr_flag && status == 1 && thread_lock->locked) {
                /*
                 * Somehow we've been signaled but didn't get the lock.
                 * Return PY_LOCK_INTR to allow the caller to handle it and retry.
                 */
                success = PY_LOCK_INTR;
                break;
            } else if (!thread_lock->locked)
                success = PY_LOCK_ACQUIRED;
            else
                success = PY_LOCK_FAILURE;
        }
    }

    if (success == PY_LOCK_ACQUIRED)
        thread_lock->locked = 1;

    LightLock_Unlock(&(thread_lock->lock));

    return success;
}

void
PyThread_release_lock(PyThread_type_lock lock)
{
    _thread_lock *thread_lock = (_thread_lock *)lock;

    LightLock_Lock(&(thread_lock->lock));
    thread_lock->locked = 0;
    CondVar_NotifyOne(&(thread_lock->cv));
    LightLock_Unlock(&(thread_lock->lock));
}

/* The following functions require native TLS. */
#undef Py_HAVE_NATIVE_TLS

#ifdef Py_HAVE_NATIVE_TLS
int
PyThread_create_key(void)
{
    int result;
    unsigned char buffer[24];

    return result;
}

void
PyThread_delete_key(int key)
{
}

int
PyThread_set_key_value(int key, void *value)
{
    int ok;

    /* A failure in this case returns -1. */
    if (!ok)
        return -1;
    return 0;
}

void *
PyThread_get_key_value(int key)
{
    void *result;
    return result;
}

void
PyThread_delete_key_value(int key)
{
}

void
PyThread_ReInitTLS(void)
{
}
#endif
