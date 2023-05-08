#include <header>

/*
  给定连接矩阵is_conn
  is_conn[i][j] 表示 node_i 和 node_j 有连接
  可以推断 is_conn对角线上元素都是1 is_conn是对称矩阵
  求集群数量。集群指有直接或间接关系的节点组成的节点集合, 例如 is_conn[a][b]=is_conn[b][c]=1 is_conn[a][c]=0 则{a,b,c}构成集群
  集群是一个连通域
*/

int findCircleNum(vector<vector<int>>& is_conn) {
  if (is_conn.empty() || is_conn.size() != is_conn[0].size()) {
    return 0;
  }
  const int node_num = (int)is_conn.size();

  // init. root(i) is assigned self
  vector<int> root(node_num, 0);
  for (int i = 0; i < node_num; i++) {
    root[i] = i;
  }

  // merge
  for (int i = 0; i < node_num; i++) {
    for (int j = 0; j < i; j++) {
      if (is_conn[i][j]) {
        int new_root = std::min(root[i], root[j]);
        int mod_root = -1, k = -1;
        if (root[i] != new_root) {
          mod_root = root[i];
          k = i;
        } else if (root[j] != new_root) {
          mod_root = root[j];
          k = j;
        }
        for (; k >= mod_root; k--) {
          if (root[k] == mod_root) {
            root[k] = new_root;
          }
        }
      }
    }
  }

  int root_num = 0;
  for (int i = 0; i < node_num; i++) {
    if (root[i] == i) root_num++;
  }
  return root_num;
}
