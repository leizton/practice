#include <header>

/*
  给定连接矩阵is_conn
  is_conn[i][j] 表示 node_i 和 node_j 有连接
  可以推断 is_conn对角线上元素都是1 is_conn是对称矩阵
  求集群数量。集群指有直接或间接关系的节点组成的节点集合, 例如 is_conn[a][b]=is_conn[b][c]=1 is_conn[a][c]=0 则{a,b,c}构成集群
  集群是一个连通域
*/

int findCircleNum(vector<vector<int>>& is_conn) {
  const int n = (int)is_conn.size();
  if (n == 0) return 0;

  // cluster[i] = node_i 所属的连通域编号
  vector<int> cluster(n, 0);
  // init: 每个node都属于不同的集群
  for (int i = 0; i < n; i++) {
    cluster[i] = i;
  }

  for (int i = 0; i < n; i++) {
    int root_of_i = i;
    while (cluster[root_of_i] != root_of_i) root_of_i = cluster[root_of_i];
    for (int j = i+1; j < n; j++) {
      if (is_conn[i][j]) {
        int root_of_j = j;
        while (cluster[root_of_j] != root_of_j) root_of_j = cluster[root_of_j];
        // merge: 只在root层面合并
        if (root_of_i < root_of_j) {
          cluster[root_of_j] = root_of_i;
        } else if (root_of_i > root_of_j) {
          cluster[root_of_i] = root_of_j;
          root_of_i = root_of_j;
        }
      }
    }
  }

  // adjust: 改成和root直连
  for (int i = 1; i < n; i++) {
    int root = i;
    while (cluster[root] != root) root = cluster[root];
    cluster[i] = root;
  }

  int res = 1;
  for (int i = 1; i < n; i++) {
    if (cluster[i] == i) res++;
  }
  return res;
}
