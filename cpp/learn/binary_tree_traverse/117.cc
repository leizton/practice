#include <header>

/*
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
