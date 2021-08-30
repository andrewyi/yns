#ifndef _SRC_UTIL_SINGLETON_H_
#define _SRC_UTIL_SINGLETON_H_

#include "src/util/exception.h"

#include <unistd.h>
#include <pthread.h>

#include <memory>

namespace yns {

template<typename T>
class Singleton {
    public:
        Singleton();
        ~Singleton();

        static T& instance();
        static void destroy();
        static void set_initializer(std::shared_ptr<T> (*i)());

    protected:
        static void init_instance();
        static std::shared_ptr<T> default_initializer();

    protected:
        static pthread_once_t once;
        static std::shared_ptr<T> p;
        static std::shared_ptr<T> (*initializer)();
};

template<typename T>
Singleton<T>::Singleton() {}

template<typename T>
Singleton<T>::~Singleton() {}

template<typename T>
pthread_once_t Singleton<T>::once = PTHREAD_ONCE_INIT;

template<typename T>
std::shared_ptr<T> Singleton<T>::p;

template<typename T>
std::shared_ptr<T> (*Singleton<T>::initializer)() = &(Singleton<T>::default_initializer);

template<typename T>
T& Singleton<T>::instance() {
    pthread_once(&once, &init_instance);
    if (!p) { // in case of destroy called
        throw Fatal("instance destroyed");
    }
    return *p;
}

template<typename T>
void Singleton<T>::destroy() {
    p.reset();
}

template<typename T>
void Singleton<T>::set_initializer(std::shared_ptr<T> (*i)()) {
    initializer = i;
}

template<typename T>
void Singleton<T>::init_instance() {
    p = initializer();
}

template<typename T>
std::shared_ptr<T> Singleton<T>::default_initializer() {
    return std::make_shared<T>();
}

}

#endif //_SRC_UTIL_SINGLETON_H_
