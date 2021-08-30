#include "src/server/timer.h"
#include "src/util/exception.h"
#include "src/log/logger.h"
#include "src/util/util.h"

namespace yns {

Timer::Timer() {}
Timer::~Timer() {}

void Timer::try_alarm() {
    int cur_ms = get_cur_ms();
    if (cur_ms == -1) {
        LOG_ERROR("get current timestamp failed");
        throw Fatal("get current timestamp failed");
    }

    while (true) {
        if (bheap.empty()) {
            break;
        }

        if (cur_ms > bheap.front().on_ts) {
            auto a = bheap.take().alarmer;
            LOG_DEBUG("peek alarmer and execute");
            a->alarm();

        } else {
            break;
        }
    }
}

void Timer::register_handler(int countdown_ms, std::shared_ptr<IAlarm> alarmer) {
    int cur_ms = get_cur_ms();
    if (cur_ms == -1) {
        LOG_ERROR("get current timestamp failed");
        throw Fatal("get current timestamp failed");
    }

    timer::Alarmer a(cur_ms + countdown_ms, alarmer);
    bheap.put(a);
}

}
