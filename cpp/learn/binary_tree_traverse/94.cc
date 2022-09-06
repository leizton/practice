#include <util/base.h>

/*
二叉树的中序遍历
*/

vector<int> inorderTraversal(TreeNode* root) {
  // <node, 是否访问过left>
  vector<pair<TreeNode*, bool>> sk;
  sk.emplace_back(root, false);
  //
  vector<int> result;
  while (!sk.empty()) {
    auto node = sk.back().first;
    if (node == nullptr) {
      sk.pop_back();
      continue;
    }
    if (sk.back().second == false) {
      sk.back().second = true;
      sk.emplace_back(node->left, false);
    } else {
      result.push_back(node->val);
      sk.pop_back();
      sk.emplace_back(node->right, false);
    }
  }
  return result;
}
