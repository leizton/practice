//
//  001.cpp
//  practice
//
//  Created by bytedance on 2018/6/24.
//  Copyright © 2018年 wh. All rights reserved.
//

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;

namespace p001 {

  void test() {
  }

  void test_3() {
    set<pair<int, int>> s;
    s.insert(make_pair(1, 1));
    s.insert(make_pair(1, 1));
    s.insert(make_pair(1, 2));
    cout << (s.find(make_pair(1, 1)) != s.end()) << endl;
    cout << (s.find(make_pair(1, 2)) != s.end()) << endl;
    cout << (s.find(make_pair(1, 3)) != s.end()) << endl;
  }

  void test_2() {
    unordered_map<int, set<int>> map;
    map[1] = {1};
    map[1].insert(2);
    map[2] = {0};
    for (auto& e : map) {
      cout << e.first << ": ";
      for (auto& e1 : e.second) {
        cout << e1 << " ";
      }
      cout << endl;
    }
  }

  void test_1() {
    unordered_map<char, int> map;
    // set & get
    map['a'] = 1;
    ++map['a'];
    ++map['b'];
    cout << map['a'] << endl;
    cout << map['b'] << endl;
    // earse
    if (--map['b'] == 0) {
      map.erase('b');
    }
    // for_each
    for_each(map.begin(), map.end(), [](auto& e) {
      cout << e.first << ": " << e.second << endl;
    });
    map['b'] = 1;
    for (auto& e : map) {
      cout << e.first << ": " << e.second << endl;
    }
  }

  void test_0() {
    ifstream in("/opt/tmp/t");
    if (not in) {
      return;
    }
    vector<string> text;
    string line;
    while (getline(in, line)) {
      text.push_back(line);
    }
    sort(text.begin(), text.end());
    for_each(text.begin(), text.end(), [](string s) { cout << s << endl; });
  }

}
