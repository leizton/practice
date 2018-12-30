#include "hello_util.h"

namespace hello_util {

void HelloUtil::Greet(const std::string& name, std::string& out) {
  out = "hello, " + name;
}

}
