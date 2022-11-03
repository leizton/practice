#include <util/base.h>

/*
给定字符串 s 和 t ，计算在 s 的子序列中 t 出现的个数。

例如，"ACE" 是 "ABCDE" 的一个子序列，而 "AEC" 不是
*/

int numDistinct(string s, string t) {
  const int m = s.length(), n = t.length();
  if (n == 0) return 1;
  if (m < n) return 0;

  // dp[i][j]: s[0:i]上有多少个t[0:j]
  vector<vector<uint32_t>> dp(m, vector<uint32_t>(n, 0));  // vector<int> 会越界

  dp[0][0] = (s[0] == t[0]) ? 1 : 0;
  for (int i = 1; i < m; i++) {
    dp[i][0] = dp[i-1][0] + ((s[i] == t[0]) ? 1 : 0);
  }
  for (int i = 1; i < n; i++) {
    dp[i][i] = (dp[i-1][i-1] && s[i] == t[i]) ? 1 : 0;
  }

  for (int i = 1; i < m; i++) {
    for (int j = 1; j < i && j < n; j++) {
      if (s[i] == t[j]) {
        dp[i][j] = dp[i-1][j-1] + dp[i-1][j];
      } else {
        dp[i][j] = dp[i-1][j];
      }
    }
  }
  return dp[m-1][n-1];
}

int main() {
  auto test = numDistinct;
  assert_eq(0, test("b", "a"));
  assert_eq(5, test("babgbag", "bag"));
  assert_eq(543744000, test("xslledayhxhadmctrliaxqpokyezcfhzaskeykchkmhpyjipxtsuljkwkovmvelvwxzwieeuqn"
    "jozrfwmzsylcwvsthnxujvrkszqwtglewkycikdaiocglwzukwovsghkhyidevhbgffoqkpabthmqihcfxxzdejletqjoxmwft"
    "lxfcxgxgvpperwbqvhxgsbbkmphyomtbjzdjhcrcsggleiczpbfjcgtpycpmrjnckslrwduqlccqmgrdhxolfjafmsrfdghnat"
    "exyanldrdpxvvgujsztuffoymrfteholgonuaqndinadtumnuhkboyzaqguwqijwxxszngextfcozpetyownmyneehdwqmtpjl"
    "oztswmzzdzqhuoxrblppqvyvsqhnhryvqsqogpnlqfulurexdtovqpqkfxxnqykgscxaskmksivoazlducanrqxynxlgvwonal"
    "psyddqmaemcrrwvrjmjjnygyebwtqxehrclwsxzylbqexnxjcgspeynlbmetlkacnnbhmaizbadynajpibepbuacggxrqavfnw"
    "pcwxbzxfymhjcslghmajrirqzjqxpgtgisfjreqrqabssobbadmtmdknmakdigjqyqcruujlwmfoagrckdwyiglviyyrekjeal"
    "vvigiesnvuumxgsveadrxlpwetioxibtdjblowblqvzpbrmhupyrdophjxvhgzclidzybajuxllacyhyphssvhcffxonysahvz"
    "hzbttyeeyiefhunbokiqrpqfcoxdxvefugapeevdoakxwzykmhbdytjbhigffkmbqmqxsoaiomgmmgwapzdosorcxxhejvgajy"
    "zdmzlcntqbapbpofdjtulstuzdrffafedufqwsknumcxbschdybosxkrabyfdejgyozwillcxpcaiehlelczioskqtptzaczob"
    "vyojdlyflilvwqgyrqmjaeepydrcchfyftjighntqzoo", "rwmimatmhydhbujebqehjprrwfkoebcxxqfktayaaeheys"));
}
