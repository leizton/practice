#include "util/base.h"

run(test_lru_cache) {
  LruCache<int, int> cache(10);

  for (int key = 1; key <= 10; key++) {
    assert_T(cache.put(key, 1));
    assert_T(cache.size() == key);

    auto iter = cache.find(key);
    assert_T(iter != cache.end());
    assert_T(iter->second.val == 1);
  }

  assert_T(cache.put(11, 1));
  assert_T(cache.size() == 10);
  assert_T(cache.find(1) == cache.end());

  assert_F(cache.put(3, 1));
  for (int key = 2; key <= 9; key++) {
    assert_T(cache.put(key + 10, 1));
    assert_T(cache.size() == 10);
  }
  assert_T(cache.find(3) != cache.end());
}

main_run;
