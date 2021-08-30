#ifndef _SRC_SERVER_TIMER_H_
#define _SRC_SERVER_TIMER_H_

#include "src/util/ialarm.h"
#include "src/util/binary_heap.h"

#include <memory>

namespace yns {

namespace timer {

class Alarmer {
public: 
    Alarmer(int t, std::shared_ptr<yns::IAlarm> a):on_ts(t), alarmer(a) {}
    bool operator<(const Alarmer& oa) {
        return this->on_ts < oa.on_ts;
    }

    int on_ts;
    std::shared_ptr<yns::IAlarm> alarmer;
};

}

class Timer {
public:
    Timer();
    ~Timer();

    void register_handler(int, std::shared_ptr<IAlarm>);
    void try_alarm();

protected:
    BinaryHeap<timer::Alarmer> bheap;
};

}

#endif //_SRC_SERVER_TIMER_H_
