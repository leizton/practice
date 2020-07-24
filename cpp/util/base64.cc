#include <cstdint>
#include <string>

namespace base64 {

uint8_t g_encode_mapping[256];
uint8_t g_decode_mapping[256];
uint8_t g_encode_onebyte_mapping[256];
uint8_t g_decode_onebyte_mapping[256];
const uint8_t g_end_char = '~';

bool g_mapping_inited = []() {
  memset(g_encode_mapping, 255, sizeof(g_encode_mapping));
  memset(g_decode_mapping, 255, sizeof(g_decode_mapping));
  int encodev = 0;
  int j = 0;
  for (int i = 0; i < 10; i++) {
    encodev = g_encode_mapping[j] = '0' + i;
    g_decode_mapping[encodev] = j++;
  }
  for (int i = 0; i < 26; i++) {
    encodev = g_encode_mapping[j] = 'a' + i;
    g_decode_mapping[encodev] = j++;
  }
  for (int i = 0; i < 26; i++) {
    encodev = g_encode_mapping[j] = 'A' + i;
    g_decode_mapping[encodev] = j++;
  }
  encodev = g_encode_mapping[j] = '+';
  g_decode_mapping[encodev] = j++;
  encodev = g_encode_mapping[j] = '-';
  g_decode_mapping[encodev] = j++;

  memset(g_encode_onebyte_mapping, 255, sizeof(g_encode_onebyte_mapping));
  memset(g_decode_onebyte_mapping, 255, sizeof(g_decode_onebyte_mapping));
  j = 0;
  encodev = g_encode_onebyte_mapping[j] = '0';
  g_decode_onebyte_mapping[encodev] = j++;
  encodev = g_encode_onebyte_mapping[j] = '1';
  g_decode_onebyte_mapping[encodev] = j++;
  encodev = g_encode_onebyte_mapping[j] = '2';
  g_decode_onebyte_mapping[encodev] = j++;
  encodev = g_encode_onebyte_mapping[j] = '3';
  g_decode_onebyte_mapping[encodev] = j++;

  return true;
}();

inline void encodeOneByte(uint8_t in, uint8_t* out) {
  *out++ = g_encode_onebyte_mapping[(in>>6) & 0x03];
  *out = g_encode_mapping[in & 0x3F];
}

inline uint8_t decodeOneByte(const uint8_t* in) {
  return ((g_decode_onebyte_mapping[*in] << 6) & 0xC0) | g_decode_mapping[*(in+1)];
}

int encode(const void* src, const int src_n, void* dst) {
  int pack(src_n/3), left(src_n%3);
  const uint8_t* in = (const uint8_t*)src;
  uint8_t* out = (uint8_t*)dst;
  for (int pack_i = 0; pack_i < pack; pack_i++) {
    *out++ = g_encode_mapping[*in & 0x3F];
    uint8_t tmp = (*in++) >> 6;
    *out++ = g_encode_mapping[*in & 0x3F];
    tmp |= ((*in++) >> 4) & 0x0C;
    *out++ = g_encode_mapping[*in & 0x3F];
    tmp |= ((*in++) >> 2) & 0x30;
    *out++ = g_encode_mapping[tmp & 0x3F];
  }
  for (int i = 0; i < left; i++) {
    encodeOneByte(*in++, out);
    out += 2;
  }
  for (int i = 0; i < left; i++) {
    *out++ = g_end_char;
  }
  *out++ = g_end_char;
  return out - (uint8_t*)dst;
}

int decode(const void* src, const int src_n, void* dst) {
  int pack(src_n>>2), left(src_n%4);
  if (left == 2) return -1;
  if (left != 1) pack--;
  const uint8_t* in = (const uint8_t*)src;
  uint8_t* out = (uint8_t*)dst;
  for (int pack_i = 0; pack_i < pack; pack_i++) {
    *out++ = g_decode_mapping[*in++];
    *out++ = g_decode_mapping[*in++];
    *out++ = g_decode_mapping[*in++];
    uint8_t tmp = g_decode_mapping[*in++];
    out -= 3;
    *out++ |= (tmp & 0x03) << 6;
    *out++ |= (tmp & 0x0C) << 4;
    *out++ |= (tmp & 0x30) << 2;
  }
  static constexpr int real_left_mapping[4] = {1, 0, 0, 2};
  left = real_left_mapping[left];
  for (int i = 0; i < left; i++) {
    *out++ = decodeOneByte(in);
    in += 2;
  }
  if ((*in++) != g_end_char) return -2;
  for (int i = 0; i < left; i++) {
    if ((*in++) != g_end_char) return -2;
  }
  return out - (uint8_t*)dst;
}

int getEncodedSize(int n) {
  int pack(n/3), left(n%3);
  return (pack<<2) + (left<<1) + left + 1;
}

int getDecodedSize(int n) {
  static const int mapping[4] = {-2, 0, 0, -1};
  int pack(n>>2), left(n%4);
  return left == 2 ? -1 : ((pack<<1) + pack + mapping[left]);
}

}  // namespace base64
