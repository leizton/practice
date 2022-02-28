#pragma once

#include "util/cpp_lib.h"

bool str2int(const std::string& s, int& v);
bool str2int64(const std::string& s, int64_t& v);
bool str2uint64(const std::string& s, uint64_t& v);
bool str2float(const std::string& s, float& v);
bool str2double(const std::string& s, double& v);

std::string double2Str(double d, int precision = 4);
