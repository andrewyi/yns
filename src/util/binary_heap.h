#ifndef _SRC_UTIL_BINARY_HEAP_H_
#define _SRC_UTIL_BINARY_HEAP_H_

#include "src/util/exception.h"

#include <vector>

namespace yns {

class IComparable {
public:
    IComparable();
    ~IComparable();
    virtual bool compare() = 0;
};

template<typename T>
class BinaryHeap {
public:
    BinaryHeap<T>() {}
    ~BinaryHeap<T>() {}

    int size() {
        return list.size();
    }

    void put(T t) {
        list.push_back(t);
        reheap_tail();
    }

    bool empty() {
        return list.empty();
    }

    T front() {
        if ( empty() ) {
            throw Fatal("take from empty container");
        }

        return list.front();
    }

    T take() {
        if ( empty() ) {
            throw Fatal("take from empty container");
        }

        T head = list.front();

        int len = size();
        if (len == 1) {
            list.pop_back();
        } else {
            list[0] = list[len-1];
            list.pop_back();

            if (len-1 > 1) {
                reheap_head(0, len-1);
            }
        }

        return head;
    }

protected:
    bool reheap_head(int cur_ind, int len) {
        if ( len == 0 ) {
            throw Fatal("reheap of empty container");
        }

        bool is_changed = false;

        while (true) {
            int left_ind = 2 * cur_ind + 1;
            int right_ind = 2 * cur_ind + 2;
            if (left_ind > len-1) {
                break;
            }

            int to_swap_ind = cur_ind;
            if (list[left_ind] < list[to_swap_ind]) {
                to_swap_ind = left_ind;
            }

            if (right_ind < len && list[right_ind] < list[to_swap_ind]) {
                to_swap_ind = right_ind;
            }

            if (to_swap_ind == cur_ind) {
                break;

            } else {
                is_changed = true;

                T tmp = list[to_swap_ind];
                list[to_swap_ind] = list[cur_ind];
                list[cur_ind] = tmp;

                cur_ind = to_swap_ind;
            }
        }

        return is_changed;
    }

    void reheap_tail() {
        int len = size();
        if (len == 1) {
            return;
        }

        int cur_index = len-1;
        while (true) {
            int parent = (cur_index-1) / 2;
            if (reheap_head(parent, len) && parent != 0) {
                cur_index = parent;
            } else {
                break;
            }
        }
    }

protected:
    std::vector<T> list;
};

}

#endif //_SRC_UTIL_BINARY_HEAP_H_
