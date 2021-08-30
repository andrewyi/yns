#ifndef _SRC_UTIL_WORKER_H
#define _S RC_UTIL_WORKER_H

#include "src/util/block_queue.h"
#include "src/util/itask.h"
#include "src/util/irunnable.h"

#include <memory>

namespace yns {

class Worker : public IRunnable {
public:
    Worker(BlockQueue<std::shared_ptr<ITask>>&);
    ~Worker();
    void run();
    void execute_task();

protected:
    BlockQueue<std::shared_ptr<ITask>>& queue;
};

}

#endif //_SRC_UTIL_WORKER_H
