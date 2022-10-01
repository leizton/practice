#include <header>

/*
给定一个数组 prices ，它的第 i 个元素 prices[i] 表示一支给定股票第 i 天的价格。

你只能选择 某一天 买入这只股票，并选择在 未来的某一个不同的日子 卖出该股票。
设计一个算法来计算你所能获取的最大利润。

返回你可以从这笔交易中获取的最大利润。如果你不能获取任何利润，返回 0 。

输入：[7,1,5,3,6,4]
输出：5
解释：在第 2 天（股票价格 = 1）的时候买入
     在第 5 天（股票价格 = 6）的时候卖出，最大利润 = 6-1 = 5
*/

int maxProfit(vector<int>& prices) {
  int min_buy_price = INT_MAX;
  int max_profit = 0;

  for (int price : prices) {
    // 第i天能拿到的最低买入价格
    min_buy_price = std::min(min_buy_price, price);
    // 如果在第i天卖出, 最大收益是当天价格减最低买入价格
    max_profit = std::max(max_profit, price - min_buy_price);
  }
  return max_profit;
}
