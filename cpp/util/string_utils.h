#pragma once

#include <string>
#include <vector>

bool str2int(const std::string& s, int& v);
bool str2int64(const std::string& s, int64_t& v);
bool str2uint64(const std::string& s, uint64_t& v);
bool str2float(const std::string& s, float& v);
bool str2double(const std::string& s, double& v);

std::string double2Str(double d, int precision = 4);

bool startswith(const std::string& s, const std::string& prefix);

void splitString(const std::string& str, const std::string& delimiter,
                 std::vector<std::string>& out);

int cstr2int(const char* s, int begin, int* end = nullptr);
