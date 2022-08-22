#include <util/base.h>

int calcTreeHeight(TreeNode* root) {
  int h = -1;
  if (root->left) {
    h = calcTreeHeight(root->left);
  }
  if (root->right) {
    h = std::max(calcTreeHeight(root->right), h);
  }
  return h + 1;
}

void printTreeNode(TreeNode* node, int h, int r, int c, vector<vector<string>>& mat) {
  mat[r][c] = std::to_string(node->val);
  if (node->left) {
    printTreeNode(node->left, h, r+1, c-(1 << (h-r-1)), mat);
  }
  if (node->right) {
    printTreeNode(node->right, h, r+1, c+(1 << (h-r-1)), mat);
  }
}

vector<vector<string>> printTree(TreeNode* root) {
  if (root == nullptr) {
    return vector<vector<string>>();
  }
  int h = calcTreeHeight(root);
  vector<vector<string>> mat(h+1);
  int col_num = (1 << (h+1)) - 1;
  for (auto& row : mat) {
    row.resize(col_num);
  }
  printTreeNode(root, h, 0, ((col_num-1) >> 1), mat);
  return mat;
}
