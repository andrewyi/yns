#include "src/server/conn.h"
#include "src/log/logger.h"
#include "src/server/poller.h"
#include "src/util/exception.h"
#include "src/util/util.h"

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>


namespace yns {

Conn::Conn(int fd, Poller& p): fd(fd), flags(0), poller(p) {
    last_active = get_cur_ms();
}

Conn::~Conn(){}

void Conn::process(uint32_t events) {
    handle_events(events);
    rearm();
}

void Conn::handle_events(uint32_t events) {
    if ((events & EPOLLERR) || (events & EPOLLHUP)) {
        LOG_DEBUG("fd(%d) error/hup event", fd);
        set_error();
        return;
    }

    if (events & EPOLLIN) {
        LOG_DEBUG("fd(%d) read event", fd);
        read();
        try_handle_request();
    }

    if (events & EPOLLRDHUP) {
        LOG_DEBUG("fd(%d) read hup event", fd);
        set_peer_close();
    }

    if (events & EPOLLOUT) {
        LOG_DEBUG("fd(%d) write event", fd);
        write();
    }

    return;
}

void Conn::rearm() {
    if ( is_processing() ) {
        return;
    }

    if ( is_error() ) {
        LOG_WARN("close fd(%d) due to abnormal", fd);
        poller.force_remove_fd(fd);
        return;
    }

    if ( wb.empty() && is_peer_close() ) {
        LOG_DEBUG("close fd(%d) for no more interaction", fd);
        poller.remove_fd(fd);
        return;
    }

    int next_events = EPOLLET | EPOLLONESHOT;
    if ( !is_peer_close() ) {
        next_events |= (EPOLLIN | EPOLLRDHUP);
    }
    if ( !wb.empty() ) {
        next_events |= EPOLLOUT;
    }
    poller.arm(fd, next_events);
}

void Conn::read() {
    int total = 0;
    ssize_t size = 0;
    while (true) {
        int cur_ava_len = rb.free_len();
        size = recv(fd, rb.free_start(), cur_ava_len, 0);
        if (size == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;

            } else {
                LOG_ERROR("fd (%d) recv failed, errno: %d", fd, errno);
                throw Fatal("recv failed");
            }

        } else if (size == 0) {
            set_peer_close();
            break;

        } else {
            total += size;
            rb.increase_free_bytes(size);
            if (size == cur_ava_len) {
                rb.scale();
            } else {
                break;
            }
        }
    }

    last_active = get_cur_ms();
    LOG_DEBUG("read total %d bytes", total);
}

void Conn::write() {
    ssize_t size = send(fd, wb.used_start(), wb.used_len(), 0);
    if (size == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return;

        } else if (errno == EPIPE) {
            set_error();
            return;

        } else {
            LOG_ERROR("fd (%d) recv failed, errno: %d", fd, errno);
            throw Fatal("recv failed");
        }
    } else {
        wb.increase_used_bytes(size);
        wb.shrink();
    }

    last_active = get_cur_ms();
    LOG_DEBUG("write total %d bytes", size);
}

int Conn::get_last_active() {
    return last_active;
}

void Conn::set_peer_close() {
    flags |= CONN_FLAG_PEER_CLOSE;
}

bool Conn::is_peer_close() {
    return flags & CONN_FLAG_PEER_CLOSE;
}

void Conn::set_error() {
    flags |= CONN_FLAG_ERROR;
}

bool Conn::is_error() {
    return flags & CONN_FLAG_ERROR;
}

void Conn::set_processing() {
    flags |= CONN_FLAG_PROCESSING;
}

void Conn::clear_processing() {
    flags &= ~CONN_FLAG_PROCESSING;
}

bool Conn::is_processing() {
    return flags & CONN_FLAG_PROCESSING;
}

void Conn::schedule_async() {
    poller.schedule_to_worker(fd);
}

void Conn::execute() {
    try_handle_request_async();
    poller.add_async_fd(fd);
    poller.notify_eventer();
}

void Conn::try_handle_request() {
    schedule_async();
}

void Conn::try_handle_request_async() {
    int rb_used_len = rb.used_len();
    wb.put(rb.used_start(), rb_used_len);
    rb.increase_used_bytes(rb_used_len);
}

}
