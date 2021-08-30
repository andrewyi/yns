#include "src/util/util.h"

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

namespace yns {

int get_current_time_fmt(char *p) {
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == -1) {
        return -1;
    }
    struct tm tm_r;
    if (localtime_r(&tv.tv_sec, &tm_r) == NULL) {
        return -1;
    }

    int time_len = snprintf(p, TIME_FMT_BUFFER_LEN,
            "%04d-%02d-%02d %02d:%02d:%02d:%06ld",
            tm_r.tm_year+1900, tm_r.tm_mon+1, tm_r.tm_mday,
            tm_r.tm_hour, tm_r.tm_min, tm_r.tm_sec, tv.tv_usec);
    if (time_len >= TIME_FMT_BUFFER_LEN) {
        return -1;
    }

    return 0;
}

int get_cur_ms() {
    struct timeval tv;
    if (gettimeofday(&tv, NULL) == -1) {
        return -1;
    }

    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

static __thread pid_t thread_id;

int get_tid() {
    if (thread_id == 0) {
        thread_id = gettid();
    }
    return (int)thread_id;
}

int setsock_block(int fd) {
    return setsock_block_status(fd, true);
}

int setsock_unblock(int fd) {
    return setsock_block_status(fd, false);
}

int setsock_block_status(int fd, bool is_block) {
    int flags = fcntl(fd, F_GETFD);
    if (is_block) {
        flags &= ~O_NONBLOCK;
    } else {
        flags |= O_NONBLOCK;
    }

    if ( fcntl(fd, F_SETFL, flags) != 0 ) {
        return -1;
    }
    return 0;
}

int setsock_reuseport(int fd) {
    int opt_val = 1;
    int res = setsockopt(
                fd,
                SOL_SOCKET,
                SO_REUSEPORT,
                (const void*)&opt_val,
                (socklen_t)sizeof(int)
                );

    if (res != 0) {
        return -1;
    }
    return 0;
}

int setsock_reuseaddr(int fd) {
    int opt_val = 1;
    int res = setsockopt(
                fd,
                SOL_SOCKET,
                SO_REUSEADDR,
                (const void*)&opt_val,
                (socklen_t)sizeof(int)
                );

    if (res != 0) {
        return -1;
    }
    return 0;
}

int setsock_no_linger(int fd) {
    struct linger l;
    l.l_onoff = 0;

    int res = setsockopt(
                fd,
                SOL_SOCKET,
                SO_LINGER,
                (const void*)&l,
                (socklen_t)sizeof(struct linger)
                );

    if (res != 0) {
        return -1;
    }
    return 0;
}

}
