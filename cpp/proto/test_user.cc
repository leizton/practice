#include "util/base.h"

#include "user.pb.h"

int main() {
  std::string data;

  reco::User user;
  user.set_id(1);
  user.set_name("a");
  user.add_tags("t1");
  user.add_tags("t2");
  LOG() << user.DebugString();
  LOG() << "------------------------------";

  user.SerializeToString(&data);

  reco::UserVer1 user1;
  user1.ParseFromString(data);
  user1.add_cates("c1");
  LOG() << user1.DebugString();
  LOG() << "------------------------------";

  user1.SerializeToString(&data);

  reco::User user0;
  user0.ParseFromString(data);
  LOG() << user0.DebugString();
  LOG() << "------------------------------";
}
