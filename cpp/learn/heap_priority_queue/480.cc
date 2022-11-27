#include <header>

vector<double> medianSlidingWindow(vector<int>& nums, int k) {
  const int n = nums.size();
  if (n == 0) return vector<double>();
  k = std::min(k, n);
  if (k == 1) {
    vector<double> res(n, 0);
    for (int i = 0; i < n; i++) res[i] = nums[i];
    return res;
  }

  // 左堆元素都小于等于右堆 && 左堆有效size等于右堆有效size
  using Elem = std::pair<int, int>;  // <nums[i], i>
  std::priority_queue<Elem, vector<Elem>> lheap;  // 左边大根堆
  std::priority_queue<Elem, vector<Elem>, std::greater<Elem>> rheap;  // 右边小根堆

  const bool is_odd = (k&1) != 0;
  const int rheap_target_size = k/2;
  vector<int8_t> which_heap(n, 1);  // 1表示在lheap里, 2表示在rheap里
  for (int i = 0; i < k; i++) {
    lheap.emplace(nums[i], i);
  }
  for (int i = 0; i < rheap_target_size; i++) {
    which_heap[lheap.top().second] = 2;
    rheap.push(lheap.top());
    lheap.pop();
  }

  vector<double> res(n-k+1, 0);
  int res_i = 0;
  if (is_odd) {
    res[res_i++] = lheap.top().first;
  } else {
    res[res_i++] = (double(lheap.top().first) + rheap.top().first) * 0.5;
  }

  for (int last = k; last < n; last++) {
    int delete_pos = last - k;
    while (!lheap.empty() && lheap.top().second <= delete_pos) lheap.pop();
    while (!rheap.empty() && rheap.top().second <= delete_pos) rheap.pop();
    int v = nums[last];
    if (v <= rheap.top().first) {
      lheap.emplace(v, last);
      which_heap[last] = 1;
      if (which_heap[delete_pos] == 2) {
        which_heap[lheap.top().second] = 2;
        rheap.push(lheap.top());
        lheap.pop();
        // lheap pop后新的top可能是待删除元素, 如果不清理res就不正确
        while (!lheap.empty() && lheap.top().second <= delete_pos) lheap.pop();
      }
    } else {
      rheap.emplace(v, last);
      which_heap[last] = 2;
      if (which_heap[delete_pos] == 1) {
        which_heap[rheap.top().second] = 1;
        lheap.push(rheap.top());
        rheap.pop();
      }
      while (!rheap.empty() && rheap.top().second <= delete_pos) rheap.pop();
    }
    //
    if (is_odd) {
      res[res_i++] = lheap.top().first;
    } else {
      res[res_i++] = (double(lheap.top().first) + rheap.top().first) * 0.5;
    }
  }

  return res;
}

int main() {
  // vector<int> nums{5,2,2,7,3,7,9,0,2,3};
  // print(medianSlidingWindow(nums, 9));
  vector<int> nums{7,8,8,3,8,1,5,3,5,4};
  print(medianSlidingWindow(nums, 3));
}
