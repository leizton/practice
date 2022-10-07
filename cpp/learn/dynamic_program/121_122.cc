#include <header>

/*
121.cc

给定一个数组 prices ，它的第 i 个元素 prices[i] 表示一支给定股票第 i 天的价格。

你只能选择 某一天 买入这只股票，并选择在 未来的某一个不同的日子 卖出该股票。
设计一个算法来计算你所能获取的最大利润。

返回你可以从这笔交易中获取的最大利润。如果你不能获取任何利润，返回 0 。

输入：[7,1,5,3,6,4]
输出：5
解释：在第 2 天（股票价格 = 1）的时候买入
     在第 5 天（股票价格 = 6）的时候卖出，最大利润 = 6-1 = 5
*/

int maxProfit_121(vector<int>& prices) {
  int min_buy_price = INT_MAX;
  int global_max_profit = 0;
  for (int price : prices) {
    // 第i天能拿到的最低买入价格
    min_buy_price = std::min(min_buy_price, price);
    // 如果在第i天卖出, 最大收益是当天价格减最低买入价格
    global_max_profit = std::max(global_max_profit, price - min_buy_price);
  }
  return global_max_profit;
}

/*
122.cc

可以进行多次交易，但只有卖出后才能再次买进，且一天中只能做买或卖一次操作

输入：prices = [7,1,5,3,6,4]
输出：7
解释：在第 2 天（股票价格 = 1）的时候买入，在第 3 天（股票价格 = 5）的时候卖出, 这笔交易所能获得利润 = 5 - 1 = 4 。
     随后，在第 4 天（股票价格 = 3）的时候买入，在第 5 天（股票价格 = 6）的时候卖出, 这笔交易所能获得利润 = 6 - 3 = 3 。
     总利润为 4 + 3 = 7 。
*/

/*
  每天交易结束后有两种状态: 持有股票 不持有股票
  用 dp[i][0] 表示第i天不持有股票的最大收益, dp[i][1] 对应持有
  得到状态转移方程如下:
    dp[i][0] = max{
      dp[i-1][0]             ; 昨天不持有, 今天也不买入
      dp[i-1][1]+price[i]    ; 昨天持有, 今天卖出增加price[i]
    }
    dp[i][1] = max{
      dp[i-1][0]-price[i]    ; 昨天不持有 今天买入
      dp[i-1][1]             ; 昨天持有, 今天继续持有
    }

  如果状态选择第[i,j]天内只完成一次买卖, 则状态空间很大
  全部待遍历的状态空间是 {
    maxProfit_121(1,2) + maxProfit_122(3,n)
    maxProfit_121(1,3) + maxProfit_122(4,n)
    ...
    maxProfit_121(1,n-2) + maxProfit_122(n-1,n)
  }
*/

int maxProfit_122(vector<int>& prices) {
  if (prices.empty()) return 0;
  int prev_0, prev_1, curr_0, curr_1;
  curr_0 = curr_1 = 0;
  prev_0 = 0;
  prev_1 = -prices[0];
  for (int price : prices) {
    curr_0 = std::max(prev_0, prev_1 + price);
    curr_1 = std::max(prev_0 - price, prev_1);
    prev_0 = curr_0;
    prev_1 = curr_1;
  }
  return std::max(curr_0, curr_1);
}

// 122.cc 的贪心解法
// 只要今天股价比昨天高, 就在昨天买入今天卖出
int maxProfit(vector<int>& prices) {
  int max_profit = 0;
  for (int i = 1; i < prices.size(); i++) {
    if (prices[i] > prices[i-1]) {
      max_profit += prices[i] - prices[i-1];
    }
  }
  return max_profit;
}