#ifndef _SRC_SERVER_POLLER_H_
#define _SRC_SERVER_POLLER_H_

#include "src/util/irunnable.h"
#include "src/util/ialarm.h"
#include "src/util/syncer.h"
#include "src/util/block_queue.h"
#include "src/server/binder.h"
#include "src/server/eventer.h"
#include "src/server/timer.h"

#include <memory>
#include <map>
#include <set>
#include <sys/epoll.h>
#include <stdint.h>

#define MAX_FD_COUNT_PER_ITER 100
#define MAX_CONN_IDLE_MS (20*1000)

namespace yns {

class Conn;


class Poller : public IRunnable, public IAlarm, public std::enable_shared_from_this<Poller> {
public:
    Poller(BlockQueue<std::shared_ptr<ITask>>&);
    ~Poller();

    void run();
    void poll();

    void process_timer();

    void process_accept();

    void process_eventer();
    void process_async_fds();
    std::set<int> poll_async_fds();
    void schedule_to_worker(int fd);
    void add_async_fd(int);
    void notify_eventer();

    void process_conn(struct epoll_event *);

    void force_remove_fd(int fd);
    void remove_fd(int fd);
    void arm(int fd, uint32_t events);

    void alarm();

protected:
    Binder binder;
    Eventer eventer;
    Timer timer;

    struct epoll_event *fd_list;
    int epoll_fd;
    uint64_t poll_tmout;
    std::map<int, std::shared_ptr<Conn>> conns;
    BlockQueue<std::shared_ptr<ITask>>& queue;

    Mutex async_fds_mutex;
    std::set<int> async_fds;
};

}

#endif //_SRC_SERVER_POLLER_H_
