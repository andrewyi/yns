#ifndef _SRC_UTIL_IRUNNABLE_H_
#define _SRC_UTIL_IRUNNABLE_H_

namespace yns {

class IRunnable {
public:
    IRunnable(){};
    virtual ~IRunnable(){};
    virtual void run() = 0;
};

}

#endif //_SRC_UTIL_IRUNNABLE_H_
