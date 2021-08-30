#ifndef _SRC_UTIL_ITASK_H_
#define _SRC_UTIL_ITASK_H_

namespace yns {

class ITask {
public:
    ITask(){};
    virtual ~ITask(){};
    virtual void execute() = 0;
};

}

#endif //_SRC_UTIL_ITASK_H_
