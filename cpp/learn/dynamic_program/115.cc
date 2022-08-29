#include <util/base.h>

/*
给定字符串 s 和 t ，计算在 s 的子序列中 t 出现的个数。
字符串的一个子序列是指，通过删除一些（也可以不删除）字符且不干扰剩余字符相对位置所组成的新字符串。
子串要求连续，子序列不要求连续
例如，"ACE" 是 "ABCDE" 的一个子序列，而 "AEC" 不是
题目数据保证答案符合 32 位带符号整数范围。
*/

/*
  考虑 s[0..i] 有多少个子序列 t[0..j]
  if s[i] != t[j]
    ans(s[0..i], t[0..j]) = ans(s[0..i-1], t[0..j])
  else
    ans(s[0..i], t[0..j]) = ans(s[0..i-1], t[0..j])
                          + ans(s[0..i-1], t[0..j-1])
*/

int numDistinct(string s, string t) {
  if (t.empty()) return 1;
  if (s.length() < t.length()) return 0;
  const int m = s.length(), n = t.length();
  vector<uint32_t> dp(m, 0);  // vector<int> 会越界
  // dp[i][0] = ans(s[0..i], t[0])
  dp[0] = (s[0] == t[0]) ? 1 : 0;
  for (int i = 1; i < m; i++) {
    dp[i] = dp[i-1] + ((s[i] == t[0]) ? 1 : 0);
  }
  //
  uint32_t dp_i1_j1 = 0, next_dp_i1_j1 = 0;
  for (int j = 1; j < n; j++) {
    dp_i1_j1 = dp[0];  // dp[i-1][j-1]
    dp[0] = 0;
    for (int i = 1; i < m; i++) {
      next_dp_i1_j1 = dp[i];
      dp[i] = dp[i-1];
      if (s[i] == t[j]) dp[i] += dp_i1_j1;
      dp_i1_j1 = next_dp_i1_j1;
    }
  }
  return dp[m-1];
}

int main() {
  auto test = numDistinct;
  assert_eq(543744000, test("xslledayhxhadmctrliaxqpokyezcfhzaskeykchkmhpyjipxtsuljkwkovmvelvwxzwieeuqnjozrfwmzsylcwvsthnxujvrkszqwtglewkycikdaiocglwzukwovsghkhyidevhbgffoqkpabthmqihcfxxzdejletqjoxmwftlxfcxgxgvpperwbqvhxgsbbkmphyomtbjzdjhcrcsggleiczpbfjcgtpycpmrjnckslrwduqlccqmgrdhxolfjafmsrfdghnatexyanldrdpxvvgujsztuffoymrfteholgonuaqndinadtumnuhkboyzaqguwqijwxxszngextfcozpetyownmyneehdwqmtpjloztswmzzdzqhuoxrblppqvyvsqhnhryvqsqogpnlqfulurexdtovqpqkfxxnqykgscxaskmksivoazlducanrqxynxlgvwonalpsyddqmaemcrrwvrjmjjnygyebwtqxehrclwsxzylbqexnxjcgspeynlbmetlkacnnbhmaizbadynajpibepbuacggxrqavfnwpcwxbzxfymhjcslghmajrirqzjqxpgtgisfjreqrqabssobbadmtmdknmakdigjqyqcruujlwmfoagrckdwyiglviyyrekjealvvigiesnvuumxgsveadrxlpwetioxibtdjblowblqvzpbrmhupyrdophjxvhgzclidzybajuxllacyhyphssvhcffxonysahvzhzbttyeeyiefhunbokiqrpqfcoxdxvefugapeevdoakxwzykmhbdytjbhigffkmbqmqxsoaiomgmmgwapzdosorcxxhejvgajyzdmzlcntqbapbpofdjtulstuzdrffafedufqwsknumcxbschdybosxkrabyfdejgyozwillcxpcaiehlelczioskqtptzaczobvyojdlyflilvwqgyrqmjaeepydrcchfyftjighntqzoo", "rwmimatmhydhbujebqehjprrwfkoebcxxqfktayaaeheys"));
  assert_eq(0, test("b", "a"));
}
