#ifndef _SRC_UTIL_IALARM_H_
#define _SRC_UTIL_IALARM_H_

namespace yns {

class IAlarm {
public:
    IAlarm(){};
    virtual ~IAlarm(){};
    virtual void alarm() = 0;
};

}

#endif //_SRC_UTIL_IALARM_H_
