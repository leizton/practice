#include "cpp_lib.h"

vector<string> getWords() {
  vector<string> words;
  words.reserve(8000);

  ifstream fin("util/gre_words.txt");
  string line;
  while (std::getline(fin, line)) {
    if (line.empty()) continue;
    string::size_type i = line.find(" ");
    if (i == string::npos || i == 0) continue;
    words.emplace_back(line, 0, i);
  }
  fin.close();
  return words;
}
