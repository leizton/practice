// core/framework/tensor.h
class Tensor {
  TensorShape shape_;
  TensorBuffer* buf_;

  // 提供和proto转换的接口为了可以在进程之间传递tensor
  bool FromProto(const TensorProto& other);
  void AsProtoField(TensorProto* proto);
};