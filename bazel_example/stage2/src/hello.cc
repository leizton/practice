#include <iostream>
#include <string>
#include <memory>

#include "hello_util/hello_util.h"

int main() {
  std::shared_ptr<hello_util::HelloUtil> util =
      std::make_shared<hello_util::HelloUtil>();

  std::string name = "kitty";
  std::string greet;
  util->Greet(name, greet);
  std::cout << greet << std::endl;
}
