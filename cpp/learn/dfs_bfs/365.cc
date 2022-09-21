#include <header>

/*
有两个水壶，容量分别为 jug1Capacity 和 jug2Capacity 升。水的供应是无限的。确定是否有可能使用这两个壶准确得到 targetCapacity 升。
如果可以得到 targetCapacity 升水，最后请用以上水壶中的一或两个来盛放取得的 targetCapacity 升水。

你可以：
  operator-full, 装满任意一个水壶
  operator-clear, 清空任意一个水壶
  operator-dump, 从一个水壶向另外一个水壶倒水，直到[倒空水壶]或者[装满另一个水壶]
*/

/*
  两个水壶 a b, 三种操作 full clear dump
  所以经过组合每次有6种选择 a-full a-clear a-dump b-full b-clear b-dump
  所以是在一个八叉树上遍历, 但需要记录已经遍历过的状态, 避免死循环
*/

struct Cap {
  int a;
  int b;
  int target;
};
// a: 当前a水壶的水量, b: 当前b水壶的水量
bool dfs(const Cap& cap, int a, int b, std::unordered_set<uint64_t>& state) {
  uint64_t st = (uint64_t(a) << 32) + b;
  if (state.insert(st).second == false) return false;
  //
  if (a == cap.target || b == cap.target || (a + b) == cap.target) return true;
  // a-full
  if (a < cap.a && dfs(cap, cap.a, b, state)) return true;
  // a-clear
  if (b != 0 && dfs(cap, 0, b, state)) return true;
  // a-dump
  if (0 < a && b < cap.b) {
    int dump_sz = std::min(a, cap.b - b);
    if (dfs(cap, a - dump_sz, b + dump_sz, state)) return true;
  }
  // b-full
  if (b < cap.b && dfs(cap, a, cap.b, state)) return true;
  // b-clear
  if (a != 0 && dfs(cap, a, 0, state)) return true;
  // b-dump
  if (0 < b && a < cap.a) {
    int dump_sz = std::min(b, cap.a - a);
    if (dfs(cap, a + dump_sz, b - dump_sz, state)) return true;
  }
  return false;
}
bool canMeasureWater_dfs(int a_cap, int b_cap, int target) {
  if (target <= 0) return true;
  if (a_cap + b_cap < target) return false;
  if (target == a_cap || target == b_cap || target == (a_cap + b_cap)) return true;
  if (a_cap <= 0 || b_cap <= 0) return (target == a_cap || target == b_cap);
  const Cap cap{a_cap, b_cap, target};
  std::unordered_set<uint64_t> state;
  return dfs(cap, 0, 0, state);
}

/*
  bfs 时间和 dfs 差不多
*/
bool canMeasureWater(int a_cap, int b_cap, int target) {
  queue<pair<int,int>> que;
  que.emplace(0, 0);
  std::unordered_set<uint64_t> state;
  while (!que.empty()) {
    int a = que.front().first;
    int b = que.front().second;
    que.pop();
    uint64_t st = (uint64_t(a) << 32) + b;
    if (state.insert(st).second == false) continue;
    if (a == target || b == target || (a + b) == target) return true;
    // a-full
    if (a < a_cap) que.emplace(a_cap, b);
    // a-clear
    if (b != 0) que.emplace(0, b);
    // a-dump
    int dump = std::min(a, b_cap - b);
    if (dump > 0) que.emplace(a-dump, b+dump);
    // b-full
    if (b < b_cap) que.emplace(a, b_cap);
    // b-clear
    if (a != 0) que.emplace(a, 0);
    // b-dump
    dump = std::min(b, a_cap - a);
    if (dump > 0) que.emplace(a+dump, b-dump);
  }
  return false;
}

int main() {
  assert_T(canMeasureWater(22003,31237,1));
}
