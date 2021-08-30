#ifndef _SRC_SERVER_SERVER_H_
#define _SRC_SERVER_SERVER_H_

#include "src/util/thread.h"
#include "src/util/block_queue.h"
#include "src/util/itask.h"

#include <vector>
#include <memory>

namespace yns {

class Server {
public:
    Server(int, int);
    ~Server();
    void start(void);

protected:
    int poller_count;
    int worker_count;
    std::vector<std::shared_ptr<Thread>> poller_threads;
    std::vector<std::shared_ptr<Thread>> worker_threads;
    BlockQueue<std::shared_ptr<ITask>> queue;
};

}

#endif // _SRC_SERVER_SERVER_H_
