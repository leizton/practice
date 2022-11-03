#include <header>

/*
给你一个整数数组 coins ，表示不同面额的硬币；以及一个整数 amount ，表示总金额。
计算并返回可以凑成总金额所需的 最少的硬币个数 。如果没有任何一种硬币组合能组成总金额，返回 -1 。
你可以认为每种硬币的数量是无限的

输入：coins = [1, 2, 5], amount = 11
输出：3 
解释：11 = 5 + 5 + 1
*/

int dfs(vector<int>& coins, int amount, vector<int>& memo) {
  // 每次选择一个硬币
  // 每个树节点有coins.size个分叉
  // 树节点含义是用coins构成amount的最少硬币数
  int result = INT_MAX;
  for (int coin : coins) {
    int left_amount = amount - coin;
    if (left_amount == 0) { memo[amount] = 1; return 1;}
    if (left_amount < 0) continue;
    //
    int min_take_num = memo[left_amount];
    if (min_take_num == -2) {
      min_take_num = dfs(coins, left_amount, memo);
    }
    if (0 < min_take_num && min_take_num < result) {
      result = min_take_num;
    }
  }
  result = result==INT_MAX ? -1 : result+1;
  memo[amount] = result;
  return result;
}
int coinChange(vector<int>& coins, int amount) {
  if (amount <= 0) return 0;
  // memo[i]: amount==i时的最少硬币数
  // memo从map换成vector后速度提升10倍
  vector<int> memo(amount+1, -2);
  return dfs(coins, amount, memo);
}

int coinChange_v1(vector<int>& coins, int amount) {
  // dp[i]: amount==i时的最少硬币数
  vector<int> dp(amount+1, amount+1);
  dp[0] = 0;
  for (int coin : coins) {
    for (int i = coin; i <= amount; i++) {
      dp[i] = min(dp[i], dp[i-coin] + 1);
    }
  }
  return dp[amount] > amount ? -1 : dp[amount];
}

int main() {
  auto test = [] (vector<int> coins, int amount) {
    return coinChange(coins, amount);
  };
  assert_eq(-1, test({2}, 1));
  assert_eq(-1, test({2}, 3));
  assert_eq(3, test({1,2,5}, 11));
  assert_eq(20, test({186,419,83,408}, 6249));
  assert_eq(24, test({411,412,413,414,415,416,417,418,419,420,421,422}, 9864));
}