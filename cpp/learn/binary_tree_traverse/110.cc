#include <header>

/*
给定一个二叉树，判断它是否是高度平衡的二叉树。
本题中，一棵高度平衡二叉树定义为：
一个二叉树每个节点的左右两个子树的高度差的绝对值不超过 1 。
*/

// 返回树的高度, 小于0表示非平衡
int check(TreeNode* root) {
  if (root == nullptr) return 0;
  int l = check(root->left);
  if (l < 0) return -1;
  int r = check(root->right);
  if (r < 0 || std::abs(l - r) > 1) return -1;
  return std::max(l, r) + 1;
}
bool isBalanced(TreeNode* root) {
  return check(root) >= 0;
}
