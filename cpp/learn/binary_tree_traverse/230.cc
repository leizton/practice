#include <header>

/*
找二叉搜索树中第k小的元素, k从1开始
*/

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
  // end - begin >= 5
  int p = a[begin], pivot = a[begin+1], q = a[end-1];
  if (p <= pivot) {
    if (pivot <= q) { }
    else if (p <= q) { std::swap(pivot, q); }
    else { int t = p; p = q; q = pivot; pivot = t; }
  } else {
    if (p < q) { std::swap(p, pivot); }
    else if (pivot < q) { int t = p; p = pivot; pivot = q; q = t; }
    else { std::swap(p, q); }
  }
  //
  a[begin+1]=p; a[begin]=pivot; a[end-1]=q;
  pivot = a[begin], p = begin+2, q = end-2;
  while (true) {
    while (a[p] < pivot) p++;  // [end-1]是哨兵, begin+2<=p<=end-1
    while (a[q] > pivot) q--;  // [begin+1]是哨兵, begin+1<=q<=end-2
    if (p >= q) break;
    std::swap(a[p], a[q]);
    p++, q--;
  }
  // begin+2<=p<=end-1 && a[p-1]<pivot && a[0]=pivot
  return p;
}
void select_topk(vector<int>& a, int k) {
  int p = 0, q = (int)a.size();
  while (q - p >= 5) {
    int pivot = q_partition(a, p, q);
    if (pivot < k)
      p = pivot;
    else if (pivot == k)
      return;
    else
      q = pivot;
  }
  insert_sort(a, p, q);
}
void dfs(TreeNode* node, int k, vector<int>& buf) {
  if (!node) return;
  buf.push_back(node->val);
  if ((int)buf.size() >= (k << 1)) {
    select_topk(buf, k);
    buf.resize(k);
  }
  dfs(node->left, k, buf);
  dfs(node->right, k, buf);
}
int kthSmallest(TreeNode* root, int k) {
  if (!root || k <= 0) return 0;
  vector<int> buf;
  buf.reserve(k * 2);
  dfs(root, k, buf);
  select_topk(buf, k);
  if (k < (int)buf.size()) buf.resize(k);
  int ans = buf[0];
  for (int e : buf) ans = std::max(ans, e);
  return ans;
}
