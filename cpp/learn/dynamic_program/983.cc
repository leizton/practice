#include <header>

/*
在一个火车旅行很受欢迎的国度，你提前一年计划了一些火车旅行。在接下来的一年里，你要旅行的日子将以一个名为 days 的数组给出。每一项是一个从 1 到 365 的整数。

火车票有 三种不同的销售方式 ：

一张 为期一天 的通行证售价为 costs[0] 美元；
一张 为期七天 的通行证售价为 costs[1] 美元；
一张 为期三十天 的通行证售价为 costs[2] 美元。
通行证允许数天无限制的旅行。 例如，如果我们在第 2 天获得一张 为期 7 天 的通行证，那么我们可以连着旅行 7 天：第 2 天、第 3 天、第 4 天、第 5 天、第 6 天、第 7 天和第 8 天。

返回 你想要完成在给定的列表 days 中列出的每一天的旅行所需要的最低消费 。

示例 1：
输入：days = [1,4,6,7,8,20], costs = [2,7,15]
输出：11
解释： 
  例如，这里有一种购买通行证的方法，可以让你完成你的旅行计划：
  在第 1 天，你花了 costs[0] = $2 买了一张为期 1 天的通行证，它将在第 1 天生效。
  在第 3 天，你花了 costs[1] = $7 买了一张为期 7 天的通行证，它将在第 3, 4, ..., 9 天生效。
  在第 20 天，你花了 costs[0] = $2 买了一张为期 1 天的通行证，它将在第 20 天生效。
  你总共花了 $11，并完成了你计划的每一天旅行。
*/

int dfs(vector<int>& days, vector<int>& costs, vector<int>& effective_days, int day_begin, vector<int>& dp) {
  // 树节点的最优值: days[day_begin:-1]所需的最小费用
  // 当前树节点有3种选择
  if (days.size() <= day_begin) return 0;
  int min_cost = dp[day_begin];
  if (min_cost != -1) return min_cost;

  min_cost = costs[0] + dfs(days, costs, effective_days, day_begin+1, dp);
  for (int j = 1; j < ; j++) {
    int d = days[day_begin] + effective_days[j];
    int i = day_begin + 1;
    while (i < days.size() && days[i] < d) i++;
    if (i < days.size()) {
      min_cost = std::min(min_cost, costs[j] + dfs(days, costs, effective_days, i, dp));
    } else {
      min_cost = std::min(min_cost, costs[j]);
    }
  }

  dp[day_begin] = min_cost;
  return min_cost;
}
int mincostTickets(vector<int>& days, vector<int>& costs) {
  vector<int> dp(days.size(), -1);
  dp[days.size()-1] = std::min({costs[0], costs[1], costs[2]});
  vector<int> effective_days{1, 7, 30};
  return dfs(days, costs, effective_days, 0, dp);
}
