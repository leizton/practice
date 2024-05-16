#include <header>

/*
已知每天股票价格prices[i], 最多可以操作k次, 返回最大收益

每天最多3种选择: buy sell skip

3个状态值的组合: 第i天, 最多操作了k次, 持有或空仓股票(1/0)
dp[i][k][0]: 截止第i天, 最多操作了k次(剩余最多可操作K-k次)空仓的最大收益
dp[i][k][1]: 截止第i天, 最多操作了k次(剩余最多可操作K-k次)持有的最大收益
全局最大收益 = dp[last_day][K][0]

状态转移方程
dp[i][k][0] = max(dp[i-1][k][0], dp[i-1][k][1]+prices[i])
            = max(今天skip,      今天sell)
dp[i][k][1] = max(dp[i-1][k][1], dp[i-1][k-1][0]-prices[i])
            = max(今天skip,      今天buy)
*/
int maxProfit(int K, vector<int>& prices) {
  const int day_num = (int)prices.size();
  vector<int> prev_hold_dp(K+1, 0), prev_empty_dp(K+1, 0);  // 前一天
  vector<int> hold_dp(K+1, 0), empty_dp(K+1, 0);  // 今天

  // 第一天
  for (int k = 1; k <= K; k++) {
    hold_dp[k] = -prices[0];
  }
  for (int i = 1; i < day_num; i++) {
    prev_empty_dp = empty_dp;
    prev_hold_dp = hold_dp;
    for (int k = 1; k <= K; k++) {
      empty_dp[k] = max(prev_empty_dp[k], prev_hold_dp[k] + prices[i]);
      hold_dp[k] = max(prev_hold_dp[k], prev_empty_dp[k-1] - prices[i]);
    }
  }
  return empty_dp[K];
}

int main() {
  vector<int> prices{3,3,5,0,0,3,1,4};
  print(maxProfit(2, prices));
}
