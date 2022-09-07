#include <header>

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
