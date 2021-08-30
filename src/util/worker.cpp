#include "src/util/worker.h"
#include "src/log/logger.h"

namespace yns{

Worker::Worker(BlockQueue<std::shared_ptr<ITask>>& q):queue(q) {}
Worker::~Worker() {}

void Worker::run() {
    while (true) {
        execute_task();
    }
}

void Worker::execute_task() {
    std::shared_ptr<ITask> p = queue.take();
    LOG_DEBUG("peek task and execute");
    p->execute();
}

}
