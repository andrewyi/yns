#ifndef _SRC_UTIL_UTIL_H
#define _SRC_UTIL_UTIL_H

#include <unistd.h>
#include <sys/types.h>

#if __GLIBC__ == 2 && __GLIBC_MINOR__ < 30
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)
#endif

#define TIME_FMT_BUFFER_LEN 27

namespace yns {

int get_current_time_fmt(char *p);
int get_cur_ms();

int get_tid();
int setsock_block(int);
int setsock_nonblock(int);
int setsock_block_status(int, bool);
int setsock_reuseaddr(int);
int setsock_reuseport(int);
int setsock_no_linger(int);

}

#endif //_SRC_UTIL_UTIL_H
