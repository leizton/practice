#include <header>

/*
207.cc
课程依赖关系数组 prerequisites
prerequisites的每个元素是size=2的vector<int>{x, y}，表示 课程x 依赖 课程y
学习时间上是先y后x，即存在边 y->x
如果出现 x1->x2->x3->x1 环形，说明课程安排有问题
课程数量 course_num
0 <= x < course_num
*/
bool hasCircle(vector<vector<int>>& edges, int curr, vector<bool>& checked, vector<bool>& go_mark) {
  if (checked[curr]) return false;
  checked[curr] = true;
  go_mark[curr] = true;  // 标记路过
  for (auto to_node : edges[curr]) {
    if (go_mark[to_node]) return true;
    if (hasCircle(edges, to_node, checked, go_mark)) return true;
  }
  go_mark[curr] = false;  // 解除路过标记
  return false;
}
bool canFinish(int course_num, vector<vector<int>>& prerequisites) {
  vector<vector<int>> edges(course_num, vector<int>{});
  for (auto& e : prerequisites) {
    edges[e[1]].push_back(e[0]);
  }
  //
  vector<bool> checked(course_num, false);
  vector<bool> go_mark(course_num, false);
  bool succ = true;
  for (int i = 0; i < course_num; i++) {
    if (checked[i]) continue;
    if (hasCircle(edges, i, checked, go_mark)) {
      succ = false;
      break;
    }
  }
  return succ;
}

/*
210.cc
在207的基础上给出一个可行的学习课程的顺序
拓扑排序
*/
vector<int> findOrderV0(int course_num, vector<vector<int>>& prerequisites) {
  vector<vector<int>> edges(course_num, vector<int>{});
  vector<int> dep_nums(course_num, 0);  // 依赖数量
  for (auto& e : prerequisites) {
    edges[e[1]].push_back(e[0]);
    dep_nums[e[0]]++;
  }
  //
  vector<int> res;
  res.reserve(course_num);
  int add_num = -1;
  while (add_num != 0) {
    add_num = 0;
    for (int i = 0; i < course_num; i++) {
      if (dep_nums[i] == 0) {
        dep_nums[i] = -1;
        res.push_back(i);
        add_num++;
        for (int j : edges[i]) {
          dep_nums[j]--;
        }
      }
    }
  }
  if ((int)res.size() != course_num) res.clear();
  return res;
}
// 把 V0 版本每次遍历 dep_nums 换成队列后速度提升75%
vector<int> findOrderV1(int course_num, vector<vector<int>>& prerequisites) {
  vector<vector<int>> edges(course_num, vector<int>{});
  vector<int> dep_nums(course_num, 0);  // 依赖数量
  for (auto& e : prerequisites) {
    edges[e[1]].push_back(e[0]);
    dep_nums[e[0]]++;
  }
  //
  vector<int> res;
  res.reserve(course_num);
  queue<int> que;
  for (int i = 0; i < course_num; i++) {
    if (dep_nums[i] == 0) que.push(i);
  }
  while (!que.empty()) {
    int course = que.front();
    que.pop();
    res.push_back(course);
    for (int i : edges[course]) {
      if (--dep_nums[i] == 0) que.push(i);
    }
  }
  if ((int)res.size() != course_num) res.clear();
  return res;
}
vector<int> findOrder(int course_num, vector<vector<int>>& prerequisites) {
  vector<vector<int>> edges(course_num, vector<int>{});
  vector<int> dep_nums(course_num, 0);  // 依赖数量
  for (auto& e : prerequisites) {
    edges[e[1]].push_back(e[0]);
    dep_nums[e[0]]++;
  }
  //
  vector<int> res;
  res.reserve(course_num);
  for (int i = 0; i < course_num; i++) {
    if (dep_nums[i] == 0) res.push_back(i);
  }
  int begin = 0, end = res.size();
  while (begin < end) {
    for (int i = begin; i < end; i++) {
      for (int j : edges[res[i]]) {
        if (--dep_nums[j] == 0) res.push_back(j);
      }
    }
    begin = end;
    end = res.size();
  }
  if ((int)res.size() != course_num) res.clear();
  return res;
}

int main() {
  auto testCanFinish = [] (int nc, vector<vector<int>> pr) {
    return canFinish(nc, pr);
  };
  auto testFindOrder = [] (int nc, vector<vector<int>> pr) {
    return findOrder(nc, pr);
  };
  assert_T(testCanFinish(5, {{1,4},{2,4},{3,1},{3,2}}));
  print(testFindOrder(5, {{1,4},{2,4},{3,1},{3,2}}));
}
