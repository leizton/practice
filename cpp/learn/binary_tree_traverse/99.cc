#include <util/base.h>

/*
给你二叉搜索树的根节点root ，该树中的恰好两个节点的值被错误地交换。请在不改变其结构的情况下，恢复这棵树。
*/

/*
  利用性质: ✌🏻🔍🌲的中序遍历是有序递增的数组
  用一个数组记录中序遍历的节点, 再找出需要交换的两个节点
*/
void inOrderTraverseTree(TreeNode* node, TreeNode* &prev,
                         vector<pair<TreeNode*,TreeNode*>>& to_move) {
  if (!node) return;
  inOrderTraverseTree(node->left, prev, to_move);
  if (prev && node->val < prev->val) {
    to_move.emplace_back(prev, node);
  }
  prev = node;
  inOrderTraverseTree(node->right, prev, to_move);
}

void recoverTree(TreeNode* root) {
  vector<pair<TreeNode*,TreeNode*>> to_move;
  to_move.reserve(2);
  TreeNode* prev = nullptr;
  inOrderTraverseTree(root, prev, to_move);
  if (to_move.size() == 1) {
    // (x1 x2)
    std::swap(to_move[0].first->val, to_move[0].second->val);
  } else if (to_move.size() == 2) {
    /*
      原顺序: (x1 x2) (x3 x4)
      满足条件: x1 > x2, x2 <= x3, x3 > x4
      如果只能交换一次, 只有<x1 x4>交换才能满足条件
      例如 <x1 x3>交换后 (x3 x2 x1 x4) 不满足 x3 > x4
    */
    std::swap(to_move[0].first->val, to_move[1].second->val);
  }
}
