#include <util/base.h>

template <typename RandomIter>
std::string rangestr(RandomIter begin, RandomIter end) {
  std::ostringstream ss;
  ss << "[";
  for (; begin != end; ++begin) {
    ss << *begin << ",";
  }
  ss.seekp(ss.tellp() - 1);
  ss << "]";
  return ss.str();
}

#define less(x, y) (cmp(*(x), *(y)))

template <typename RandomIter, typename Compare>
void insertion_sort(RandomIter begin, RandomIter end, Compare cmp) {
  using Value = typename std::iterator_traits<RandomIter>::value_type;
  RandomIter thre_end = begin + 7;
  if (thre_end > end) thre_end = end;
  for (RandomIter i = begin+1; i < thre_end; ++i) {
    if (less(i, i-1) == false) continue;
    RandomIter p = i-2;
    while (p >= begin && less(i, p)) --p;
    ++p;
    Value v = std::move(*i);
    for (RandomIter j = i-1; j >= p; --j) *(j+1) = std::move(*j);
    *p = std::move(v);
  }
  for (RandomIter i = thre_end; i < end; ++i) {
    if (less(i, i-1) == false) continue;
    RandomIter p = std::upper_bound(begin, i-1, *i);  // p ∈ [begin, i-1]
    Value v = std::move(*i);
    for (RandomIter j = i-1; j >= p; --j) *(j+1) = std::move(*j);
    *p = std::move(v);
  }
}

template <typename RandomIter, typename Compare>
void move_median_to_begin(RandomIter begin, RandomIter a, RandomIter b, RandomIter c, Compare& cmp) {
  RandomIter median;
  if (less(a, b)) {
    if (less(b, c))       median = b;
    else if (less(a, c))  median = c;
    else                  median = a;
  } else {
    if (less(a, c))       median = a;
    else if (less(b, c))  median = c;
    else                  median = b;
  }
  std::iter_swap(begin, median);
}

template <typename RandomIter, typename Compare>
RandomIter guarded_partition_pivot(RandomIter begin, RandomIter end, Compare& cmp) {
  RandomIter mid = begin + (end-begin)/2;
  // 选中间值作分割依据
  move_median_to_begin(begin, begin+1, mid, end-1, cmp);
  // 待检查区间 [p, q]
  RandomIter p = begin+1, q = end-1;
  while (true) {
    while (p < end && less(p, begin)) ++p;
    while (q > begin && less(begin, q)) --q;
    // 此时  begin<p<=end && begin<=q<end
    if (p >= q) break;
    std::iter_swap(p, q);
    ++p;
    --q;
  }
  // begin<p<=end && begin<=q<end
  // arr[begin,p-1] <= *begin <= arr[q+1,end]
  return p;
}

// 不检查边界的分割函数
template <typename RandomIter, typename Compare>
RandomIter unguarded_partition_pivot(RandomIter begin, RandomIter end, Compare& cmp) {
  RandomIter mid = begin + (end-begin)/2;
  // 选中间值作分割依据
  move_median_to_begin(begin, begin+1, mid, end-1, cmp);
  // 待检查区间 [p, q]
  RandomIter p = begin+1, q = end-1;
  print(rangestr(begin, end));
  while (true) {
    /*
       和 guarded_partition_pivot() 区别就是移动 p q 时没有检查边界 *************************************************************** look here !!!
       如果less不是严格小于
         当arr[begin+1,end)<=begin时  p会超出end-1继续往后读  因为(begin+1, mid, end-1)三者都不能拦住p
         当arr[begin,end-1]>=begin时  q会超出begin继续往前读  原因同上
    */
    while (less(p, begin)) ++p;
    if (p > end) print("overflow p", (p-end), *p);      // 埋点检查
    while (less(begin, q)) --q;
    if (q < begin) print("overflow q", (q-begin), *q);  // 埋点检查
    if (p >= q) break;
    // 此时可能 *p 或 *q 等于 *begin, 交换后就不是稳定排序 ************************************************************************* look here !!!
    std::iter_swap(p, q);
    ++p;
    --q;
  }
  // 由于返回p, 所以p溢出会引起越界读写, q溢出不会********************************************************************************** look here !!!
  return p;
}

template <typename RandomIter, typename Compare>
using PartitionPivotFn = std::function<RandomIter(RandomIter, RandomIter, Compare&)>;

static void* g_partition_pivot_fn = nullptr;

static const int g_introsort_loop_thre = 16;

