#include <header>

/*
给你二叉树的根结点 root 请你将它展开为一个单链表：
展开后的单链表应该同样使用 TreeNode 其中 right 子指针指向链表中下一个结点，而左子指针始终为 null 。
展开后的单链表应该与二叉树 先序遍历 顺序相同。
*/

void preOrderTraverseTree(TreeNode* node, TreeNode* &prev) {
  if (prev) {
    prev->left = nullptr;
    prev->right = node;
  }
  if (node) {
    TreeNode *left(node->left), *right(node->right);
    prev = node;
    preOrderTraverseTree(left, prev);
    preOrderTraverseTree(right, prev);
  }
}
void flatten(TreeNode* root) {
  TreeNode* prev = nullptr;
  preOrderTraverseTree(root, prev);
}
