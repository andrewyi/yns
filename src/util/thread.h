#ifndef _SRC_UTIL_THREAD_H_
#define _SRC_UTIL_THREAD_H_

#include "src/util/irunnable.h"

#include <memory>
#include <pthread.h>


namespace yns {

class Thread {
public:
    Thread(std::shared_ptr<IRunnable>);
    ~Thread();
    void start();

protected:
    std::shared_ptr<IRunnable> runnable;
    pthread_t ptid;
    pthread_attr_t attr;
};

}

#endif //_SRC_UTIL_THREAD_H_
