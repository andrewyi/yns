#ifndef _SRC_SERVER_BINDER_H_
#define _SRC_SERVER_BINDER_H_

#include <cstdint>
#include <string>

#define LISTEN_BACKLOG 65535

namespace yns {

class Binder {
public:
    Binder();
    ~Binder();
    int fd();
    int accept();

protected:
    // bind addr
    uint16_t port;
    std::string ipaddr;
    int bind_fd;
};

}

#endif //_SRC_SERVER_BINDER_H_