template <typename RandomIter, typename Compare>
void introsort_loop(RandomIter begin, RandomIter end, int depth_limit, Compare& cmp) {
  auto& partition_pivot_fn = *(PartitionPivotFn<RandomIter, Compare>*)g_partition_pivot_fn;
  while (end - begin > g_introsort_loop_thre) {
    if (depth_limit <= 0) {
      // 递归较深转成堆排序
      std::make_heap(begin, end, cmp);
      std::sort_heap(begin, end, cmp);
      return;
    }
    RandomIter pivot = partition_pivot_fn(begin, end, cmp);
    // 递归排右边 [pivot, end)
    introsort_loop(pivot, end, --depth_limit, cmp);
    // 接着排左边 [begin, pivot)
    end = pivot;
  }
}

template <typename RandomIter, typename Compare>
void gcc_sort(RandomIter begin, RandomIter end, Compare cmp) {
  if (begin == end) return;
  // auto less = [&cmp] (RandomIter x, RandomIter y) { return (bool)cmp(*x, *y); };

  // 内观排序 在快排的基础上，当递归深度较深时转成堆排序，使最坏时间复杂度是O(n·logn)
  // 1997 David Musser设计
  // std::__lg(x) = std::floor(std::log2(x))
  introsort_loop(begin, end, std::__lg(end-begin) * 2, cmp);

  insertion_sort(begin, end, cmp);
}

int main() {
  using Container = vector<int>;
  using RandomIter = Container::iterator;
  using Compare = std::function<bool(int,int)>;
  using PPF = PartitionPivotFn<RandomIter, Compare>;
  Compare cmp = [] (int x, int y) { return x <= y; };
  PPF unguarded_partition_pivot_fn = unguarded_partition_pivot<RandomIter, Compare>;
  PPF guarded_partition_pivot_fn = guarded_partition_pivot<RandomIter, Compare>;
  int n = g_introsort_loop_thre + 5;
  int begin = 2, end = n-2;
  Container a(n, 0);

  // 复现 p 超出 end-1
  std::fill(a.begin(), a.end(), 1);
  a[0] = a[n-1] = 9;  // 哨兵 避免core
  a[1] = a[n-2] = 0;
  a[begin] = a[begin+1] = a[begin+(end-begin)/2] = a[end-1] = 3;
  print(a);
  print("-----");
  g_partition_pivot_fn = &unguarded_partition_pivot_fn;
  gcc_sort(a.begin()+begin, a.begin()+end, cmp);
  print("-----");
  print(a);
  print("\n");

  // 复现 q 超出 begin
  std::fill(a.begin(), a.end(), 7);
  a[0] = a[n-1] = 9;  // 哨兵 避免core
  a[1] = a[n-2] = 0;
  a[begin] = a[begin+1] = a[begin+(end-begin)/2] = a[end-1] = 3;
  print(a);
  print("-----");
  g_partition_pivot_fn = &unguarded_partition_pivot_fn;
  gcc_sort(a.begin()+begin, a.begin()+end, cmp);
  print("-----");
  print(a);
  print("\n");

  begin += 3;
  end += 3;

  // std::sort p溢出引起越界读写
  n = g_introsort_loop_thre + 15;
  a.resize(n);
  std::fill(a.begin(), a.end(), 1);
  a[begin] = a[begin+1] = a[begin+(end-begin)/2] = a[end-1] = 3;
  for (int i = 0; i < begin; i++) a[i] = 0;
  for (int i = end; i < n-1; i++) a[i] = 0;
  a[0] = a[n-1] = 9;
  print("std::sort overflow p bad");
  print(a);
  print("-----");
  print(rangestr(a.begin()+begin, a.begin()+end));
  std::sort(a.begin()+begin, a.begin()+end, cmp);
  print(rangestr(a.begin()+begin, a.begin()+end));
  print("-----");
  print(a);
  print("\n");

  // std::sort q溢出不会引起越界访问
  n = g_introsort_loop_thre + 11;
  a.resize(n);
  std::fill(a.begin(), a.end(), 7);
  a[begin] = a[begin+1] = a[begin+(end-begin)/2] = a[end-1] = 3;
  for (int i = 0; i < begin; i++) a[i] = 0;
  for (int i = end; i < n-1; i++) a[i] = 0;
  a[0] = a[n-1] = 9;
  print("std::sort overflow q good");
  print(a);
  print("-----");
  print(rangestr(a.begin()+begin, a.begin()+end));
  std::sort(a.begin()+begin, a.begin()+end, cmp);
  print(rangestr(a.begin()+begin, a.begin()+end));
  print("-----");
  print(a);
  print("\n");
}
