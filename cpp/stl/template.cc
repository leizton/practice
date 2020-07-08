#include "util/base.h"


def(get_name_from_template_type) {
  assert_eq("int", getTypeName<int>());
  assert_eq("unsigned int", getTypeName<uint32_t>());
  assert_eq("Aoo", getTypeName<Aoo>());
  assert_eq("void", getTypeName<void>());
  assert_eq("void", getTypeName<void>());
  assert_eq("void", getTypeName<void>());
}


main_run;