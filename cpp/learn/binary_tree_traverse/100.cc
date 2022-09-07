#include <header>

/*
给你两棵二叉树的根节点 p 和 q ，编写一个函数来检验这两棵树是否相同。
如果两个树在结构上相同，并且节点具有相同的值，则认为它们是相同的。
*/

// 同时前序遍历p和q
bool isSameTree(TreeNode* p, TreeNode* q) {
  if (p && !q) return false;
  if (!p && q) return false;
  if (!p && !q) return true;
  if (p->val != q->val) return false;
  return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
}
