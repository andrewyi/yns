#include "src/server/binder.h"
#include "src/log/logger.h"
#include "src/util/exception.h"
#include "src/util/util.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

namespace yns {

Binder::Binder(): ipaddr("0.0.0.0"), port(38000) {
    // do bind
    bind_fd = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK, 0);
    if (bind_fd == -1) {
        LOG_ERROR("create socket failed");
        throw Fatal("create socket failed");
    }

    if ( setsock_reuseport(bind_fd) != 0 ) {
        LOG_ERROR("setsockopt reuseport failed");
        throw Fatal("setsockopt reuseport failed");
    }

    if ( setsock_reuseaddr(bind_fd) != 0 ) {
        LOG_ERROR("setsockopt reuseaddr failed");
        throw Fatal("setsockopt reuseaddr failed");
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ipaddr.c_str(), &addr.sin_addr) != 1) {
        LOG_ERROR("convert addr failed");
        throw Fatal("convert addr failed");
    }

    if (bind(bind_fd, (const sockaddr*)&addr, sizeof(struct sockaddr_in)) != 0) {
        LOG_ERROR("bind failed");
        throw Fatal("bind failed");
    }

    if (listen(bind_fd, LISTEN_BACKLOG) != 0) {
        LOG_ERROR("listen failed");
        throw Fatal("listen failed");
    }
}

Binder::~Binder() {
    close(bind_fd);
}

int Binder::fd() {
    return bind_fd;
}

// -1: no more connections
int Binder::accept() {
    int fd = accept4(bind_fd, NULL, NULL, SOCK_NONBLOCK);
    if (fd == -1) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return -1;

        } else {
            int en = errno;
            LOG_ERROR("accept failed, errno: %d", en);
            throw Fatal("accept failed");
        }
    }

    if (setsock_no_linger(fd) != 0) {
        LOG_ERROR("set socket no linger failed");
        throw Fatal("set socket no linger failed");
    }

    return fd;
}

}
