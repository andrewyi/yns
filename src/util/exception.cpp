#include "src/util/exception.h"

namespace yns {

Exception::Exception(std::string s):msg(s) {}
Exception::~Exception(){}
std::string Exception::desc() { return msg; }

Fatal::Fatal(std::string s):Exception(s) {}
Fatal::~Fatal(){}

}
