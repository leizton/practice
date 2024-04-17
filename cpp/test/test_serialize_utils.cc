#include <header>

#define OPERATOR_EQUAL_FAILSE(clz, fd) { LOG_ERROR() << "not equal: " << #clz << "::" << #fd; return false; }

struct ShoppingItem : public SerializeApi {
#define SERIALIZE_UTILS_FIELD_LIST(field) \
  field(1, name, std::string) \
  field(2, price, double) \
  ;
  SERIALIZE_UTILS_API_IMPL;
#undef SERIALIZE_UTILS_FIELD_LIST

  ShoppingItem() {
  }
  ShoppingItem(const std::string& name, double price) {
    this->name = name;
    this->price = price;
  }

  bool operator==(const ShoppingItem& x) const {
    if (name != x.name) OPERATOR_EQUAL_FAILSE(ShoppingItem, name);
    if (price != x.price) OPERATOR_EQUAL_FAILSE(ShoppingItem, price);
    return true;
  }
};

struct ShoppingList : public SerializeApi {
#define SERIALIZE_UTILS_FIELD_LIST(field) \
  field(1, timestamp, int64_t) \
  field(2, user, std::string) \
  field(3, total_price, double) \
  field(4, items, std::vector<ShoppingItem>) \
  field(5, item_counts, std::unordered_map<std::string, int>) \
  ;
  SERIALIZE_UTILS_API_IMPL;
#undef SERIALIZE_UTILS_FIELD_LIST

  bool operator==(const ShoppingList& x) const {
    if (timestamp != x.timestamp) OPERATOR_EQUAL_FAILSE(ShoppingList, timestamp);
    if (user != x.user) OPERATOR_EQUAL_FAILSE(ShoppingList, user);
    if (total_price != x.total_price) OPERATOR_EQUAL_FAILSE(ShoppingList, total_price);
    if (items.size() != x.items.size()) OPERATOR_EQUAL_FAILSE(ShoppingList, items);
    if (item_counts.size() != x.item_counts.size()) OPERATOR_EQUAL_FAILSE(ShoppingList, item_counts);
    for (size_t i = 0; i < items.size(); i++) {
      if (items[i] != x.items[i]) OPERATOR_EQUAL_FAILSE(ShoppingList, items);
    }
    for (auto& e : item_counts) {
      auto iter = x.item_counts.find(e.first);
      if (iter == x.item_counts.end()) OPERATOR_EQUAL_FAILSE(ShoppingList, item_counts);
      if (iter->second != e.second) OPERATOR_EQUAL_FAILSE(ShoppingList, item_counts);
    }
    return true;
  }

  ShoppingList() {
    total_price = 0;
  }

  void AddItem(const std::string& name, double price) {
    if (price <= 0) return;
    total_price += price;
    items.emplace_back(name, price);
    item_counts[name] += 1;
  }
};

void test() {
  ShoppingList sl1;
  sl1.timestamp = nowUs();
  sl1.user = "Alice";
  sl1.AddItem("a", 0.11);
  sl1.AddItem("b", 0.12);
  sl1.AddItem("c", 0.13);
  sl1.AddItem("c", 0.13);
  print(sl1.item_counts);

  std::string raw;
  ShoppingList sl2;
  int ser_n = 10000;
  int ser_size = 0, deser_size = 0;

  auto t0 = nowUs();
  for (int i = 0; i < ser_n; i++) {
    raw.clear();
    ser_size = SerializeUtils::SerializeToString(sl1, raw);
    deser_size = SerializeUtils::DeserializeFromString(sl2, raw);
  }
  auto cost = double(nowUs() - t0) / ser_n;

  assert_T(ser_size > 0);
  assert_T(deser_size > 0);
  assert_eq(ser_size, deser_size);
  assert_eq(ser_size, int(raw.length()));
  println("size:", raw.length());
  println("cost:", cost, "us");

  assert_T(sl1 == sl2);
  print(sl2.item_counts);
}

int main() {
  test();
}
