#include "src/util/buffer.h"
#include "src/log/logger.h"
#include "src/util/exception.h"

#include <stdlib.h>
#include <string.h>

namespace yns {

Buffer::Buffer() {
    length = INIT_BUFFER_LENGTH;
    header = (char *)malloc(length);
    if (header == NULL) {
        LOG_ERROR("malloc %d bytes failed", length);
        throw Fatal("malloc failed");
    }
    free_pos = 0;
    used_pos = 0;
}

Buffer::~Buffer() {
    free(header);
}

bool Buffer::empty() {
    return free_pos == used_pos;
}

char *Buffer::free_start() {
    return header + free_pos;
}

int Buffer::free_len() {
    return length - free_pos;
}

void Buffer::increase_free_bytes(int len) {
    free_pos += len;
}

char *Buffer::used_start() {
    return header + used_pos;
}

int Buffer::used_len() {
    return free_pos - used_pos;
}

void Buffer::increase_used_bytes(int len) {
    used_pos += len;
}

void Buffer::scale() {
    if (free_pos == used_pos) { // reset to header
        free_pos = 0;
        used_pos = 0;
        return;
    }
    change_buf(free_pos - used_pos);
}

void Buffer::scale_to(int required_free_len) {
    change_buf(free_pos - used_pos + required_free_len);
}

void Buffer::change_buf(int least_len) {
    int new_length = get_upper_len(least_len);
    char *new_header = (char *)malloc(new_length);
    if (new_header == NULL) {
        LOG_ERROR("malloc %d bytes failed", new_length);
        throw Fatal("malloc failed");
    }
    memcpy(new_header, header + used_pos, free_pos - used_pos);

    free(header);
    header = new_header;
    length = new_length;
    free_pos = free_pos - used_pos;
    used_pos = 0;
}

void Buffer::shrink() {
    scale();
}

void Buffer::put(char *src, int len) {
    if (free_pos + len > length) {
        scale_to(len);
    }

    memcpy(header+free_pos, src, len);
    increase_free_bytes(len);
}

int Buffer::take(char *dest, int len) {
    int actual_len = free_pos - used_pos;
    if (actual_len > len) {
        actual_len = len;
    }
    memcpy(dest, header+used_pos, actual_len);
    increase_used_bytes(actual_len);
    return actual_len;
}

int Buffer::get_upper_len(int len) {
    int base = INIT_BUFFER_LENGTH;
    while (base <= len) {
        base = (base << 0x1);
    }
    return base;
}

}
