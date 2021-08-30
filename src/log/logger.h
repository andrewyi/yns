#ifndef _SRC_LOG_LOGGER_H_
#define _SRC_LOG_LOGGER_H_

#include "src/util/syncer.h"
#include "src/util/singleton.h"

#include <stdarg.h>

#define LOG_LEVEL_DEBUG  0
#define LOG_LEVEL_WARN   1
#define LOG_LEVEL_ERROR  2

#define LOG_DEBUG(...) Singleton<Logger>::instance().debug(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...) Singleton<Logger>::instance().warn(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) Singleton<Logger>::instance().error(__FILE__, __LINE__, __VA_ARGS__)

#define SET_LOG_LEVEL(lvl) yns::Singleton<yns::Logger>::instance().set_level(lvl)
#define CLOSE_LOG() yns::Singleton<yns::Logger>::destroy()

#define MAX_FULL_FMT_LEN 256
#define MAX_LOG_LEN 1024

namespace yns {

class Logger {
public:
    Logger();
    ~Logger();

    void init();
    void close();
    void set_level(int);
    int get_level();

    void debug(const char* file_name, int line_no, const char *fmt...);
    void warn(const char* file_name, int line_no, const char *fmt...);
    void error(const char* file_name, int line_no, const char *fmt...);
    void va_log(const char *lvl,
            const char* file_name, int line_no, const char *fmt, va_list args);
    void do_output(const char *log_buf, int log_buf_len);

protected:
    Mutex mutex;
    int log_level;
};

}

#endif //_SRC_LOG_LOGGER_H_
