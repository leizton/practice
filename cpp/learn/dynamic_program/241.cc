#include <header>

/*
给你一个由数字和运算符组成的字符串 expr
按不同优先级组合数字和运算符，计算并返回所有可能组合的结果
expr只包含 0~9 + - *

输入：expr = "2*3-4*5"
输出：[-34,-14,-10,-10,10]
解释：(2*(3-(4*5))) = -34 
     ((2*3)-(4*5)) = -14 
     ((2*(3-4))*5) = -10 
     (2*((3-4)*5)) = -10 
     (((2*3)-4)*5) = 10
*/

vector<int> diffWaysToCompute(string expression) {
  vector<int> expr;
  expr.reserve(expression.size());
  for (int i = 0; i < (int)expression.size(); i++) {
    if ('0' <= expression[i] && expression[i] <= '9') {
      int v = int(expression[i] - '0');
      i++;
      while (i < (int)expression.size() && '0' <= expression[i] && expression[i] <= '9') {
        v = 10 * v + int(expression[i] - '0');
        i++;
      }
      i--;
      expr.push_back(v);
    } else if (expression[i] == '+') {
      expr.push_back(-1);
    } else if (expression[i] == '-') {
      expr.push_back(-2);
    } else if (expression[i] == '*') {
      expr.push_back(-3);
    }
  }
  int n = expr.size();

  vector<function<int(int,int)>> ops {
    [] (int x, int y) { return x + y; },
    [] (int x, int y) { return x + y; },
    [] (int x, int y) { return x - y; },
    [] (int x, int y) { return x * y; },
  };

  // dp[l,r] = expr[l:r]上的所有可能组合
  vector<vector<vector<int>>> dp(n, vector<vector<int>>(n));
  for (int i = 0; i < n; i += 2) {
    dp[i][i].push_back(expr[i]);
  }
  for (int i = 0; i < n-2; i += 2) {
    dp[i][i+2].push_back(ops[-expr[i+1]](expr[i], expr[i+2]));
  }
  for (int k = 4; k < n; k += 2) {
    for (int i = 0; i < n-k; i += 2) {
      int j = i + k;
      // solve dp[i,j]
      // example: dp[0,6] = { dp[0,4]+dp[6], dp[0,2]+dp[4,6], dp[0]+dp[2,6] }
      for (int split = i; split <= j-2; split += 2) {
        // dp[i,split]+dp[split+2,j]
        auto& op = ops[-expr[split+1]];
        for (int lv : dp[i][split])
          for (int rv : dp[split+2][j])
            dp[i][j].push_back(op(lv, rv));
      }
    }
  }
  return dp[0][n-1];
}

int main() {
  auto ans = diffWaysToCompute("2*3-4*5");
  print(ans);  // [-34,-10,-14,-10,10]
}
