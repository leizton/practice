#include "serialize_utils.h"
#include <string.h>

int SerializeUtils::SerializeToString(const SerializeApi& obj, std::string& output) {
  int output_size = obj.ByteSize();
  output.clear();
  output.resize(output_size);
  int ser_size = Serialize(obj, (uint8_t*)output.data(), 0);
  return (ser_size == output_size) ? output_size : -1;
}

int SerializeUtils::DeserializeFromString(SerializeApi& obj, const std::string& input) {
  DeserializeClear(obj);
  int input_size = (int)input.length();
  int deser_size = Deserialize(obj, (uint8_t*)input.data(), input_size, 0);
  return (deser_size == input_size) ? input_size : -1;
}

int SerializeUtils::ByteSize(const SerializeApi& obj) {
  return obj.ByteSize();
}

int SerializeUtils::Serialize(const SerializeApi& obj, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  int sz = obj.ByteSize();
  int next_offset = offset + sz;
  int ser_offset = obj.Serialize(output, offset);
  return (next_offset == ser_offset) ? next_offset : -1;
}

int SerializeUtils::Deserialize(SerializeApi& obj, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  return obj.Deserialize(input, input_size, offset);
}

int SerializeUtils::ByteSize(bool i) {
  return 1;
}

int SerializeUtils::Serialize(bool i, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  output[offset] = uint8_t(i);
  return offset + 1;
}

int SerializeUtils::Deserialize(bool& i, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  if (input_size < offset + ByteSize(i)) return -1;
  i = input[offset] != 0u;
  return offset + 1;
}

int SerializeUtils::ByteSize(int16_t i) {
  return 2;
}

int SerializeUtils::Serialize(int16_t i, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  output[offset] = uint8_t(i & 0xFF);
  output[offset + 1] = uint8_t((i >> 8) & 0xFF);
  return offset + 2;
}

int SerializeUtils::Deserialize(int16_t& i, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  i = int16_t((uint8_t(input[offset]) & 0xFF));
  i |= int16_t((uint8_t(input[offset + 1]) & 0xFF)) << 8;
  return offset + 2;
}

int SerializeUtils::ByteSize(uint16_t i) {
  return 2;
}

int SerializeUtils::Serialize(uint16_t i, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  output[offset] = uint8_t(i & 0xFF);
  output[offset + 1] = uint8_t((i >> 8) & 0xFF);
  return offset + 2;
}

int SerializeUtils::Deserialize(uint16_t& i, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  i = uint16_t((uint8_t(input[offset]) & 0xFF));
  i |= uint16_t((uint8_t(input[offset + 1]) & 0xFF)) << 8;
  return offset + 2;
}

int SerializeUtils::ByteSize(int32_t i) {
  return 4;
}

int SerializeUtils::Serialize(int32_t i, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  output[offset] = uint8_t(i & 0xFF);
  output[offset + 1] = uint8_t((i >> 8) & 0xFF);
  output[offset + 2] = uint8_t((i >> 16) & 0xFF);
  output[offset + 3] = uint8_t((i >> 24) & 0xFF);
  return offset + 4;
}

int SerializeUtils::Deserialize(int32_t& i, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  i = int32_t((uint8_t(input[offset]) & 0xFF));
  i |= int32_t((uint8_t(input[offset + 1]) & 0xFF)) << 8;
  i |= int32_t((uint8_t(input[offset + 2]) & 0xFF)) << 16;
  i |= int32_t((uint8_t(input[offset + 3]) & 0xFF)) << 24;
  return offset + 4;
}

int SerializeUtils::ByteSize(uint32_t i) {
  return 4;
}

int SerializeUtils::Serialize(uint32_t i, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  output[offset] = uint8_t(i & 0xFF);
  output[offset + 1] = uint8_t((i >> 8) & 0xFF);
  output[offset + 2] = uint8_t((i >> 16) & 0xFF);
  output[offset + 3] = uint8_t((i >> 24) & 0xFF);
  return offset + 4;
}

int SerializeUtils::Deserialize(uint32_t& i, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  i = uint32_t((uint8_t(input[offset]) & 0xFF));
  i |= uint32_t((uint8_t(input[offset + 1]) & 0xFF)) << 8;
  i |= uint32_t((uint8_t(input[offset + 2]) & 0xFF)) << 16;
  i |= uint32_t((uint8_t(input[offset + 3]) & 0xFF)) << 24;
  return offset + 4;
}

int SerializeUtils::ByteSize(int64_t i) {
  return 8;
}

int SerializeUtils::Serialize(int64_t i, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  output[offset] = uint8_t(i & 0xFF);
  output[offset + 1] = uint8_t((i >> 8) & 0xFF);
  output[offset + 2] = uint8_t((i >> 16) & 0xFF);
  output[offset + 3] = uint8_t((i >> 24) & 0xFF);
  output[offset + 4] = uint8_t((i >> 32) & 0xFF);
  output[offset + 5] = uint8_t((i >> 40) & 0xFF);
  output[offset + 6] = uint8_t((i >> 48) & 0xFF);
  output[offset + 7] = uint8_t((i >> 56) & 0xFF);
  return offset + 8;
}

