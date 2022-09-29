#include <header>

/*
交错字符串

给定三个字符串 s1、s2、s3，请你帮忙验证 s3 是否是由 s1 和 s2 交错 组成的

例1 s1=ab s2=ac s3=abac
s3的第一个a从s1而来  s1_0->s3_0  s1_1->s3_1  s2_0->s3_2  s2_1->s3_3

例2 s1=ab s2=ac s3=abca
s3不能由s1 s2交错生成
*/

/*
  每次有两个选择 s1 或者 s2
  相当于在二叉树上遍历

  遍历时有相同的情况, 例如
  s1_0_n s2_0_m -> s3_0_l
    (1) s1_1_n s2_0_m -> s3_1_l
          (1.1) s1_2_n s2_0_m -> s3_2_l
          (1.2) s1_1_n s2_1_m -> s3_2_l
    (2) s1_0_n s2_1_m -> s3_0_l
          (2.1) s1_1_n s2_1_m -> s3_2_l
          (2.2) s1_0_n s2_2_m -> s3_2_l
  (1.2)和(2.1)相同
*/

bool dfs(string& s1, string& s2, string& s3, size_t i, size_t j, vector<vector<uint8_t>>& memo) {
  // 如果验证过, 说明此路不通
  if (memo[i][j]) return false;
  // 标记验证过
  memo[i][j] = 1;
  //
  size_t k = i + j;
  if (k == s3.length()) return true;
  // left
  if (i < s1.length() && s3[k] == s1[i] && dfs(s1, s2, s3, i+1, j, memo)) {
    return true;
  }
  // right
  if (j < s2.length() && s3[k] == s2[j] && dfs(s1, s2, s3, i, j+1, memo)) {
    return true;
  }
  return false;
}

bool isInterleave(string s1, string s2, string s3) {
  size_t n = s1.length(), m = s2.length();
  if (n + m != s3.length()) return false;
  vector<vector<uint8_t>> memo(n+1, vector<uint8_t>(m+1, 0));
  return dfs(s1, s2, s3, 0, 0, memo);
}

int main() {
  assert_T(isInterleave("aabcc", "dbbca", "aadbbcbcac"));
}
