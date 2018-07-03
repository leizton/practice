//
//  solu.cpp
//  practice
//
//  Created by bytedance on 2018/7/1.
//  Copyright © 2018年 wh. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class _3Sum {
public:
  vector<vector<int>> threeSum(vector<int>& nums) {
    vector<vector<int>> ret;
    if (nums.size() < 3) {
      return ret;
    }
    sort(nums.begin(), nums.end());

    unordered_map<int, int> allNums;
    for (int i = 0; i < nums.size(); i++) {
      allNums[nums[i]] = i;
    }
    const int size1 = nums.size() - 1;

    for (int i = 0; i < size1; i++) {
      int first = nums[i];
      if (i > 0 && first == nums[i - 1]) {
        continue;
      }
      for (int j = i + 1; j < size1; ) {
        int second = nums[j];
        int target = - first - second;
        if (allNums[target] > j) {
          ret.push_back({first, second, target});
        }
        while (++j < size1 && nums[j] == second);
      }
    }
    return ret;
  }
};

class _RomanToInteger {
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

unordered_map<char, int> _RomanToInteger::romanMap = {
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
    test3Sum();
  }

private:
  static void test3Sum() {
    _3Sum solu;
    vector<int> nums = {-1, 0, 1, 2, -1, -4};
    vector<vector<int>> ret = solu.threeSum(nums);
    for (auto& v : ret) {
      cout << v[0] << ", " << v[1] << ", " << v[2] << endl;
    }
    cout << "-----" << endl;
    nums = {0, 0, 0};
    ret = solu.threeSum(nums);
    for (auto& v : ret) {
      cout << v[0] << ", " << v[1] << ", " << v[2] << endl;
    }
    cout << "-----" << endl;
    nums = {0, 0, 0, 0};
    ret = solu.threeSum(nums);
    for (auto& v : ret) {
      cout << v[0] << ", " << v[1] << ", " << v[2] << endl;
    }
    cout << "-----" << endl;
    nums = {};
    ret = solu.threeSum(nums);
    for (auto& v : ret) {
      cout << v[0] << ", " << v[1] << ", " << v[2] << endl;
    }
    cout << "-----" << endl;
  }

  static void testRomanToInteger() {
    _RomanToInteger solu;
    cout << solu.romanToInt("LVIII") << endl;
    cout << solu.romanToInt("MCMXCIV") << endl;
  }
};
