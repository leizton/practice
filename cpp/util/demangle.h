#pragma once

#include <string>

// c++filt 解码
// @ref glog/src/demangle.cc::Demangle
bool demangle(const char *mangled, char *out, int out_size);

std::string demangleFromFile(const int fd, uint64_t offset);
