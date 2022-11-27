#include <header>

// 满足 a[begin:expect_pivot-1] <= a[expect_pivot], begin<=expect_pivot<end
void qpart(vector<int>& a, int begin, int end, int expect_pivot) {
  if (end-begin <= 4) {
    std::sort(a.begin()+begin, a.begin()+end);
    return;
  }
  int pivot, p, q;
  //
  pivot = a[begin]; p = a[begin+1]; q = a[end-1];
  if (pivot < p) {
    if (p <= q) {
      a[begin+1] = pivot; a[begin] = p;
    } else if (pivot < q) {
      a[begin+1] = pivot; a[begin] = q; a[end-1] = p;
    } else {
      a[begin+1] = q; a[end-1] = p;
    }
  } else {
    if (pivot <= q) {
    } else if (p <= q) {
      a[begin] = q; a[end-1] = pivot;
    } else {
      a[begin+1] = q; a[begin] = p; a[end-1] = pivot;
    }
  }
  //
  pivot = a[begin]; p = begin+1; q = end-1;
  while (true) {
    while (p < end && a[p] < pivot) p++;  // a[begin+1:p-1] < pivot, begin+1<=p<=end
    while (q > p && a[q] >= pivot) q--;   // a[q+1:end-1] >= pivot, p<=q<=end-1
    if (p == end) {
      std::swap(a[end-1], a[begin]);
      if (expect_pivot < end-1) qpart(a, begin, end-1, expect_pivot);
      return;
    }
    if (p < q) {
      std::swap(a[p], a[q]);
      p++, q--;
    }
    if (q <= p) {
      if (pivot <= a[p]) p--;
      std::swap(a[p], a[begin]);
      if (p < expect_pivot) qpart(a, p+1, end, expect_pivot);
      if (expect_pivot < p) qpart(a, begin, p, expect_pivot);
      return;
    }
  }
}
vector<double> medianSlidingWindow(vector<int>& nums, int k) {
  const int n = nums.size();
  if (n == 0) return vector<double>();
  k = std::min(k, n);
  bool is_odd = (k&1) != 0;
  int mid = is_odd ? (k/2) : (k/2-1);

  vector<double> res(n-k+1, 0);
  vector<int> wnd(k, 0);
  for (int i = 0; i < (int)res.size(); i++) {
    wnd.assign(nums.begin()+i, nums.begin()+i+k);
    qpart(wnd, 0, k, mid);
    if (is_odd) {
      res[i] = wnd[mid];
    } else {
      res[i] = wnd[mid];
      int min_v = wnd[mid+1];
      for (int j = mid+2; j < k; j++) {
        if (wnd[j] < min_v) min_v = wnd[j];
      }
      res[i] += min_v;
      res[i] *= 0.5;
    }
  }
  return res;
}

int main() {
  vector<int> nums{5,2,2,7,3,7,9,0,2,3};
  print(medianSlidingWindow(nums, 9));
}
