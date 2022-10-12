#include <header>

/*
地下城是由 m × n 个房间组成的二维网格
一些恶魔抓住了公主并将她关在了地下城的右下角(m-1,n-1)
骑士最初被安置在左上角(0,0)，他必须穿过地下城对抗恶魔来拯救公主

网格中负数表示恶魔，会减少骑士的生命值；正数表示药，会增加骑士的生命值
如下 3×3 网格
    -2	 -3	  3
    -5	-10	  1
    10	 30	 -5
骑士在每个网格中的生命值至少是1
求骑士最初应该至少有多少生命值
上图的骑士至少需要7个生命值，一条最佳路径 (0,0) -> (0,1) -> (0,2) -> (1,2) -> (2,2)
*/

int calculateMinimumHP(vector<vector<int>>& dp) {
  if (dp.empty() || dp[0].empty()) return 0;
  int m = dp.size(), n = dp[0].size();
  dp[m-1][n-1] = std::max(1, 1 - dp[m-1][n-1]);
  for (int j = n-2; j >= 0; j--) {
    dp[m-1][j] = std::max(1, dp[m-1][j+1] - dp[m-1][j]);
  }
  for (int i = m-2; i >= 0; i--) {
    dp[i][n-1] = std::max(1, dp[i+1][n-1] - dp[i][n-1]);
  }
  for (int i = m-2; i >= 0; i--) {
    for (int j = n-2; j >= 0; j--) {
      dp[i][j] = std::max(1, std::min(dp[i+1][j] - dp[i][j], dp[i][j+1] - dp[i][j]));
    }
  }
  return dp[0][0];
}
