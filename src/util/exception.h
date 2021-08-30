#ifndef _SRC_UTIL_EXCEPTION_H_
#define _SRC_UTIL_EXCEPTION_H_

#include <string>

namespace yns {

class Exception {
public:
    Exception(std::string);
    virtual ~Exception();
    std::string desc();

protected:
    std::string msg;
};

class Fatal: public Exception{
public:
    Fatal(std::string);
    ~Fatal();
};

};

#endif //_SRC_UTIL_EXCEPTION_H_
