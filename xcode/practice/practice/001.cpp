//
//  001.cpp
//  practice
//
//  Created by bytedance on 2018/6/24.
//  Copyright © 2018年 wh. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

namespace p001 {

  int test() {
    ifstream in("/opt/tmp/t");
    if (not in) {
      return -1;
    }
    vector<string> text;
    string line;
    while (getline(in, line)) {
      text.push_back(line);
    }
    sort(text.begin(), text.end());
    for_each(text.begin(), text.end(), [](string s) { cout << s << endl; });
    return 0;
  }
  
}
