#include <util/base.h>

ListNode* getIntersectionNodeV0(ListNode* ha, ListNode* hb) {
  if (ha == nullptr || hb == nullptr) return nullptr;
  vector<ListNode*> a, b;
  for (ListNode* p = ha; p != nullptr; p = p->next) {
    a.push_back(p);
  }
  for (ListNode* p = hb; p != nullptr; p = p->next) {
    b.push_back(p);
  }
  int i = int(a.size())-1, j = int(b.size())-1;
  if (a[i] != b[j]) return nullptr;  // 不相交
  i--, j--;
  while (i >= 0 && j >= 0) {
    if (a[i] == b[j]) {
      i--, j--;
    } else {
      return a[i+1];
    }
  }
  return (i < 0) ? ha : hb;
}

ListNode* getIntersectionNodeV1(ListNode* ha, ListNode* hb) {
  ListNode *p(ha), *q(hb);
  while (p != q) {
    p = p == nullptr ? hb : p->next;
    q = q == nullptr ? ha : q->next;
  }
  return p;
}

// 两个链表的长度差
ListNode* getIntersectionNode(ListNode* ha, ListNode* hb) {
  if (ha == nullptr || hb == nullptr) return nullptr;
  int len_a = 0, len_b = 0;
  for (ListNode* p = ha; p != nullptr; p = p->next) len_a++;
  for (ListNode* p = hb; p != nullptr; p = p->next) len_b++;
  ListNode *p, *q;
  int n = len_a - len_b;
  if (n < 0) {
    p = hb, q = ha;
    n = -n;
  } else {
    p = ha, q = hb;
  }
  for (; n > 0; n--) p = p->next;
  while (p && q && p != q) {
    p = p->next;
    q = q->next;
  }
  return p;
}
