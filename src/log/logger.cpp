#include "src/log/logger.h"
#include "src/util/util.h"

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include <memory>

namespace yns {

const char *header_fmt = "%s FILE:%s LINE:%d THREAD:%d %s ";

Logger::Logger():log_level(LOG_LEVEL_DEBUG) {}
Logger::~Logger() {}

void Logger::init() {}
void Logger::close() {}
void Logger::set_level(int lvl) { log_level = lvl; }
int Logger::get_level() { return log_level; }

void Logger::debug(const char* file_name, int line_no, const char *fmt...) {
    if (log_level > LOG_LEVEL_DEBUG) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    va_log("DEBUG", file_name, line_no, fmt, args);
    va_end(args);
}

void Logger::warn(const char* file_name, int line_no, const char *fmt...) {
    if (log_level > LOG_LEVEL_WARN) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    va_log("WARN", file_name, line_no, fmt, args);
    va_end(args);
}

void Logger::error(const char* file_name, int line_no, const char *fmt...) {
    if (log_level > LOG_LEVEL_ERROR) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    va_log("ERROR", file_name, line_no, fmt, args);
    va_end(args);
}

void Logger::va_log(const char *lvl,
        const char* file_name, int line_no, const char *fmt, va_list args) {

    char cur_time_str[TIME_FMT_BUFFER_LEN];
    if (get_current_time_fmt(cur_time_str) != 0) {
        LOG_ERROR("get_current_time_fmt failed");
        return;
    }

    char full_fmt[MAX_FULL_FMT_LEN+1];
    int header_len = snprintf(full_fmt, MAX_FULL_FMT_LEN+1, header_fmt,
            lvl, file_name, line_no, get_tid(), cur_time_str);
    if (header_len >= MAX_FULL_FMT_LEN+1) {
        LOG_ERROR("log header too long");
        return;
    }

    int input_fmt_len = strlen(fmt);
    if ((header_len + input_fmt_len) >= MAX_FULL_FMT_LEN) {
        LOG_ERROR("log fmt too long");
        return;
    }

    memcpy(full_fmt + header_len, fmt, input_fmt_len);
    full_fmt[header_len + input_fmt_len] = '\0';

    char log_buf[MAX_LOG_LEN+1];
    int log_len = vsnprintf(log_buf, MAX_LOG_LEN+1, full_fmt, args);
    if (log_len >= MAX_LOG_LEN+1) {
        LOG_ERROR("log line too long");
        return;
    }

    do_output(log_buf, log_len);
}

void Logger::do_output(const char *log_buf, int log_buf_len) {
    // Lock(mutex); // optional lock
    fprintf(stdout, "%s\n", log_buf);
}

}
