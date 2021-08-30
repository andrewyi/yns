#include "src/server/eventer.h"
#include "src/util/exception.h"
#include "src/log/logger.h"

#include <stdint.h>

#include <sys/eventfd.h>
#include <unistd.h>

namespace yns {

Eventer::Eventer() {
    event_fd = eventfd(0, EFD_NONBLOCK);
    if (event_fd == -1) {
        LOG_ERROR("create eventfd failed");
        throw Fatal("create eventfd failed");
    }
}

Eventer::~Eventer() {
    close(event_fd);
}

int Eventer::fd() {
    return event_fd;
}

void Eventer::clear() {
    uint64_t tmp;
    int res = read(event_fd, &tmp, 0x8);
    if ( res == 0x8 || (res == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) ) {
        // pass
    } else {
        LOG_ERROR("eventer clear failed");
        throw Fatal("eventer clear failed");
    }
}

void Eventer::notify() { // normally counter should not overflow and then block
    uint64_t tmp = 0x1;
    if ( write(event_fd, &tmp, 0x8) != 0x8 ) {
        LOG_ERROR("eventer noitify failed, errno: %d", errno);
        throw Fatal("eventer noitify failed");
    }
}

}