int SerializeUtils::Deserialize(int64_t& i, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  i = int64_t((uint8_t(input[offset]) & 0xFF));
  i |= int64_t((uint8_t(input[offset + 1]) & 0xFF)) << 8;
  i |= int64_t((uint8_t(input[offset + 2]) & 0xFF)) << 16;
  i |= int64_t((uint8_t(input[offset + 3]) & 0xFF)) << 24;
  i |= int64_t((uint8_t(input[offset + 4]) & 0xFF)) << 32;
  i |= int64_t((uint8_t(input[offset + 5]) & 0xFF)) << 40;
  i |= int64_t((uint8_t(input[offset + 6]) & 0xFF)) << 48;
  i |= int64_t((uint8_t(input[offset + 7]) & 0xFF)) << 56;
  return offset + 8;
}

int SerializeUtils::ByteSize(uint64_t i) {
  return 8;
}

int SerializeUtils::Serialize(uint64_t i, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  output[offset] = uint8_t(i & 0xFF);
  output[offset + 1] = uint8_t((i >> 8) & 0xFF);
  output[offset + 2] = uint8_t((i >> 16) & 0xFF);
  output[offset + 3] = uint8_t((i >> 24) & 0xFF);
  output[offset + 4] = uint8_t((i >> 32) & 0xFF);
  output[offset + 5] = uint8_t((i >> 40) & 0xFF);
  output[offset + 6] = uint8_t((i >> 48) & 0xFF);
  output[offset + 7] = uint8_t((i >> 56) & 0xFF);
  return offset + 8;
}

int SerializeUtils::Deserialize(uint64_t& i, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  i = uint64_t((uint8_t(input[offset]) & 0xFF));
  i |= uint64_t((uint8_t(input[offset + 1]) & 0xFF)) << 8;
  i |= uint64_t((uint8_t(input[offset + 2]) & 0xFF)) << 16;
  i |= uint64_t((uint8_t(input[offset + 3]) & 0xFF)) << 24;
  i |= uint64_t((uint8_t(input[offset + 4]) & 0xFF)) << 32;
  i |= uint64_t((uint8_t(input[offset + 5]) & 0xFF)) << 40;
  i |= uint64_t((uint8_t(input[offset + 6]) & 0xFF)) << 48;
  i |= uint64_t((uint8_t(input[offset + 7]) & 0xFF)) << 56;
  return offset + 8;
}

int SerializeUtils::ByteSize(float d) {
  return 4;
}

int SerializeUtils::Serialize(float d, uint8_t* output, int offset) {
  void* p = &d;
  uint32_t i = *reinterpret_cast<uint32_t*>(p);
  return Serialize(i, output, offset);
}

int SerializeUtils::Deserialize(float& d, const uint8_t* input, const int input_size, int offset) {
  void* p = &d;
  uint32_t* i = reinterpret_cast<uint32_t*>(p);
  return Deserialize(*i, input, input_size, offset);
}

int SerializeUtils::ByteSize(double d) {
  return 8;
}

int SerializeUtils::Serialize(double d, uint8_t* output, int offset) {
  void* p = &d;
  uint64_t i = *reinterpret_cast<uint64_t*>(p);
  return Serialize(i, output, offset);
}

int SerializeUtils::Deserialize(double& d, const uint8_t* input, const int input_size, int offset) {
  void* p = &d;
  uint64_t* i = reinterpret_cast<uint64_t*>(p);
  return Deserialize(*i, input, input_size, offset);
}

int SerializeUtils::ByteSize(const std::string& s) {
  uint32_t len = s.length();
  return ByteSize(len) + int(len);
}

int SerializeUtils::Serialize(const std::string& s, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  uint32_t len = s.length();
  offset = Serialize(len, output, offset);
  memcpy(output + offset, s.data(), len);
  return offset + int(len);
}

int SerializeUtils::Deserialize(std::string& s, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  s.clear();
  uint32_t len = 0u;
  offset = Deserialize(len, input, input_size, offset);
  if (offset < 0 || len == 0u) return offset;
  int next_offset = offset + int(len);
  if (input_size < next_offset) return -1;
  s.reserve(len);
  s.assign(input + offset, input + next_offset);
  return next_offset;
}

/*
int SerializeUtils::ByteSize(const ::google::protobuf::Message& msg) {
  uint32_t len = uint32_t(msg.ByteSize());
  return ByteSize(len) + int(len);
}

int SerializeUtils::Serialize(const ::google::protobuf::Message& msg, uint8_t* output, int offset) {
  if (offset < 0) return -1;
  uint32_t len = uint32_t(msg.ByteSize());
  offset = Serialize(len, output, offset);
  if (len == 0u) return offset;
  if (!msg.SerializeToArray(output + offset, int(len))) return -1;
  return offset + int(len);
}

int SerializeUtils::Deserialize(::google::protobuf::Message& msg, const uint8_t* input, const int input_size, int offset) {
  if (offset < 0) return -1;
  msg.Clear();
  uint32_t len = 0u;
  offset = Deserialize(len, input, input_size, offset);
  if (offset < 0 || len == 0u) return offset;
  int next_offset = offset + int(len);
  if (input_size < next_offset) return -1;
  if (!msg.ParseFromArray(input + offset, int(len))) return -1;
  return next_offset;
}
*/
