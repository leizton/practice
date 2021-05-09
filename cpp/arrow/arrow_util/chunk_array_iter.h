#pragma once

#include <bits/functional_hash.h>

#include <arrow/table.h>

template <class T>
class ChunkArrayIter {
public:
  explicit ChunkArrayIter(std::shared_ptr<arrow::ChunkedArray> chunk_arr, int field_offset = 0)
      : chunk_arr_(chunk_arr)
      , chunk_num_((chunk_arr == nullptr ? 0 : chunk_arr->num_chunks()))
      , field_offset_(field_offset) {
    reset();
  }

  void reset() {
    chunk_i_ = -1;
    gotoNextArray();
  }

  bool not_end() const {
    return 0 <= data_i_ && data_i_ < curr_arr_len_;
  }

  T& data() {
    return *(data_raw_ + data_i_);
  }

  T& next() {
    T& d = data();
    if (!gotoNextValidData()) {
      gotoNextArray();
    }
    return d;
  }

private:
  void gotoNextArray() {
    resetCurrArray();
    while (++chunk_i_ < chunk_num_) {
      setCurrArray();
      if (gotoNextValidData()) return;
      resetCurrArray();
    }
  }

  void resetCurrArray() {
    curr_arr_ = nullptr;
    curr_arr_len_ = 0;
    data_raw_ = nullptr;
    data_i_ = -1;
  }

  void setCurrArray() {
    curr_arr_ = chunk_arr_->chunk(chunk_i_).get();
    curr_arr_len_ = curr_arr_->length();
    data_raw_ = curr_arr_len_ > 0
                    ? curr_arr_->data()->GetMutableValues<T>(1, 0)
                    : nullptr;
  }

  bool gotoNextValidData() {
    return ++data_i_ < curr_arr_len_;
  }

private:
  const std::shared_ptr<arrow::ChunkedArray> chunk_arr_;
  const int chunk_num_;
  const int field_offset_;
  arrow::Array* curr_arr_;
  int curr_arr_len_;
  T* data_raw_;
  int chunk_i_;
  int data_i_;
};

struct BinaryData {
  char* data;
  int len;

  BinaryData()
      : data(nullptr)
      , len(0) {}

  BinaryData(char* data_, int len_)
      : data(data_)
      , len(len_) {}

  bool operator==(const BinaryData& rhs) const {
    if (len != rhs.len) return false;
    if (data == rhs.data) return true;
    return strncmp(data, rhs.data, len) == 0;
  }
};

template <>
struct std::hash<BinaryData> {
  std::size_t operator()(const BinaryData& k) const {
    return std::_Hash_impl::hash(k.data, k.len);
  }
};

class BinaryChunkArrayIter {
public:
  explicit BinaryChunkArrayIter(std::shared_ptr<arrow::ChunkedArray> chunk_arr)
      : chunk_arr_(chunk_arr)
      , chunk_num_((chunk_arr == nullptr ? 0 : chunk_arr->num_chunks())) {
    reset();
  }

  void reset() {
    chunk_i_ = -1;
    gotoNextArray();
  }

  bool not_end() const {
    return 0 <= data_i_ && data_i_ < curr_arr_len_;
  }

  BinaryData data() {
    int32_t pos = offset_raw_[data_i_];
    return BinaryData(data_raw_ + pos, offset_raw_[data_i_ + 1] - pos);
  }

  BinaryData next() {
    BinaryData d = data();
    if (!gotoNextValidData()) {
      gotoNextArray();
    }
    return d;
  }

private:
  void gotoNextArray() {
    resetCurrArray();
    while (++chunk_i_ < chunk_num_) {
      setCurrArray();
      if (gotoNextValidData()) return;
      resetCurrArray();
    }
  }

  void resetCurrArray() {
    curr_arr_ = nullptr;
    curr_arr_len_ = 0;
    offset_raw_ = nullptr;
    data_raw_ = nullptr;
    data_i_ = -1;
  }

  void setCurrArray() {
    curr_arr_ = chunk_arr_->chunk(chunk_i_).get();
    curr_arr_len_ = curr_arr_->length();
    offset_raw_ = curr_arr_len_ > 0
                      ? curr_arr_->data()->GetMutableValues<int32_t>(1, 0)
                      : nullptr;
    data_raw_ = curr_arr_len_ > 0
                    ? curr_arr_->data()->GetMutableValues<char>(2, 0)
                    : nullptr;
  }

  bool gotoNextValidData() {
    return ++data_i_ < curr_arr_len_;
  }

private:
  std::shared_ptr<arrow::ChunkedArray> chunk_arr_;
  const int chunk_num_;
  arrow::Array* curr_arr_;
  int curr_arr_len_;
  int32_t* offset_raw_;
  char* data_raw_;
  int chunk_i_;
  int data_i_;
};
