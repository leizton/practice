#include "util/base.h"


def(find_rfind) {
  string s = "a/b/c/d";
  assert_eq(std::string::npos, s.find('\\'));
  assert_eq(1, s.find('/'));
  assert_eq(3, s.find('/', 2));
  assert_eq(5, s.rfind('/'));
}


def(dirpath) {
  auto getDirpath = [] (const string& path) -> string {
    auto idx = path.rfind('/');
    return (idx == std::string::npos) ? "" : path.substr(0, idx+1);
  };
  assert_eq("/home/", getDirpath("/home/a.txt"));
  assert_eq("/home/lz/", getDirpath("/home/lz/a.txt"));
  assert_eq("", getDirpath("a.txt"));
}


main_run;