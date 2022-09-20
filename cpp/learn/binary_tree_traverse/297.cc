#include <header>

/*
  二叉树序列化和反序列化
  层序遍历
*/

class TreeCodec {
public:
  string serialize(TreeNode* root) {
    // 层序遍历
    if (!root) return "[]";
    string result(256, '\0');
    result.clear();
    std::queue<TreeNode*> que;
    que.push(root);
    while (!que.empty()) {
      auto* node = que.front();
      que.pop();
      if (node) {
        result += "," + std::to_string(node->val);
        que.push(node->left);
        que.push(node->right);
      } else {
        result += ",N";
      }
    }
    int p = (int)result.length() - 1;
    while (result[p] == 'N') p -= 2;
    result.resize(p+1);
    result[0] = '[';
    result += "]";
    return result;
  }

  TreeNode* deserialize(string data) {
    const size_t n = data.length();
    if (n <= 2) return nullptr;
    const char* s = data.c_str();
    size_t i = 1u;
    int v = atoi(s+1);
    while (s[i] != ',' && s[i] != ']') i++;
    i++;
    TreeNode* root = new TreeNode(v);
    // pair<parent_node, 是否设置过左子节点>
    queue<pair<TreeNode*,bool>> que;
    que.push({root,false});
    while (i < n) {
      TreeNode* node = nullptr;
      if (s[i] == 'N') {
        i += 2;
      } else {
        v = atoi(s+i);
        while (s[i] != ',' && s[i] != ']') i++;
        i++;
        node = new TreeNode(v);
        que.push({node,false});
      }
      auto& parent = que.front();
      if (parent.second) {
        parent.first->right = node;
        que.pop();
      } else {
        parent.second = true;
        parent.first->left = node;
      }
    }
    return root;
  }
};
