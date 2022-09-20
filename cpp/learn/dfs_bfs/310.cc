#include <header>

/*
树是一个无向图，其中任何两个顶点只通过一条路径连接
换句话说，一个任何没有简单环路的连通图都是一棵树

给你一棵包含 n 个节点的树，标记为 0 到 n-1
给定数字 n 和一个有 n-1 条无向边的 edges数组, 每一个元素都是一对标签
例如 edges[i] = [ai, bi] 表示树中节点 ai 和 bi 之间存在一条无向边

可选择树中任何一个节点作为根
当选择节点 x 作为根节点时，设结果树的高度为 h
在所有可能的树中，具有最小高度的树被称为`最小高度树`

找到所有的最小高度树
*/

/*
  因为叶节点到达其他节点必须通过非叶节点, 所以应从非叶节点中选择
  每一次迭代淘汰叶节点, 直到剩下节点都是叶节点时终止
  递归的角度看, 每次把最外层节点删除, 剩下内部节点
*/
vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
  vector<int> edge_num(n, 0);
  vector<vector<int>> node_link(n, vector<int>{});
  for (auto& edge : edges) {
    int i = edge[0], j = edge[1];
    edge_num[i]++;
    edge_num[j]++;
    node_link[i].push_back(j);
    node_link[j].push_back(i);
  }

  vector<int> leaf_nodes;
  leaf_nodes.reserve(n);

  while (true) {
    int min_num = INT32_MAX, max_num = INT32_MIN;
    for (int num : edge_num) {
      if (num < 0) continue;
      min_num = std::min(min_num, num);
      max_num = std::max(max_num, num);
    }
    if (min_num == max_num) break;
    // 删除叶节点
    leaf_nodes.clear();
    for (int i = 0; i < n; i++) {
      if (edge_num[i] == 1) {
        edge_num[i] = -1;
        leaf_nodes.push_back(i);
      }
    }
    for (int leaf : leaf_nodes) {
      for (int link : node_link[leaf]) {
        edge_num[link]--;
      }
    }
  }

  vector<int> result;
  for (int i = 0; i < n; i++) {
    if (edge_num[i] >= 0) result.push_back(i);
  }
  return result;
}

/*
  优化: 用队列实现bfs
*/
vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
  vector<int> edge_num(n, 0);
  vector<vector<int>> node_link(n, vector<int>{});
  for (auto& edge : edges) {
    int i = edge[0], j = edge[1];
    edge_num[i]++;
    edge_num[j]++;
    node_link[i].push_back(j);
    node_link[j].push_back(i);
  }

  vector<int> leaf_nodes;
  leaf_nodes.reserve(n);
  for (int i = 0; i < n; i++) {
    // 只有一个节点(n=1)时, edge_num[0] = 0
    if (edge_num[i] <= 1) leaf_nodes.push_back(i);
  }

  int begin = 0, end = (int)leaf_nodes.size();
  while (end < n) {
    for (int i = begin; i < end; i++) {
      for (int link : node_link[leaf_nodes[i]]) {
        if (--edge_num[link] == 1) leaf_nodes.push_back(link);
      }
    }
    begin = end;
    end = (int)leaf_nodes.size();
  }

  return vector<int>(leaf_nodes.begin()+begin, leaf_nodes.end());
}
