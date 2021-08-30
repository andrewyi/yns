#ifndef _SRC_UTIL_SYNCER_H
#define _SRC_UTIL_SYNCER_H

#include <pthread.h>

namespace yns {

class Mutex {
public:
    Mutex();
    ~Mutex();
    void lock();
    int trylock();
    void unlock();
    pthread_mutex_t& m();

    // neither copyable nor assignable
    Mutex(const Mutex&) = delete;
    Mutex& operator=(const Mutex&) = delete;

protected:
    pthread_mutex_t mutex;
    pthread_mutexattr_t attr;
};

class Lock{
public:
    Lock(Mutex&);
    ~Lock();

    // neither copyable nor assignable
    Lock() = delete;
    Lock(const Lock&) = delete;
    Lock& operator=(const Lock&) = delete;

protected:
    Mutex& mutex;
};

class Condition {
public:
    Condition();
    ~Condition();
    void wait(Mutex&);
    void notify_one();
    void notify_all();

    // neither copyable nor assignable
    Condition(const Condition&) = delete;
    Condition& operator=(const Condition&) = delete;

protected:
    pthread_cond_t cond;
};

}

#endif //_SRC_UTIL_SYNCER_H
