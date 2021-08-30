#include "src/util/thread.h"
#include "src/log/logger.h"
#include "src/util/exception.h"

#include <pthread.h>

namespace yns {

void *thread_run_wrapper(void *p) {
    IRunnable *r = reinterpret_cast<IRunnable*>(p);
    r->run();
    return NULL;
}

Thread::Thread(std::shared_ptr<IRunnable> r):runnable(r) {
    if (pthread_attr_init(&attr) != 0) {
        LOG_ERROR("pthread_attr_init failed");
        throw Fatal("pthread_attr_init failed");
    }

    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0) {
        LOG_ERROR("pthread_attr_setdetachstate failed");
        throw Fatal("pthread_attr_setdetachstate failed");
    }
}

Thread::~Thread() {
    pthread_attr_destroy(&attr);
}

void Thread::start() {
    int res = pthread_create(
            &ptid, &attr, &thread_run_wrapper, (void*)runnable.get());

    if (res != 0) {
        int en = errno;
        LOG_ERROR("pthread_create failed, errno: %d", en);
        throw Fatal("pthread_create failed");
    }

    LOG_DEBUG("thread created, id: %ld", ptid);
}

}
