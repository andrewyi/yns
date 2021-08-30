#include "src/util/syncer.h"
#include "src/log/logger.h"
#include "src/util/exception.h"

#include <errno.h>

namespace yns {

Mutex::Mutex() {
    pthread_mutexattr_init(&attr);
    if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_FAST_NP) != 0) {
        LOG_ERROR("pthread_mutexattr_settype failed");
        throw Fatal("pthread_mutexattr_settype failed");
    }

    pthread_mutex_init(&mutex, &attr);
}

Mutex::~Mutex() {
    pthread_mutexattr_destroy(&attr);
    pthread_mutex_destroy(&mutex);
}

void Mutex::lock() {
    if (pthread_mutex_lock(&mutex) != 0) {
        LOG_ERROR("pthread_mutex_lock failed");
        throw Fatal("pthread_mutex_lock failed");
    }
}

int Mutex::trylock() {
    int res = pthread_mutex_trylock(&mutex);
    if (res == 0) {
        return 0;

    } else if (res == EBUSY) {
        return -1;

    } else {
        LOG_ERROR("pthread_mutex_trylock failed");
        throw Fatal("pthread_mutex_trylock failed");
        // return -1;
    }
}

void Mutex::unlock() {
    if (pthread_mutex_unlock(&mutex) != 0) {
        LOG_ERROR("pthread_mutex_unlock failed");
        throw Fatal("pthread_mutex_unlock failed");
    }
}

pthread_mutex_t& Mutex::m() {
    return mutex;
}

Lock::Lock(Mutex& m): mutex(m) {
    mutex.lock();
}

Lock::~Lock() {
    mutex.unlock();
}

Condition::Condition() {
    pthread_cond_init(&cond, NULL);

}

Condition::~Condition() {
    pthread_cond_destroy(&cond);
}

void Condition::wait(Mutex& mutex) {
    pthread_cond_wait(&cond, &mutex.m());
}

void Condition::notify_one() {
    pthread_cond_signal(&cond);
}

void Condition::notify_all() {
    pthread_cond_broadcast(&cond);
}

}
