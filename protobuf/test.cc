#include <iostream>
#include <fstream>

#include <memory>
#include <string>
#include <algorithm>
#include <vector>
#include <list>

#include <type_traits>
#include <typeinfo>

#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

#include "protobuf/user.pb.h"
#include "protobuf/goods.pb.h"
#include "protobuf/shop_item.pb.h"
#include "protobuf/shop.pb.h"

using namespace std;

void buildShopCart(prototest::ShopCart& shop) {
  prototest::User& user = *shop.mutable_user();
  user.set_id(1001);
  user.set_name("Zhang");

  prototest::ShopItem& item = *shop.add_shoplist();
  prototest::GoodsItem& goods = *item.mutable_goods();
  goods.set_id(2001);
  goods.set_name("apple");
  goods.set_price(1.5);
  item.set_num(3);
}

double calcTotalPrice(const prototest::ShopCart& shop) {
  double total_price = 0.;
  for (auto i = 0; i < shop.shoplist_size(); i++) {
    auto& item = shop.shoplist(i);
    if (item.has_goods() && item.has_num()) {
      total_price += item.goods().price() * item.num();
    }
  }
  return total_price;
}

bool appendRepeated(const std::string& raw,
                    google::protobuf::Message* msg,
                    const std::string& repeated_field_name)
{
  const google::protobuf::Descriptor* desc = msg->GetDescriptor();
  const google::protobuf::FieldDescriptor* repeated_field = nullptr;
  for (auto i = 0; i < desc->field_count(); i++) {
    const google::protobuf::FieldDescriptor* field = desc->field(i);
    if (field->name() == repeated_field_name) {
      repeated_field = field;
      break;
    }
  }
  if (repeated_field == nullptr) {
    return false;
  }

  const google::protobuf::Reflection* refl = msg->GetReflection();
  google::protobuf::Message* repeated_msg = refl->AddMessage(msg, repeated_field);
  repeated_msg->ParseFromString(raw);
  return true;
}


// #define RUN test
void test() {
  prototest::ShopCart shop;
  buildShopCart(shop);
  cout << calcTotalPrice(shop) << endl;
}


// #define RUN testSerDeser
void testSerDeser() {
  prototest::ShopCart shop;
  buildShopCart(shop);
  cout << calcTotalPrice(shop) << endl;

  string raw;
  shop.SerializeToString(&raw);
  prototest::ShopCart shop_rep;
  shop_rep.ParseFromString(raw);
  cout << calcTotalPrice(shop_rep) << endl;
}


#define RUN testReflection
void testReflection() {
  prototest::ShopCart shop;
  buildShopCart(shop);

  string raw;
  shop.shoplist(0).SerializeToString(&raw);
  appendRepeated(raw, &shop, "shopList");
  cout << calcTotalPrice(shop) << endl;
}

#define RUNNAME(run) #run

int main() {
  std::cout << std::boolalpha;
  std::cout << "----------" << RUNNAME(RUN) << std::endl;
  RUN();
  std::cout << "----------" << std::endl;
}
