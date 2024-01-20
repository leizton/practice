~~~c++

// c10/core/TensorImpl.h
struct TensorImpl {
protected:
  Storage storage_;
  SizesAndStrides sizes_and_strides_;
  int64 stroage_offset_;
  int64 numel_;
  bool is_contiguous_;
  bool is_channels_last_;
  bool is_channels_last_contiguous_;
  bool is_channels_last_3d_;
  bool is_channels_last_3d_contiguous_;

  IntArrayRef strides() const {
    return sizes_and_strides_.strides_arrayref();  // sizes_and_strides_内部用一个int64变长数组存各维度的size
  }

private:
  unique_ptr<c10::AutogradMetaInterface> autograd_meta_;
};

// c10/core/Storage.h
struct Storage {
protected:
  shared_ptr<StorageImpl> storage_impl_;
};

// c10/core/StorageImpl.h
struct StorageImpl {
private:
  DataPtr data_ptr_;
};

// c10/core/Allocator.h
class DataPtr {
private:
  VoidPtr ptr_;
  Device device_;
};

~~~