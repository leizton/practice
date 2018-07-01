//
//  solu.cpp
//  practice
//
//  Created by bytedance on 2018/7/1.
//  Copyright © 2018年 wh. All rights reserved.
//

#include <iostream>
#include <string>
#include <map>
#include <unordered_map>

using namespace std;

class RomanToInteger {
private:
  static unordered_map<char, int> romanMap;
  
public:
  int romanToInt(string s) {
    int ret = 0;
    int i = 0;
    for (; i < s.size() - 1; ) {
      int curr = romanMap[s[i]];
      int next = romanMap[s[i + 1]];
      if (curr < next) {
        ret += next - curr;
        i += 2;
      } else {
        ret += curr;
        i++;
      }
    }
    if (i < s.size()) {
      ret += romanMap[s[i]];
    }
    return ret;
  }
};

unordered_map<char, int> RomanToInteger::romanMap = {
  { 'I', 1},
  { 'V', 5},
  { 'X', 10},
  { 'L', 50},
  { 'C', 100},
  { 'D', 500},
  { 'M', 1000}
};

class Solution {
public:
  static void test() {
  }
  
private:
  static void testRomanToInteger() {
    RomanToInteger solu;
    cout << solu.romanToInt("LVIII") << endl;
    cout << solu.romanToInt("MCMXCIV") << endl;
  }
};
