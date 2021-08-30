#ifndef _SRC_SERVER_EVENTER_H_
#define _SRC_SERVER_EVENTER_H_

namespace yns {

class Eventer {
public:
    Eventer();
    ~Eventer();

    int fd();
    void clear();
    void notify();

protected:
    int event_fd;
};

}

#endif //_SRC_SERVER_EVENTER_H_
