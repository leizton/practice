#include <header>

/*
  edges存放入n个节点的边信息, <edges_i_0, edges_i_1> 两个节点有边
  总共有n条边, 期望删除一条边使图变成数
  例如 edges = [ <1,2> <1,3> <2,4> <2,5> <4,5> ]
  删除边<4,5>后就是树
*/

vector<int> findRedundantConnection(vector<vector<int>>& edges) {
  const int n = (int)edges.size();

  // edges编号从1开始，所以root长度用n+1
  vector<int> root(n+1, 0);
  for (int i = 1; i < n; i++) {
    root[i] = i;
  }

  for (auto& edge : edges) {
    int root_i = edge[0];
    while (root[root_i] != root_i) root_i = root[root_i];
    int root_j = edge[1];
    while (root[root_j] != root_j) root_j = root[root_j];
    if (root_i < root_j) {
      root[root_j] = root_i;
    } else if (root_i > root_j) {
      root[root_i] = root_j;
    } else {
      // 当node_i和node_j有相同root且两个节点右边, 说明这是一个环
      return edge;
    }
  }
  return vector<int>();
}
