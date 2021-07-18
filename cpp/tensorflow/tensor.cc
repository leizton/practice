// core/framework/tensor.h
class Tensor {
  TensorShape shape_;
  TensorBuffer* buf_;

  // 提供和proto转换的接口为了可以在进程之间传递tensor
  bool FromProto(const TensorProto& other);
  void AsProtoField(TensorProto* proto);

  T* base<T>() {
    return buf_->base<T>();
  }

  // 转成 Eigen 的 tensor
  TTypes<T, NDIMS>::Tensor tensor<typename T, size_t NDIMS>() {
    return TTypes<T, NDIMS>::Tensor(
        base<T>(),
        shape().AsEigenDSizes<NDIMS>()
    );
  }
};