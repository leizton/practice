#include "util/base.h"

def(read_file_to_string) {
  ifstream in("stl/fstream.cc");
  ostringstream ss;
  ss << in.rdbuf();
  print(ss.str());
}

main_run;