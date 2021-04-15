#include "util/base.h"
#include "util/json.hpp"

using json = nlohmann::json;

int main() {
  std::string home_dir = getenv("HOME");
  std::string json_path = home_dir + "/Library/Application Support/Code/User/settings.json";
  println("json_path: ", json_path);

  json ori_j;
  ifstream fin;
  fin.open(json_path);
  if (!fin.is_open()) {
    println("open fail");
    return 1;
  }
  fin >> ori_j;
  fin.close();

  // 按 key 升序遍历
  for (auto& e : ori_j.items()) {
    print(e.key());
  }

  return 0;
}
