#ifndef _SRC_UTIL_BLOCK_QUEUE_H_
#define _SRC_UTIL_BLOCK_QUEUE_H_

#include "src/util/itask.h"
#include "src/util/syncer.h"

#include <queue>
#include <memory>

namespace yns {

template<typename T>
class BlockQueue {
public:
    BlockQueue() {}
    ~BlockQueue() {}

    T take() {
        Lock l(mutex);
        while (queue.empty()) {
            cond.wait(mutex);
        }

        T& t = queue.front();
        queue.pop();
        return t;
    }

    void put(T t) {
        Lock l(mutex);
        queue.push(t);
        cond.notify_one();
    }

private:
    Mutex mutex;
    Condition cond;
    std::queue<T> queue;
};

}

#endif //_SRC_UTIL_BLOCK_QUEUE_H_
