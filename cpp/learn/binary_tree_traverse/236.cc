#include <util/base.h>

/*
给定一个二叉树, 找到该树中两个指定节点的最近公共祖先。

比较简单的思路是前序遍历树分别得出 从root到p和q的 路径vector<Node*> path_p path_q
找出path_p和path_q的最后一个公共节点

如果树的节点可以做标记，那就找到p后回溯时做标记，找到q后回溯时第一个有标记的节点就是LCA
*/

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
TreeNode* lowestCommonAncestorV0(TreeNode* root, TreeNode* p, TreeNode* q) {
  if (p == q || p == nullptr || q == nullptr) return (p == nullptr) ? q : p;
  if (root == nullptr || root == p || root == q) root;
  TreeNode* answer = nullptr;
  findLowestCommonAncestor(root, p, q, answer);
  return answer;
}

/*
  隐含前提: p q 都在树中

  在当前根节点root的树中同时 search p q, 有4种可能情况
  ① 没有找到 p q, 返回NULL
  ② 找到 p q 中的某一个, 返回p或q 标记找到, 通知上层节点
  ③ 找到 p q

  对于情况③
    if 在左右子树分别找到 p q
      root 是 LCA, 返回root
    elif 在左子树找到 p q
      左子树包含 LCA, 返回左子树的搜索结果
    else 在右子树找到 p q
      右子树包含 LCA, 返回右子树的搜索结果
*/
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
  if (!root) return nullptr;  // 情况①
  if (root == p) return p;    // 情况②
  if (root == q) return q;    // 情况②
  TreeNode *left = lowestCommonAncestor(root->left, p, q);
  TreeNode *right = lowestCommonAncestor(root->right, p, q);
  if (!left && !right) return nullptr;  // 情况①
  if (left && right)   return root;     // 情况③
  return left ? left : right;           // 情况② ③
}
