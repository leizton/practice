#include <header>

/*
给定二叉搜索树 root, 找出 p q 的最小公共祖先
*/

TreeNode* dfs(TreeNode* node, int min_val, int max_val) {
  if (max_val < node->val) {
    return dfs(node->left);
  } else if (min_val > node->val) {
    return dfs(node->right);
  } else {
    return node;
  }
}

TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
  if (!root) return nullptr;
  int min_val = std::min(p->val, q->val);
  int max_val = std::max(p->val, q->val);
  // return dfs(root, min_val, max_val);
  auto* node = root;
  while (node) {
    if (max_val < node->val) {
      node = node->left;
    } else if (min_val > node->val) {
      node = node->right;
    } else {
      return node;
    }
  }
  return node;
}
