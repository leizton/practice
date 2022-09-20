#include <header>

bool dfs(TreeNode* node, int k, int& count, int& v) {
  if (!node) return false;
  if (dfs(node->left, k, count, v)) return true;
  if (++count >= k) {
    v = node->val;
    return true;
  }
  return dfs(node->right, k, count, v);
}
int kthSmallest(TreeNode* root, int k) {
  if (!root || k <= 0) return 0;
  int count = 0, v = 0;
  dfs(root, k, count, v);
  return v;
}

void insert_sort(vector<int>& a, int begin, int end) {
  for (int i = begin+1; i < end; i++) {
    if (a[i-1] <= a[i]) continue;
    int v = a[i];
    a[i] = a[i-1];
    int j = i-2;
    for (; j >= 0 && v < a[j]; j--) {
      a[j+1] = a[j];
    }
    a[j+1] = v;
  }
}
int q_partition(vector<int>& a, int begin, int end) {
  // end - begin > 4
  int pivot = a[begin];
  int p = begin+1, q = end-1;
  while (true) {
    while (p < end && a[p] <= pivot) p++;   // begin+1<=p<=end && a[p-1]<=pivot
    while (q > begin && a[q] > pivot) q--;
    if (p >= q) break;
    std::swap(a[p], a[q]);
    p++, q--;
  }
  std::swap(a[begin], a[p-1]);  // 这一步是必要的
  return p;
}
void select_k(vector<int>& a, int k) {
  int p = 0, q = (int)a.size();
  while (q - p > 4) {
    int pivot = q_partition(a, p, q);
    if (pivot < k)
      p = pivot;
    else if (pivot == k)
      return;
    else
      q = pivot-1;
  }
  insert_sort(a, p, q);
}
void dfs(TreeNode* node, int k, vector<int>& buf) {
  if (!node) return;
  buf.push_back(node->val);
  if ((int)buf.size() >= (k << 1)) {
    select_k(buf, k);
    buf.resize(k);
  }
  dfs(node->left, k, buf);
  dfs(node->right, k, buf);
}
int kthSmallestV0(TreeNode* root, int k) {
  if (!root || k <= 0) return 0;
  vector<int> buf;
  buf.reserve(k * 2);
  dfs(root, k, buf);
  select_k(buf, k);
  return buf[k-1];
}
