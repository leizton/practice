#include <header>

/*
给你二叉树的根节点 root 和一个表示目标和的整数 target
判断该树中是否存在 根节点到叶子节点 的路径，这条路径上所有节点值相加的和等于目标值 target
如果存在，返回 true ；否则，返回 false
*/
bool hasPathSum(TreeNode* root, int target) {
  if (root == nullptr) return false;
  target -= root->val;
  if (root->left == nullptr && root->right == nullptr) return target == 0;
  return hasPathSum(root->left, target) || hasPathSum(root->right, target);
}

/*
给你二叉树的根节点 root 和一个整数目标和 target
找出所有 从根节点到叶子节点 路径总和等于给定目标和的路径
*/
// curr_path 记录当前路径
void pathSumAid(TreeNode* root, int target, vector<int>& curr_path, vector<vector<int>>& ans) {
  target -= root->val;
  if (root->left == nullptr && root->right == nullptr) {
    if (target == 0) {
      ans.push_back(curr_path);
      ans.back().push_back(root->val);
    }
    return;
  }
  if (root->left) {
    curr_path.push_back(root->val);
    pathSumAid(root->left, target, curr_path, ans);
    curr_path.pop_back();
  }
  if (root->right) {
    curr_path.push_back(root->val);
    pathSumAid(root->right, target, curr_path, ans);
    curr_path.pop_back();
  }
}
vector<vector<int>> pathSum(TreeNode* root, int target) {
  vector<vector<int>> ans;
  if (root == nullptr) return ans;
  vector<int> curr_path;
  pathSumAid(root, target, curr_path, ans);
  return ans;
}
