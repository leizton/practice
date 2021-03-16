// core/framework/tensor.h
class TensorShape : TensorShapeBase<TensorShape>;

class TensorShapeBase<Shape> : TensorShapeRep;

class TensorShapeRep {
  uint8 buf_[16];
  int64 num_elements_;
  //
  Rep16* as16() = reinterpret_cast<Rep16*>(buf_);
  Rep32* as32() = reinterpret_cast<Rep32*>(buf_);
  // enum class RepTag { REP16, REP32, REP_OUT_OF_LINE }
  RepTag tag() = static_cast<RepTag>(buf_[15]);
  //
  int64 dim_size(int d) {
    if (tag() == RepTag::REP16) return as16()->dims_[d];
    if (tag() == RepTag::REP32) return as32()->dims_[d];
    return (*as64()->dims_)[d];
  }
};
/**
  We use 16 bytes to represent a TensorShape
  the first 12 bytes 是dims数组, the last 4 bytes 记录meta信息
  Because we need to support 64-bit dim sizes and an arbitrary number of dims for a Tensor
  but most tensor dims are smaller than 64-bit, and most tensors are 1,2,3 dims
  so we have several representations
*/
class Rep16 {
  uint16 dims_[6];
};
class Rep32 {
  uint32 dims_[3];
};
class Rep64 {
  gtl::InlinedVector<int64, 4>* dims_;
  // InlinedVector<ValueType, N>是absl库的
  // 当size <= N时, InlinedVector使用本地字段的内存空间char[sizeof(ValueType[N])]
  // 当size > N时, 用堆上的空间, 和std::vector相同
  // gtl <=> stl, g: google
};