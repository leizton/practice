#include <header>

/*
117.cc
给定一个二叉树，填充它的每个 next 指针，让这个指针指向其下一个右侧节点。
如果找不到下一个右侧节点，则将 next 指针设置为 NULL。
*/

Node* connect(Node* root) {
  if (!root) return nullptr;
  queue<pair<Node*,int>> que;  // <node, depth>
  que.push({root, 0});
  pair<Node*,int> prev{nullptr, -1};
  while (!que.empty()) {
    auto curr = que.front();
    que.pop();
    if (prev.second == curr.second) {
      prev.first->next = curr.first;
    }
    prev = curr;
    if (curr.first->left) que.push({curr.first->left, curr.second+1});
    if (curr.first->right) que.push({curr.first->right, curr.second+1});
  }
  return root;
}

Node* connect(Node* root) {
  if (!root) return nullptr;
  queue<Node*> que;
  que.push(root);
  while (!que.empty()) {
    // 当前层的最后一个节点
    Node* last = que.back();
    last->next = nullptr;
    // 遍历当前层
    Node* prev = que.front();
    que.pop();
    if (prev->left) que.push(prev->left);
    if (prev->right) que.push(prev->right);
    while (prev != last) {
      Node* curr = que.front();
      que.pop();
      if (curr->left) que.push(curr->left);
      if (curr->right) que.push(curr->right);
      // 连接
      prev->next = curr;
      prev = curr;
    }
  }
  return root;
}

Node* connect(Node* root) {
  if (!root) return nullptr;
  queue<Node*> que;
  que.push(root);
  while (!que.empty()) {
    // 当前层的最后一个节点
    Node* last = que.back();
    last->next = nullptr;
    // 遍历当前层
    Node* p = que.front();
    que.pop();
    while (p != last) {
      if (p->left) que.push(p->left);
      if (p->right) que.push(p->right);
      // 访问
      p->next = que.front();
      que.pop();
      // 更新 p
      p = p->next;
    }
    if (p->left) que.push(p->left);
    if (p->right) que.push(p->right);
  }
  return root;
}

/*
199.cc  二叉树右视图
给定一个二叉树的 根节点 root，想象自己站在它的右侧，按照从顶部到底部的顺序，返回从右侧所能看到的节点值
*/

// BFS
vector<int> rightSideView(TreeNode* root) {
  vector<int> ans;
  if (!root) return ans;
  queue<TreeNode*> que;
  que.push(root);
  while (!que.empty()) {
    TreeNode* last = que.back();
    ans.push_back(last->val);
    TreeNode* p = que.front();
    que.pop();
    while (p != last) {
      if (p->left) que.push(p->left);
      if (p->right) que.push(p->right);
      p = que.front();
      que.pop();
    }
    if (p->left) que.push(p->left);
    if (p->right) que.push(p->right);
  }
  return ans;
}

// DFS
// 按照 node-右子树-左子树 的前序遍历, 每层第一个节点就是最右节点
void dfs(TreeNode* node, int depth, vector<int>& ans) {
  depth++;
  if (depth > ans.size()) {
    ans.push_back(node->val);
  }
  if (node->right) dfs(node->right, depth, ans);
  if (node->left) dfs(node->left, depth, ans);
}
vector<int> rightSideView(TreeNode* root) {
  vector<int> ans;
  if (!root) return ans;
  dfs(root, 0, ans);
  return ans;
}
