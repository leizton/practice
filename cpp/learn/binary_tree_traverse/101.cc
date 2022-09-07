#include <header>

/*
给你一个二叉树的根节点 root, 检查它是否轴对称
*/

/*
例如: 完全二叉树 { a [b d-d1-d2 e-e1-e2] [c f-f1-f2 g-g1-g2] }

左子树后序(先左后右) d1 d2 d e1 e2 e b
右子树后序(先右后左) g2 g1 g f2 f1 f c

后序改成前序, 可以提前检查根节点
*/

// @ref 100.cc
bool preOrderCheck(TreeNode* p, TreeNode* q) {
  if (p && !q) return false;
  if (!p && q) return false;
  if (!p && !q) return true;
  if (p->val != q->val) return false;
  return preOrderCheck(p->left, q->right) && preOrderCheck(p->right, q->left);
}
bool isSymmetric(TreeNode* root) {
  if (!root) return true;
  return preOrderCheck(root->left, root->right);
}
