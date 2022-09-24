#include <header>

/*

给定一个整数 n

请你生成并返回所有可能的不同二叉搜索树
这些二叉搜索树都包含n个节点，且节点值从1到n

*/

void dfs(int from, int to, vector<TreeNode*>& ret) {
  vector<TreeNode*> left_trees, right_trees;
  for (int root_val = from; root_val <= to; root_val++) {
    dfs(from, root_val-1, left_trees);
    dfs(root_val+1, to, right_trees);
    if (left_trees.empty()) left_trees.push_back(nullptr);
    if (right_trees.empty()) right_trees.push_back(nullptr);
    for (auto* l : left_trees) {
      for (auto* r : right_trees) {
        ret.push_back(new TreeNode(root_val, l, r));
      }
    }
    left_trees.clear();
    right_trees.clear();
  }
}
vector<TreeNode*> generateTrees(int n) {
  vector<TreeNode*> ret;
  dfs(1, n, ret);
  return ret;
}

/*
  96.cc
  只需给出二叉树的种类数目
  用动规求解
*/
int numTrees(int n) {
  if (n <= 0) return 0;
  // dp[i] = numTrees(i)
  vector<int> dp(n+1, 0);
  dp[0] = dp[1] = 1;
  for (int i = 2; i <= n; i++) {
    int num = 0;
    for (int root = 1; root <= i; root++) {
      num += dp[root-1] * dp[i-root];
    }
    dp[i] = num;
  }
  return dp[n];
}

int main() {
  auto ret = generateTrees(3);
  for (auto* root : ret) {
    print(CmdReader::dumpTree(root));
  }
}