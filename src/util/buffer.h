/*
 * free_pos: pos data cloud be written to
 * used_pos: pos from where data not consumed yet
 * */
#ifndef _SRC_UTIL_BUFFER_H_
#define _SRC_UTIL_BUFFER_H_

#define INIT_BUFFER_LENGTH 1024

namespace yns {

class Buffer{
public:
    Buffer();
    ~Buffer();
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    bool empty();
    char *free_start();
    int free_len();
    void increase_free_bytes(int);
    char *used_start();
    int used_len();
    void increase_used_bytes(int);
    void scale();
    void scale_to(int);
    void shrink();
    void put(char *, int);
    int take(char *, int);

protected:
    static int get_upper_len(int);
    void change_buf(int);

protected:
    char *header;
    int length;
    int free_pos;
    int used_pos;

};

}

#endif //_SRC_UTIL_BUFFER_H_
