namespace base64 {

int encode(const void* src, const int src_n, void* dst);
int decode(const void* src, const int src_n, void* dst);

int getEncodedSize(int n);
int getDecodedSize(int n);

}  // namespace base64
