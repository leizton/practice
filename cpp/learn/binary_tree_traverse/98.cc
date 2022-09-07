#include <util/base.h>

/*
给你一个二叉树的根节点 root ，判断其是否是一个有效的二叉搜索树。
有效 二叉搜索树定义如下：
  节点的左子树只包含 小于 当前节点的数。
  节点的右子树只包含 大于 当前节点的数。
  所有左子树和右子树自身必须也是二叉搜索树。
*/

bool checkBST(TreeNode* root, int min_val, int max_val) {
  if (root->val < min_val) return false;
  if (root->val > max_val) return false;
  if (root->left && (root->val == min_val || !checkBST(root->left, min_val, root->val-1))) return false;
  if (root->right && (root->val == max_val || !checkBST(root->right, root->val+1, max_val))) return false;
  return true;
}
bool isValidBST0(TreeNode* root) {
  if (!root) return true;
  return checkBST(root, INT32_MIN, INT32_MAX);
}

/*
  中序遍历二叉搜索树是递增的
*/
bool inOrderTraverseTree(TreeNode* node, TreeNode* &prev) {
  if (node == nullptr) return true;
  if (!inOrderTraverseTree(node->left, prev)) return false;
  if (prev && node->val <= prev->val) return false;
  prev = node;
  return inOrderTraverseTree(node->right, prev);
}
bool isValidBST(TreeNode* root) {
  TreeNode* prev = nullptr;
  return inOrderTraverseTree(root, prev);
}
