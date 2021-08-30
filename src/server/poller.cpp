#include "src/server/poller.h"
#include "src/server/binder.h"
#include "src/server/conn.h"
#include "src/log/logger.h"
#include "src/util/exception.h"
#include "src/util/util.h"

#include <unistd.h>
#include <stdlib.h>

namespace yns {

Poller::Poller(BlockQueue<std::shared_ptr<ITask>>& q):
    binder(), eventer(), timer(), poll_tmout(100), queue(q) {

    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        LOG_ERROR("create epoll_fd failed");
        throw Fatal("create epoll_fd failed");
    }

    // inject binder fd
    struct epoll_event e;
    e.events = EPOLLIN | EPOLLET;
    e.data.fd = binder.fd();
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, e.data.fd, &e) != 0) {
        close(epoll_fd);
        LOG_ERROR("inject binder fd failed");
        throw Fatal("inject binder fd failed");
    }

    // inject eventer fd
    e.events = EPOLLIN;
    e.data.fd = eventer.fd();
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, e.data.fd, &e) != 0) {
        close(epoll_fd);
        LOG_ERROR("inject eventer fd failed");
        throw Fatal("inject eventer fd failed");
    }

    fd_list = (struct epoll_event *)malloc(
            MAX_FD_COUNT_PER_ITER * sizeof(struct epoll_event));
    if (fd_list == NULL) {
        close(epoll_fd);
        LOG_ERROR("malloc for fd_list failed");
        throw Fatal("malloc for fd_list failed");
    }
}

Poller::~Poller() {
    close(epoll_fd);
    free(fd_list);
}

void Poller::run() {
    // inject timer, can not be put in constructor
    timer.register_handler(5000, shared_from_this());

    while (true) {
        poll();
    }
}

void Poller::poll() {
    int count = epoll_wait(
            epoll_fd, fd_list, MAX_FD_COUNT_PER_ITER, poll_tmout);

    // LOG_DEBUG("poll got %d fds ready", count);
    if (count == -1) {
        LOG_ERROR("epoll_wait failed");
        throw Fatal("epoll_wait failed");
    }

    process_timer();

    struct epoll_event *cur_event = fd_list;
    for (int i = 0; i != count; i ++, cur_event++) {
        int fd = cur_event->data.fd;
        if ( fd == binder.fd() ) {
            process_accept();

        } else if ( fd == eventer.fd() ){
            process_eventer();

        } else {
            process_conn(cur_event);
        }
    }
}

void Poller::process_timer() {
    timer.try_alarm();
}

void Poller::process_accept() {
    struct epoll_event e;
    e.events = EPOLLIN | EPOLLRDHUP | EPOLLET | EPOLLONESHOT;

    while (true) {
        int fd = binder.accept(); 
        if (fd == -1) {
            break;
        }

        conns[fd] = std::make_shared<Conn>(fd, *this);

        e.data.fd = fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &e) != 0) {
            LOG_ERROR("inject accepted fd failed");
            throw Fatal("inject accepted fd failed");
        }
    }
}

void Poller::process_eventer() {
    eventer.clear();
    process_async_fds(); 
}

void Poller::process_async_fds() {
    std::set<int> fds = poll_async_fds();
    if (fds.empty()) {
        LOG_ERROR("process_async_fds get empty fds");
        throw Fatal("process_async_fds get empty fds");
    }

    for (auto it = fds.begin(); it != fds.end(); it ++) {
        auto cit = conns.find(*it);
        if (cit == conns.end()) {
            LOG_ERROR("process async fds, find fd(%d) failed", *it);
            throw Fatal("find fd failed");
        }

        std::shared_ptr<Conn> c = cit->second;
        c->clear_processing();
        c->rearm();
    }
}

void Poller::schedule_to_worker(int fd) {
    auto it = conns.find(fd);
    if (it == conns.end()) {
        LOG_ERROR("schedule fd, find fd(%d) failed", *it);
        throw Fatal("find fd failed");
    }

    std::shared_ptr<Conn> c = it->second;
    c->set_processing();
    queue.put(c);
}

void Poller::add_async_fd(int fd) {
    Lock l(async_fds_mutex);
    async_fds.insert(fd);
}

void Poller::notify_eventer() {
    eventer.notify();
}

std::set<int> Poller::poll_async_fds() {
    Lock l(async_fds_mutex);
    std::set<int> to_process;
    to_process.swap(async_fds);
    return to_process;
}

void Poller::process_conn(struct epoll_event *e) {
    auto it = conns.find(e->data.fd);
    if (it == conns.end()) {
        LOG_WARN("process conn, find fd(%d) failed, maybe removed", e->data.fd);
        return;
    }
    std::shared_ptr<Conn> c = it->second;
    c->process(e->events);
}


void Poller::force_remove_fd(int fd) {
    remove_fd(fd);
}

void Poller::remove_fd(int fd) {
    close(fd);
    conns.erase(fd);
}

void Poller::arm(int fd, uint32_t events) {
    struct epoll_event e;
    e.events = events;
    e.data.fd = fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &e) != 0) {
        LOG_ERROR("mod fd failed");
        throw Fatal("mod fd failed");
    }
}

void Poller::alarm() {
    int cur_ms = get_cur_ms();
    std::set<int> fds;
    for (auto it = conns.begin(); it != conns.end(); it ++) {
        auto c = it->second;
        if (cur_ms > c->get_last_active() + MAX_CONN_IDLE_MS) {
            fds.insert(it->first);
        }
    }

    for (auto it = fds.begin(); it != fds.end(); it ++ ) {
        LOG_DEBUG("force remove fd(%d) for timeout of %ds", *it, MAX_CONN_IDLE_MS/1000);
        force_remove_fd(*it);
    }
    timer.register_handler(5000, shared_from_this());
}

}
