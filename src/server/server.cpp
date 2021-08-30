#include "src/server/server.h"
#include "src/server/poller.h"
#include "src/util/exception.h"
#include "src/util/worker.h"
#include "src/log/logger.h"

#include <unistd.h>
#include <signal.h>

namespace yns {

Server::Server(int poller_count, int worker_count):
    poller_count(poller_count), worker_count(worker_count) {}

Server::~Server(){}

void Server::start(){
    // ignore sigpipe
    sigset_t sigset;
    if (sigemptyset(&sigset) != 0) {
        LOG_ERROR("empty signal set failed");
        throw Fatal("empty signal set failed");
    }
    if (sigaddset(&sigset, SIGPIPE) != 0) {
        LOG_ERROR("add signal set failed");
        throw Fatal("add signal set failed");
    }
    if (sigprocmask(SIG_BLOCK, &sigset, NULL) != 0) {
        LOG_ERROR("block signal failed");
        throw Fatal("block signal failed");
    }

    for (int i=0; i!= poller_count; i++) {
        std::shared_ptr<Thread> t = std::make_shared<Thread>(std::make_shared<Poller>(queue));
        poller_threads.push_back(t);
        t->start();
    }

    for (int i=0; i!= worker_count; i++) {
        std::shared_ptr<Thread> t = std::make_shared<Thread>(std::make_shared<Worker>(queue));
        worker_threads.push_back(t);
        t->start();
    }

    pause();
    LOG_DEBUG("receive signal, exiting...");
}

}
