#include <header>

/*
给定一个二叉树，找出其最大深度。
二叉树的深度为根节点到最远叶节点的最长路径上的节点数。

example
给定二叉树 [3,9,20,null,null,15,7]，返回它的最大深度 3 。
*/

void preOrderTraverseTree(TreeNode* node, int prev_depth, int& ans) {
  ans = std::max(ans, prev_depth + 1);
  if (node->left) preOrderTraverseTree(node->left, prev_depth+1, ans);
  if (node->right) preOrderTraverseTree(node->right, prev_depth+1, ans);
}
int maxDepth0(TreeNode* root) {
  int ans = 0;
  if (root) preOrderTraverseTree(root, 0, ans);
  return ans;
}

int maxDepth(TreeNode* root) {
  return (root == nullptr) ? 0 : (1 + std::max(maxDepth(root->left), maxDepth(root->right)));
}

/*
给定一个二叉树，找出其最小深度。
最小深度是从根节点到最近叶子节点的最短路径上的节点数量。
说明：叶子节点是指没有子节点的节点。
*/
int minDepth(TreeNode* root) {
  if (root == nullptr) return 0;
  if (root->left == nullptr && root->right == nullptr) return 1;
  int l = INT32_MAX, r = INT32_MAX;
  if (root->left) l = minDepth(root->left);
  if (l == 1) return 2;
  if (root->right) r = minDepth(root->right);
  return 1 + std::min(l, r);
}
