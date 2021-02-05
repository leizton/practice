#pragma once

#include <bits/stdc++.h>

/*
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
//*/

using namespace std;

#define reload_os_out(type) friend std::ostream& operator<<(std::ostream& out, const type& obj)

inline uint64_t ptrToInt(const void* p) {
  return reinterpret_cast<uint64_t>(p);
}
