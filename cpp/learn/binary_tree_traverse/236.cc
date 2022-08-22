#include <util/base.h>

int findLowestCommonAncestor(TreeNode* node, TreeNode* p, TreeNode* q, TreeNode*& answer) {
  static const int empty_mark = 0;
  static const int p_mark = 1;
  static const int q_mark = 2;
  static const int both_mark = p_mark | q_mark;
  if (node == nullptr) {
    return empty_mark;
  } else if (node == p) {
    if ((findLowestCommonAncestor(node->left, p, q, answer) & q_mark) ||
        (findLowestCommonAncestor(node->right, p, q, answer) & q_mark)) {
      answer = node;
      return both_mark;
    }
    return p_mark;
  } else if (node == q) {
    if ((findLowestCommonAncestor(node->left, p, q, answer) & p_mark) ||
        (findLowestCommonAncestor(node->right, p, q, answer) & p_mark)) {
      answer = node;
      return both_mark;
    }
    return q_mark;
  } else {
    int mark = findLowestCommonAncestor(node->left, p, q, answer);
    if (mark == both_mark) return both_mark;
    mark |= findLowestCommonAncestor(node->right, p, q, answer);
    if (mark == both_mark && answer == nullptr) {
      answer = node;
    }
    return mark;
  }
}

TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
  if (p == q || p == nullptr || q == nullptr) return (p == nullptr) ? q : p;
  if (root == nullptr || root == p || root == q) root;
  TreeNode* answer = nullptr;
  findLowestCommonAncestor(root, p, q, answer);
  return answer;
}

/**
 * 后序遍历
 */
TreeNode* lowestCommonAncestorV1(TreeNode* root, TreeNode* p, TreeNode* q) {
  if (!root || root == p || root == q) return root;
  TreeNode *left = lowestCommonAncestor(root->left, p, q);
  TreeNode *right = lowestCommonAncestor(root->right, p, q);
  if (left && right) return root;  // p q 在不同子树
  return left ? left : right;      // p q 在相同子树
}
