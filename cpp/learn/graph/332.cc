#include <header>

/*
给你一份航线列表 tickets
其中 tickets[i] = [from_i, to_i] 表示飞机出发和降落的机场地点
起始地必须是 JFK
请规划一个包含所有机票(遍历所有边而非节点)的行程, 且该行程在所有有效行程中字典序最小

Hierholzer算法：在连通图中找欧拉路径

https://zhuanlan.zhihu.com/p/639902025
*/

struct Site {
  int id;
  int accessed;
  Site() : id(-1), accessed(0) {}
  Site(int _id) : id(_id), accessed(0) {}
  bool operator<(const Site& x) { return id < x.id; }
};

bool dfs(vector<vector<Site>>& edges, int curr_site, vector<int>& path) {
  for (auto& next_site : edges[curr_site]) {
    if (next_site.accessed) continue;
    next_site.accessed = 1;
    dfs(edges, next_site.id, path);
  }
  path.push_back(curr_site);
  return false;
}

vector<string> findItinerary(vector<vector<string>>& tickets) {
  map<string, int> site_map;
  for (auto& ticket : tickets) {
    site_map.emplace(ticket[0], -1);
    site_map.emplace(ticket[1], -1);
  }
  const int site_num = int(site_map.size());
  vector<string> site_vec;
  site_vec.reserve(site_num);
  for (auto& e : site_map) {
    e.second = int(site_vec.size());
    site_vec.push_back(e.first);
  }

  vector<vector<Site>> edges(site_num);
  for (auto& ticket : tickets) {
    int from = site_map[ticket[0]];
    int to = site_map[ticket[1]];
    edges[from].emplace_back(to);
  }
  for (auto& to_sites : edges) {
    sort(to_sites.begin(), to_sites.end());
  }

  int valid_path_length = int(tickets.size()) + 1;
  vector<int> path;
  path.reserve(valid_path_length);
  dfs(edges, site_map["JFK"], path);
  reverse(path.begin(), path.end());

  vector<string> res;
  res.reserve(site_num);
  for (int site : path) {
    res.push_back(site_vec[site]);
  }
  return res;
}

void test(string input) {
  vector<string> a;
  splitString(input.substr(2, input.size()-4), "],[", a);
  vector<vector<string>> tickets;
  for (auto& s : a) {
    tickets.push_back(vector<string>());
    splitString(s, ",", tickets.back());
  }
  print(findItinerary(tickets));
}

int main() {
  test("[[JFK,SFO],[JFK,ATL],[SFO,JFK],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL],[ATL,AAA],[AAA,BBB],[BBB,ATL]]");
}
