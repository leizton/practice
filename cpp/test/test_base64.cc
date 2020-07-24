#include "util/base.h"

void test(const int text_size) {
  const int encoded_size = base64::getEncodedSize(text_size);
  char* text = new char[text_size+1];
  char* encoded = new char[encoded_size+1];
  char* decoded = new char[text_size+1];

  for (int i = 0; i < text_size; i++) {
    text[i] = (char)(i%256);
  }
  text[text_size] = 0;

  assert_eq(encoded_size, base64::encode(text, text_size, encoded));
  encoded[encoded_size] = 0;

  const int decoded_size = base64::getDecodedSize(encoded_size);
  assert_eq(text_size, decoded_size);
  assert_eq(decoded_size, base64::decode(encoded, encoded_size, decoded));

  bool equal = true;
  for (int i = 0; i < text_size; i++) {
    if (decoded[i] != text[i]) {
      equal = false;
      break;
    }
  }
  assert_T(equal);

  delete[] text;
  delete[] encoded;
  delete[] decoded;
  text = encoded = decoded = nullptr;
}

run() {
  for (int i = 0; i < 700; i++) {
    test(3*1024+i);
  }
}

main_run;
