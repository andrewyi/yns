#ifndef _SRC_SERVER_CONN_H_
#define _SRC_SERVER_CONN_H_

#include "src/util/buffer.h"
#include "src/util/itask.h"

#include <sys/epoll.h>
#include <stdint.h>

#define CONN_FLAG_ERROR         (0x1 << 0)
#define CONN_FLAG_PEER_CLOSE    (0x1 << 1)
#define CONN_FLAG_PROCESSING    (0x1 << 2)

namespace yns {

class Poller;

class Conn : public ITask {
public:
    Conn(int, Poller&);
    ~Conn();
    void process(uint32_t events);
    void handle_events(uint32_t events);
    void rearm();
    void read();
    void write();
    int get_last_active();

    void set_peer_close();
    bool is_peer_close();
    void set_error();
    bool is_error();
    void set_processing();
    void clear_processing();
    bool is_processing();

    // TODO: inject handlers here
    void try_handle_request();
    void try_handle_request_async();
    void schedule_async();
    void execute();

protected:
    int fd;
    Buffer wb;
    Buffer rb;
    uint32_t flags;
    int last_active;

    Poller& poller;
};

}

#endif //_SRC_SERVER_CONN_H_
